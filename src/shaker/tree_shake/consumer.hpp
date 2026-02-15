#pragma once

#include "config.hpp"
#include "decl_category.hpp"
#include "helpers.hpp"
#include "ref_collector.hpp"

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Lexer.h>
#include <llvm/Support/raw_ostream.h>

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

namespace shaker {

class TreeShakeConsumer : public clang::ASTConsumer {
public:
	explicit TreeShakeConsumer(clang::CompilerInstance& CI)
		: sm_(CI.getSourceManager()), lo_(CI.getLangOpts()) {}

	void HandleTranslationUnit(clang::ASTContext& Ctx) override {
		// ── Phase 1: collect main-file top-level declarations ────
		llvm::StringRef MainBuf = sm_.getBufferData(sm_.getMainFileID());
		const unsigned MainSize = static_cast<unsigned>(MainBuf.size());

		struct Info {
			const clang::Decl* D;
			unsigned BOff, EOff; // half-open file-offset range [BOff, EOff)
			unsigned BLine, ELine; // inclusive source-line range (for diagnostics)
			DeclCat Cat;
			bool Root;
		};
		std::vector<Info> TL;						// top-level list
		std::map<const clang::Decl*, unsigned> Idx; // canonical → index

		auto reg = [&](const clang::Decl* D, unsigned i) {
			if (D) Idx[D->getCanonicalDecl()] = i;
		};

		auto collect = [&](this auto&& self, clang::DeclContext* DC) -> void {
			auto extendToDeclSemicolon = [&](unsigned Off) -> unsigned {
				if (Off >= MainSize) return Off;
				int Paren = 0, Bracket = 0;
				for (unsigned i = Off; i < MainSize; ++i) {
					char c = MainBuf[i];
					if (c == '(') {
						++Paren;
						continue;
					}
					if (c == ')') {
						if (Paren > 0) --Paren;
						continue;
					}
					if (c == '[') {
						++Bracket;
						continue;
					}
					if (c == ']') {
						if (Bracket > 0) --Bracket;
						continue;
					}
					if (c == '{' && Paren == 0 && Bracket == 0)
						return Off;
					if (c == ';' && Paren == 0 && Bracket == 0)
						return i + 1;
				}
				return Off;
			};

			for (auto* D : DC->decls()) {
				// Transparent contexts: recurse into extern "C" { }
				if (auto* LS = llvm::dyn_cast<clang::LinkageSpecDecl>(D)) {
					self(LS);
					continue;
				}

				if (auto* NS = llvm::dyn_cast<clang::NamespaceDecl>(D)) {
					self(NS);
					continue;
				}

				if (D->isImplicit()) continue;

				// Skip implicit template instantiations
				if (auto* CTSD =
						llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(D))
					if (!CTSD->isExplicitSpecialization())
						continue;

				auto Loc = sm_.getExpansionLoc(D->getLocation());
				if (!Loc.isValid() || !sm_.isWrittenInMainFile(Loc))
					continue;

				llvm::errs() << "tree-shake: consider " << D->getDeclKindName() << " at "
							 << D->getLocation().printToString(sm_) << "\n";

				auto BLoc = sm_.getExpansionLoc(D->getBeginLoc());
				auto ELoc = sm_.getExpansionLoc(D->getEndLoc());
				if (!BLoc.isValid() || !ELoc.isValid()) continue;

				unsigned BL = sm_.getExpansionLineNumber(BLoc);
				unsigned EL = sm_.getExpansionLineNumber(ELoc);
				unsigned BO = sm_.getFileOffset(BLoc);
				unsigned EO = 0;

				// Include trailing ';' (may sit on the same or next line)
				if (auto A = clang::Lexer::findLocationAfterToken(
						ELoc, clang::tok::semi, sm_, lo_, false);
					A.isValid()) {
					auto ALoc = sm_.getExpansionLoc(A);
					unsigned SL = sm_.getExpansionLineNumber(ALoc);
					if (SL <= EL + 1) EL = SL;
					EO = sm_.getFileOffset(ALoc);
				} else {
					auto EndTok = clang::Lexer::getLocForEndOfToken(
						ELoc, 0, sm_, lo_);
					if (EndTok.isValid()) {
						EndTok = sm_.getExpansionLoc(EndTok);
						EO = sm_.getFileOffset(EndTok);
					} else {
						EO = sm_.getFileOffset(ELoc) + 1;
					}
				}

				if (EO < BO) std::swap(BO, EO);

				bool HasFunctionBody = false;
				if (auto* FD = llvm::dyn_cast<clang::FunctionDecl>(D))
					HasFunctionBody = FD->doesThisDeclarationHaveABody();
				if (auto* FT = llvm::dyn_cast<clang::FunctionTemplateDecl>(D))
					HasFunctionBody = FT->getTemplatedDecl()->doesThisDeclarationHaveABody();
				if (!HasFunctionBody) {
					unsigned Ext = extendToDeclSemicolon(EO);
					if (Ext > EO) EO = Ext;
				}

				DeclCat Cat = classify(D);
				bool Root = false;

				// main() is always a root
				if (auto* FD = llvm::dyn_cast<clang::FunctionDecl>(D))
					if (FD->isMain()) Root = true;
				if (auto* FT = llvm::dyn_cast<clang::FunctionTemplateDecl>(D))
					if (FT->getTemplatedDecl()->isMain()) Root = true;

				// Global vars with side-effecting init are roots at
				// lower strengths (at strength >= 2, only main() is root)
				if (ShakeStrength <= 1)
					if (auto* VD = llvm::dyn_cast<clang::VarDecl>(D))
						if (hasSideEffectInit(VD, Ctx))
							Root = true;

				auto i = static_cast<unsigned>(TL.size());
				reg(D, i);

				// Also register inner decl for templates so that
				// references to the templated entity resolve correctly
				if (auto* CT = llvm::dyn_cast<clang::ClassTemplateDecl>(D))
					reg(CT->getTemplatedDecl(), i);
				if (auto* FT = llvm::dyn_cast<clang::FunctionTemplateDecl>(D))
					reg(FT->getTemplatedDecl(), i);
				if (auto* TA = llvm::dyn_cast<clang::TypeAliasTemplateDecl>(D))
					reg(TA->getTemplatedDecl(), i);
				// Register enumerators → owning EnumDecl
				if (auto* ED = llvm::dyn_cast<clang::EnumDecl>(D))
					for (auto* EC : ED->enumerators()) reg(EC, i);

				TL.push_back({ D, BO, EO, BL, EL, Cat, Root });
			}
		};
		collect(Ctx.getTranslationUnitDecl());

		if (TL.empty()) {
			// no top-level decls → emit the whole file unchanged
			llvm::outs() << sm_.getBufferData(sm_.getMainFileID());
			return;
		}

		// ── Phase 2: build reference graph ───────────────────────

		const auto N = static_cast<unsigned>(TL.size());
		std::vector<std::set<unsigned>> Adj(N);

		for (unsigned i = 0; i < N; ++i) {
			RefCollector RC;
			RC.TraverseDecl(const_cast<clang::Decl*>(TL[i].D));

			for (const clang::Decl* Ref : RC.Found) {
				// Try direct lookup first
				auto It = Idx.find(Ref);
				if (It == Idx.end()) {
					// Fall back to the top-level parent of the referenced decl
					if (auto* P = topLevelOf(Ref))
						It = Idx.find(P->getCanonicalDecl());
				}
				if (It != Idx.end() && It->second != i)
					Adj[i].insert(It->second);
			}
		}

		// ── Phase 3: BFS reachability ────────────────────────────

		std::vector<bool> Alive(N, false);
		std::queue<unsigned> Q;

		for (unsigned i = 0; i < N; ++i) {
			// Alive if it is an explicit root, or if its category is
			// not shakeable at the current strength level
			if (TL[i].Root || !canShake(TL[i].Cat, ShakeStrength)) {
				Alive[i] = true;
				Q.push(i);
			}
		}
		while (!Q.empty()) {
			auto u = Q.front();
			Q.pop();
			for (auto v : Adj[u])
				if (!Alive[v]) {
					Alive[v] = true;
					Q.push(v);
				}
		}

		// ── diagnostics on stderr ────────────────────────────────

		for (unsigned i = 0; i < N; ++i) {
			if (Alive[i]) continue;
			llvm::StringRef Name = "<anon>";
			if (auto* ND = llvm::dyn_cast<clang::NamedDecl>(TL[i].D))
				if (!ND->getName().empty()) Name = ND->getName();
			llvm::errs() << "tree-shake: remove " << catName(TL[i].Cat)
							 << " '" << Name << "' (L" << TL[i].BLine
							 << "-L" << TL[i].ELine << ")\n";
		}

		// ── Phase 4: compute character ranges to remove ──────────

		llvm::StringRef Buf = sm_.getBufferData(sm_.getMainFileID());
		const unsigned S = static_cast<unsigned>(Buf.size());

		auto clampRange = [&](unsigned& L, unsigned& R) {
			L = std::min(L, S);
			R = std::min(R, S);
			if (R < L) std::swap(L, R);
		};

		std::vector<int> DeadDiff(S + 1, 0), AliveDiff(S + 1, 0);
		auto addRange = [&](std::vector<int>& Diff, unsigned L, unsigned R) {
			clampRange(L, R);
			if (L >= R) return;
			++Diff[L];
			--Diff[R];
		};

		for (unsigned i = 0; i < N; ++i)
			if (Alive[i])
				addRange(AliveDiff, TL[i].BOff, TL[i].EOff);
			else
				addRange(DeadDiff, TL[i].BOff, TL[i].EOff);

		// ── Phase 5: emit output, skip killed characters ─────────

		std::string Filtered;
		Filtered.reserve(Buf.size());

		int Dead = 0, AliveMark = 0;
		for (unsigned i = 0; i < S; ++i) {
			Dead += DeadDiff[i];
			AliveMark += AliveDiff[i];
			if (Dead > 0 && AliveMark == 0) continue;
			Filtered.push_back(Buf[i]);
		}

		llvm::StringRef Out(Filtered);
		llvm::SmallVector<llvm::StringRef, 0> Lines;
		Out.split(Lines, '\n');

		unsigned Blanks = 0;
		for (llvm::StringRef L : Lines) {
			if (!L.empty() && L.back() == '\r') L = L.drop_back();

			if (L.ltrim().empty()) {
				if (++Blanks <= 1) llvm::outs() << '\n';
				continue;
			}
			Blanks = 0;
			llvm::outs() << L << '\n';
		}
	}

private:
	clang::SourceManager& sm_;
	const clang::LangOptions& lo_;
};

} // namespace shaker
