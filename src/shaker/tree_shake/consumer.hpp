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
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <vector>

namespace shaker {

/// @brief Tree-shake 的 AST 消费器。
/// @details 负责收集声明单元、构建引用图、计算可达集合并输出裁剪后的代码。
class TreeShakeConsumer : public clang::ASTConsumer {
public:
	/// @brief 构造消费器。
	/// @param CI 编译器实例，用于提取 SourceManager 与语言选项。
	explicit TreeShakeConsumer(clang::CompilerInstance& CI)
		: sm_(CI.getSourceManager()), lo_(CI.getLangOpts()) {}

	/// @brief 处理翻译单元入口。
	/// @param Ctx 当前翻译单元对应的 AST 上下文。
	void HandleTranslationUnit(clang::ASTContext& Ctx) override {
		auto Collection = collectDeclarationUnits(Ctx);
		if (Collection.Units.empty()) {
			llvm::outs() << sm_.getBufferData(sm_.getMainFileID());
			return;
		}

		auto Adj = buildReferenceGraph(Collection);
		auto Alive = computeAlive(Collection.Units, Adj);
		emitRemovalDiagnostics(Collection.Units, Alive);
		emitFilteredOutput(Collection.Units, Alive);
	}

private:
	/// @brief 单个可裁剪声明单元的信息。
	struct Info {
		/// 声明节点指针。
		const clang::Decl* D;
		/// 声明在主文件中的起止偏移（半开区间）。
		unsigned BOff, EOff;
		/// 声明在主文件中的起止行号。
		unsigned BLine, ELine;
		/// 声明分类。
		DeclCat Cat;
		/// 是否属于根节点（必须保留）。
		bool Root;
	};

	/// @brief 声明收集阶段的结果。
	struct CollectionResult {
		/// 所有声明单元。
		std::vector<Info> Units;
		/// 声明（规范化后）到单元下标的映射。
		std::map<const clang::Decl*, unsigned> Index;
		/// 同一规范声明对应的多单元分组。
		std::map<const clang::Decl*, std::vector<unsigned>> CanonGroup;
	};

	/// @brief 判断声明是否应跳过。
	/// @param D 待判断声明。
	/// @return 为 `true` 表示该声明不参与裁剪分析。
	bool shouldSkipDecl(const clang::Decl* D) const {
		if (!D || D->isImplicit()) return true;
		if (auto* CTSD = llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(D))
			return !CTSD->isExplicitSpecialization();
		auto Loc = sm_.getExpansionLoc(D->getLocation());
		return !Loc.isValid() || !sm_.isWrittenInMainFile(Loc);
	}

	/// @brief 将声明末尾扩展到分号后。
	/// @param MainBuf 主文件完整文本。
	/// @param Off 当前结束偏移。
	/// @return 若成功定位同层级分号，返回扩展后的偏移；否则返回原偏移。
	unsigned extendToDeclSemicolon(llvm::StringRef MainBuf, unsigned Off) const {
		const auto MainSize = static_cast<unsigned>(MainBuf.size());
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
	}

	/// @brief 将声明前导属性纳入起始范围。
	/// @param Target 目标声明。
	/// @param MainBuf 主文件完整文本。
	/// @param BeginOff 声明起始偏移（会被就地收缩）。
	void includeLeadingAttrs(const clang::Decl* Target, llvm::StringRef MainBuf, unsigned& BeginOff) const {
		if (!Target) return;
		for (const auto* Attr : Target->attrs()) {
			auto AttrBegin = sm_.getExpansionLoc(Attr->getRange().getBegin());
			if (!AttrBegin.isValid() || !sm_.isWrittenInMainFile(AttrBegin))
				continue;

			auto AttrOff = sm_.getFileOffset(AttrBegin);
			auto Scan = AttrOff;
			while (Scan > 0 && (MainBuf[Scan - 1] == ' ' || MainBuf[Scan - 1] == '\t'))
				--Scan;
			if (Scan >= 2 && MainBuf[Scan - 1] == '[' && MainBuf[Scan - 2] == '[')
				AttrOff = Scan - 2;
			BeginOff = std::min(BeginOff, AttrOff);
		}
	}

	/// @brief 判断声明是否拥有函数体。
	/// @param D 待判断声明。
	/// @return `true` 表示存在函数体定义。
	bool hasFunctionBody(const clang::Decl* D) const {
		if (auto* FD = llvm::dyn_cast<clang::FunctionDecl>(D))
			return FD->doesThisDeclarationHaveABody();
		if (auto* FT = llvm::dyn_cast<clang::FunctionTemplateDecl>(D))
			return FT->getTemplatedDecl()->doesThisDeclarationHaveABody();
		return false;
	}

	/// @brief 判断声明是否为可达性根节点。
	/// @param D 待判断声明。
	/// @param Ctx AST 上下文。
	/// @return `true` 表示该声明应无条件保留。
	bool isRootDecl(const clang::Decl* D, clang::ASTContext& Ctx) const {
		if (auto* FD = llvm::dyn_cast<clang::FunctionDecl>(D))
			if (FD->isMain()) return true;
		if (auto* FT = llvm::dyn_cast<clang::FunctionTemplateDecl>(D))
			if (FT->getTemplatedDecl()->isMain()) return true;

		if (ShakeStrength <= 1)
			if (auto* VD = llvm::dyn_cast<clang::VarDecl>(D))
				if (hasSideEffectInit(VD, Ctx)) return true;

		return false;
	}

	/// @brief 注册声明到索引表。
	/// @param D 声明指针。
	/// @param i 单元下标。
	/// @param Index 目标索引表。
	void registerDecl(const clang::Decl* D,
					  unsigned i,
					  std::map<const clang::Decl*, unsigned>& Index) const {
		if (D) Index[D->getCanonicalDecl()] = i;
	}

	/// @brief 注册声明及其常见别名形式到索引表。
	/// @param D 声明指针。
	/// @param i 单元下标。
	/// @param Index 目标索引表。
	void registerDeclAliases(const clang::Decl* D,
							 unsigned i,
							 std::map<const clang::Decl*, unsigned>& Index) const {
		registerDecl(D, i, Index);
		if (auto* CT = llvm::dyn_cast<clang::ClassTemplateDecl>(D))
			registerDecl(CT->getTemplatedDecl(), i, Index);
		if (auto* FT = llvm::dyn_cast<clang::FunctionTemplateDecl>(D))
			registerDecl(FT->getTemplatedDecl(), i, Index);
		if (auto* TA = llvm::dyn_cast<clang::TypeAliasTemplateDecl>(D))
			registerDecl(TA->getTemplatedDecl(), i, Index);
		if (auto* UD = llvm::dyn_cast<clang::UsingDecl>(D))
			for (auto* Shadow : UD->shadows())
				registerDecl(Shadow, i, Index);
		if (auto* ED = llvm::dyn_cast<clang::EnumDecl>(D))
			for (auto* EC : ED->enumerators())
				registerDecl(EC, i, Index);
	}

	/// @brief 将声明转换为可裁剪单元信息。
	/// @param D 待转换声明。
	/// @param Ctx AST 上下文。
	/// @param MainBuf 主文件完整文本。
	/// @return 成功时返回单元信息，失败返回空。
	std::optional<Info> makeInfo(const clang::Decl* D, clang::ASTContext& Ctx, llvm::StringRef MainBuf) const {
		if (shouldSkipDecl(D)) return std::nullopt;

		llvm::errs() << "tree-shake: consider " << D->getDeclKindName() << " at "
					 << D->getLocation().printToString(sm_) << "\n";

		auto BLoc = sm_.getExpansionLoc(D->getBeginLoc());
		auto ELoc = sm_.getExpansionLoc(D->getEndLoc());
		if (!BLoc.isValid() || !ELoc.isValid()) return std::nullopt;

		unsigned BLine = sm_.getExpansionLineNumber(BLoc);
		unsigned ELine = sm_.getExpansionLineNumber(ELoc);
		unsigned BOff = sm_.getFileOffset(BLoc);
		unsigned EOff = 0;

		includeLeadingAttrs(D, MainBuf, BOff);
		if (auto* FT = llvm::dyn_cast<clang::FunctionTemplateDecl>(D))
			includeLeadingAttrs(FT->getTemplatedDecl(), MainBuf, BOff);
		if (auto* CT = llvm::dyn_cast<clang::ClassTemplateDecl>(D))
			includeLeadingAttrs(CT->getTemplatedDecl(), MainBuf, BOff);
		if (auto* TA = llvm::dyn_cast<clang::TypeAliasTemplateDecl>(D))
			includeLeadingAttrs(TA->getTemplatedDecl(), MainBuf, BOff);

		if (auto After = clang::Lexer::findLocationAfterToken(ELoc, clang::tok::semi, sm_, lo_, false);
			After.isValid()) {
			auto AfterLoc = sm_.getExpansionLoc(After);
			unsigned SemiLine = sm_.getExpansionLineNumber(AfterLoc);
			if (SemiLine <= ELine + 1) ELine = SemiLine;
			EOff = sm_.getFileOffset(AfterLoc);
		}
		else {
			auto EndTok = clang::Lexer::getLocForEndOfToken(ELoc, 0, sm_, lo_);
			if (EndTok.isValid()) {
				EndTok = sm_.getExpansionLoc(EndTok);
				EOff = sm_.getFileOffset(EndTok);
			}
			else {
				EOff = sm_.getFileOffset(ELoc) + 1;
			}
		}

		if (EOff < BOff) std::swap(BOff, EOff);
		if (!hasFunctionBody(D)) {
			auto Ext = extendToDeclSemicolon(MainBuf, EOff);
			if (Ext > EOff) EOff = Ext;
		}

		return Info{
			.D = D,
			.BOff = BOff,
			.EOff = EOff,
			.BLine = BLine,
			.ELine = ELine,
			.Cat = classify(D),
			.Root = isRootDecl(D, Ctx),
		};
	}

	/// @brief 收集单个声明并写入结果集。
	/// @param D 待收集声明。
	/// @param Ctx AST 上下文。
	/// @param MainBuf 主文件完整文本。
	/// @param Out 输出结果。
	void collectDecl(const clang::Decl* D,
					 clang::ASTContext& Ctx,
					 llvm::StringRef MainBuf,
					 CollectionResult& Out) const {
		auto Unit = makeInfo(D, Ctx, MainBuf);
		if (!Unit.has_value()) return;

		auto i = static_cast<unsigned>(Out.Units.size());
		registerDeclAliases(D, i, Out.Index);
		if (D) Out.CanonGroup[D->getCanonicalDecl()].push_back(i);
		Out.Units.push_back(*Unit);
	}

	/// @brief 判断声明是否属于成员级声明单元。
	/// @param D 待判断声明。
	/// @return `true` 表示应作为类成员单元纳入分析。
	static bool isMemberUnitDecl(const clang::Decl* D) {
		if (auto* FD = llvm::dyn_cast<clang::FunctionDecl>(D))
			return llvm::isa<clang::RecordDecl>(FD->getDeclContext());
		if (auto* FT = llvm::dyn_cast<clang::FunctionTemplateDecl>(D))
			return llvm::isa<clang::CXXMethodDecl>(FT->getTemplatedDecl());
		if (llvm::isa<clang::FieldDecl>(D))
			return true;
		if (auto* VD = llvm::dyn_cast<clang::VarDecl>(D))
			return llvm::isa<clang::RecordDecl>(VD->getDeclContext());
		return false;
	}

	/// @brief 收集翻译单元中的声明单元。
	/// @param Ctx AST 上下文。
	/// @return 包含单元、索引与规范声明分组的收集结果。
	CollectionResult collectDeclarationUnits(clang::ASTContext& Ctx) const {
		CollectionResult Result;
		llvm::StringRef MainBuf = sm_.getBufferData(sm_.getMainFileID());

		auto collectTopLevel = [&](this auto&& self, clang::DeclContext* DC) -> void {
			for (auto* D : DC->decls()) {
				if (auto* LS = llvm::dyn_cast<clang::LinkageSpecDecl>(D)) {
					self(LS);
					continue;
				}
				if (auto* NS = llvm::dyn_cast<clang::NamespaceDecl>(D)) {
					self(NS);
					continue;
				}
				collectDecl(D, Ctx, MainBuf, Result);
			}
		};
		collectTopLevel(Ctx.getTranslationUnitDecl());

		auto collectMembers = [&](this auto&& self, const clang::DeclContext* DC) -> void {
			for (auto* D : DC->decls()) {
				if (!D || D->isImplicit() || llvm::isa<clang::AccessSpecDecl>(D))
					continue;

				if (isMemberUnitDecl(D))
					collectDecl(D, Ctx, MainBuf, Result);

				if (auto* RD = llvm::dyn_cast<clang::RecordDecl>(D))
					self(RD);
				if (auto* CT = llvm::dyn_cast<clang::ClassTemplateDecl>(D))
					self(CT->getTemplatedDecl());
			}
		};

		const auto TopN = static_cast<unsigned>(Result.Units.size());
		for (unsigned i = 0; i < TopN; ++i) {
			if (auto* RD = llvm::dyn_cast<clang::RecordDecl>(Result.Units[i].D))
				collectMembers(RD);
			if (auto* CT = llvm::dyn_cast<clang::ClassTemplateDecl>(Result.Units[i].D))
				collectMembers(CT->getTemplatedDecl());
		}

		return Result;
	}

	/// @brief 基于声明引用关系构建有向图。
	/// @param Collection 声明收集结果。
	/// @return 邻接表形式的引用图。
	std::vector<std::set<unsigned>> buildReferenceGraph(const CollectionResult& Collection) const {
		const auto N = static_cast<unsigned>(Collection.Units.size());
		std::vector<std::set<unsigned>> Adj(N);

		for (unsigned i = 0; i < N; ++i) {
			RefCollector RC;
			RC.TraverseDecl(const_cast<clang::Decl*>(Collection.Units[i].D));

			for (const clang::Decl* Ref : RC.Found) {
				auto It = Collection.Index.find(Ref);
				if (It == Collection.Index.end()) {
					if (auto* P = topLevelOf(Ref))
						It = Collection.Index.find(P->getCanonicalDecl());
				}
				if (It != Collection.Index.end() && It->second != i)
					Adj[i].insert(It->second);
			}
		}

		for (const auto& [Canon, Group] : Collection.CanonGroup) {
			(void)Canon;
			if (Group.size() <= 1) continue;
			for (unsigned a = 0; a < Group.size(); ++a)
				for (unsigned b = 0; b < Group.size(); ++b)
					if (a != b)
						Adj[Group[a]].insert(Group[b]);
		}

		return Adj;
	}

	/// @brief 计算声明单元可达性。
	/// @param Units 声明单元列表。
	/// @param Adj 引用图邻接表。
	/// @return 与 `Units` 等长的可达标记数组。
	std::vector<bool> computeAlive(
		const std::vector<Info>& Units,
		const std::vector<std::set<unsigned>>& Adj) const {
		const auto N = static_cast<unsigned>(Units.size());
		std::vector<bool> Alive(N, false);
		std::queue<unsigned> Q;

		for (unsigned i = 0; i < N; ++i) {
			if (Units[i].Root || !canShake(Units[i].Cat, ShakeStrength)) {
				Alive[i] = true;
				Q.push(i);
			}
		}

		while (!Q.empty()) {
			auto u = Q.front();
			Q.pop();
			for (auto v : Adj[u]) {
				if (Alive[v]) continue;
				Alive[v] = true;
				Q.push(v);
			}
		}

		return Alive;
	}

	/// @brief 输出被移除声明的诊断信息。
	/// @param Units 声明单元列表。
	/// @param Alive 可达标记数组。
	void emitRemovalDiagnostics(const std::vector<Info>& Units, const std::vector<bool>& Alive) const {
		const auto N = static_cast<unsigned>(Units.size());
		for (unsigned i = 0; i < N; ++i) {
			if (Alive[i]) continue;

			llvm::StringRef Name = "<anon>";
			if (auto* ND = llvm::dyn_cast<clang::NamedDecl>(Units[i].D))
				if (!ND->getName().empty()) Name = ND->getName();

			llvm::errs() << "tree-shake: remove " << catName(Units[i].Cat)
						 << " '" << Name << "' (L" << Units[i].BLine
						 << "-L" << Units[i].ELine << ")\n";
		}
	}

	/// @brief 输出过滤后的源码文本。
	/// @param Units 声明单元列表。
	/// @param Alive 可达标记数组。
	void emitFilteredOutput(const std::vector<Info>& Units, const std::vector<bool>& Alive) const {
		llvm::StringRef Buf = sm_.getBufferData(sm_.getMainFileID());
		const unsigned Size = static_cast<unsigned>(Buf.size());

		auto clampRange = [&](unsigned& L, unsigned& R) {
			L = std::min(L, Size);
			R = std::min(R, Size);
			if (R < L) std::swap(L, R);
		};

		auto addRange = [&](std::vector<int>& Diff, unsigned L, unsigned R) {
			clampRange(L, R);
			if (L >= R) return;
			++Diff[L];
			--Diff[R];
		};

		std::vector<int> DeadDiff(Size + 1, 0), AliveDiff(Size + 1, 0);
		for (unsigned i = 0; i < Units.size(); ++i) {
			if (Alive[i])
				addRange(AliveDiff, Units[i].BOff, Units[i].EOff);
			else
				addRange(DeadDiff, Units[i].BOff, Units[i].EOff);
		}

		std::string Filtered;
		Filtered.reserve(Buf.size());

		int Dead = 0, AliveMark = 0;
		for (unsigned i = 0; i < Size; ++i) {
			Dead += DeadDiff[i];
			AliveMark += AliveDiff[i];
			if (Dead > 0 && AliveMark == 0) continue;
			Filtered.push_back(Buf[i]);
		}

		llvm::StringRef Out(Filtered);
		llvm::SmallVector<llvm::StringRef, 0> Lines;
		Out.split(Lines, '\n');

		unsigned Blanks = 0;
		for (llvm::StringRef Line : Lines) {
			if (!Line.empty() && Line.back() == '\r') Line = Line.drop_back();
			if (Line.ltrim().empty()) {
				if (++Blanks <= 1) llvm::outs() << '\n';
				continue;
			}
			Blanks = 0;
			llvm::outs() << Line << '\n';
		}
	}

	/// 主文件 SourceManager。
	clang::SourceManager& sm_;
	/// 语言选项。
	const clang::LangOptions& lo_;
};

} // namespace shaker
