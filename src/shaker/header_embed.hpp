#pragma once

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Lex/PPCallbacks.h>
#include <clang/Lex/Preprocessor.h>
#include <llvm/Support/raw_ostream.h>

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace shaker {

	/// Embeds user headers into a single translation unit, keeping
	/// system headers as plain #include directives.
	///
	/// Usage:
	///   shaker --embed-headers [--embed-prefix=mirai/] source.cpp --
	///
	/// When --embed-prefix is specified, only user headers whose include
	/// name starts with that prefix are inlined (e.g. "mirai/").
	/// Without it, ALL user headers (SrcMgr::C_User) are inlined.
	class HeaderEmbedAction : public clang::PreprocessorFrontendAction {
	public:
		/// If non-empty, only inline user headers whose include name
		/// starts with this prefix.
		inline static std::string Prefix;

	protected:
		void ExecuteAction() override {
			auto& CI = getCompilerInstance();
			auto& PP = CI.getPreprocessor();
			auto& SM = CI.getSourceManager();

			// ---- data filled by PPCallbacks ----
			struct IncRecord {
				std::string FromPath;      // file containing the #include
				std::string IncludedPath;  // resolved path of included file
				unsigned Line;             // line number in FromPath
				bool Embed;                // true → this header should be inlined
			};
			std::vector<IncRecord> Records;
			std::map<std::string, clang::FileID> Path2FID;

			// Pre-register main file
			clang::FileID MainFID = SM.getMainFileID();
			std::string MainPath;
			if (auto FE = SM.getFileEntryRefForID(MainFID)) {
				MainPath = FE->getName().str();
				Path2FID[MainPath] = MainFID;
			}

			// ---- PPCallbacks implementation ----
			class Collector : public clang::PPCallbacks {
			public:
				Collector(clang::SourceManager& SM,
						  std::vector<IncRecord>& Recs,
						  std::map<std::string, clang::FileID>& P2F,
						  llvm::StringRef Pfx)
					: SM_(SM), Recs_(Recs), P2F_(P2F), Pfx_(Pfx) {}

				// Track FileID for every entered file.
				void FileChanged(clang::SourceLocation Loc,
								 FileChangeReason Reason,
								 clang::SrcMgr::CharacteristicKind,
								 clang::FileID) override {
					if (Reason != EnterFile) return;
					auto FID = SM_.getFileID(Loc);
					if (auto FE = SM_.getFileEntryRefForID(FID))
						P2F_.try_emplace(FE->getName().str(), FID);
				}

				void InclusionDirective(
						clang::SourceLocation HashLoc,
						const clang::Token&,
						llvm::StringRef FileName,
						bool IsAngled,
						clang::CharSourceRange,
						clang::OptionalFileEntryRef File,
						llvm::StringRef /*SearchPath*/,
						llvm::StringRef /*RelativePath*/,
						const clang::Module*,
						bool /*ModuleImported*/,
						clang::SrcMgr::CharacteristicKind FileType) override {
					auto Exp = SM_.getExpansionLoc(HashLoc);
					auto FromFID = SM_.getFileID(Exp);
					std::string From;
					if (auto FE = SM_.getFileEntryRefForID(FromFID))
						From = FE->getName().str();

					std::string IncPath;
					if (File) IncPath = File->getName().str();

					// Decide whether to inline:
					//   1) Clang says it is a user header (C_User)
					//   2) If a prefix filter is set, the include name must match
					bool Embed = (FileType == clang::SrcMgr::C_User);
					if (Embed && !Pfx_.empty())
						Embed = FileName.starts_with(Pfx_);

					Recs_.push_back({std::move(From), std::move(IncPath),
									 SM_.getSpellingLineNumber(HashLoc), Embed});
				}

			private:
				clang::SourceManager& SM_;
				std::vector<IncRecord>& Recs_;
				std::map<std::string, clang::FileID>& P2F_;
				llvm::StringRef Pfx_;
			};

			PP.addPPCallbacks(
				std::make_unique<Collector>(SM, Records, Path2FID, Prefix));

			// Run the preprocessor — triggers all callbacks.
			PP.EnterMainSourceFile();
			{
				clang::Token T;
				do { PP.Lex(T); } while (!T.is(clang::tok::eof));
			}

			// ---- Build user-file set ----
			std::set<std::string> UserFiles{MainPath};
			for (auto& R : Records)
				if (R.Embed && !R.IncludedPath.empty())
					UserFiles.insert(R.IncludedPath);

			// ---- Build dependency graph & skip-line map ----
			std::map<std::string, std::vector<std::string>> Deps;
			std::map<std::string, std::set<unsigned>> SkipLines;

			for (auto& F : UserFiles) Deps[F]; // ensure entries exist

			for (auto& R : Records) {
				if (!R.Embed || R.IncludedPath.empty()) continue;
				if (!UserFiles.count(R.FromPath)) continue;
				Deps[R.FromPath].push_back(R.IncludedPath);
				SkipLines[R.FromPath].insert(R.Line);
			}

			// ---- Topological sort (Kahn's algorithm) ----
			std::map<std::string, int> InDeg;
			for (auto& [F, _] : Deps) InDeg[F] = 0;
			for (auto& [_, Ds] : Deps)
				for (auto& D : Ds) InDeg[D]++;

			std::vector<std::string> Queue;
			for (auto& [F, D] : InDeg)
				if (D == 0) Queue.push_back(F);

			std::vector<std::string> Order;
			for (size_t i = 0; i < Queue.size(); ++i) {
				Order.push_back(Queue[i]);
				for (auto& D : Deps[Queue[i]])
					if (--InDeg[D] == 0) Queue.push_back(D);
			}

			// Reverse so that dependencies come first, main file last.
			std::reverse(Order.begin(), Order.end());

			// ---- Emit merged output ----
			std::set<std::string> SeenSysInc; // deduplicate system #includes

			for (auto& FP : Order) {
				auto it = Path2FID.find(FP);
				if (it == Path2FID.end()) continue;

				llvm::StringRef Buf = SM.getBufferData(it->second);
				auto& SL = SkipLines[FP];

				// Section separator comment
				llvm::outs() << "// --- " << FP << " ---\n";

				llvm::SmallVector<llvm::StringRef, 0> Lines;
				Buf.split(Lines, '\n');

				unsigned Blanks = 0;
				for (unsigned i = 0; i < Lines.size(); ++i) {
					llvm::StringRef L = Lines[i];
					if (!L.empty() && L.back() == '\r')
						L = L.drop_back();

					llvm::StringRef T = L.ltrim();

					// Strip #pragma once
					if (T.starts_with("#pragma once")) continue;

					// Skip user #include lines (content is inlined elsewhere)
					if (SL.count(i + 1)) continue;

					// Deduplicate system #include directives across files
					if (T.starts_with("#include")) {
						std::string Key = T.str();
						if (!SeenSysInc.insert(Key).second) continue;
					}

					// Collapse consecutive blank lines into at most one
					if (T.empty()) {
						if (++Blanks <= 1) llvm::outs() << '\n';
						continue;
					}
					Blanks = 0;
					llvm::outs() << L << '\n';
				}
				llvm::outs() << '\n';
			}
		}
	};

} // namespace shaker
