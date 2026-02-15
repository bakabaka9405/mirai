#pragma once

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Lex/PPCallbacks.h>
#include <clang/Lex/Preprocessor.h>
#include <llvm/Support/raw_ostream.h>

#include <map>
#include <string>
#include <vector>

namespace shaker {

	/// Collects #include directives from the main file during preprocessing.
	class IncludeCollector : public clang::PPCallbacks {
	public:
		struct Info {
			unsigned Line;
			std::string Text;
		};

		IncludeCollector(const clang::SourceManager& SM, std::vector<Info>& Out)
			: SM_(SM), Out_(Out) {}

		void InclusionDirective(clang::SourceLocation HashLoc,
								const clang::Token& IncludeTok,
								llvm::StringRef FileName,
								bool IsAngled,
								clang::CharSourceRange FilenameRange,
								clang::OptionalFileEntryRef File,
								llvm::StringRef SearchPath,
								llvm::StringRef RelativePath,
								const clang::Module* SuggestedModule,
								bool ModuleImported,
								clang::SrcMgr::CharacteristicKind FileType) override {
			if (!SM_.isWrittenInMainFile(HashLoc)) return;
			unsigned Line = SM_.getSpellingLineNumber(HashLoc);
			std::string Dir = "#include ";
			Dir += IsAngled ? "<" : "\"";
			Dir += FileName.str();
			Dir += IsAngled ? ">" : "\"";
			Out_.emplace_back(Line, std::move(Dir));
		}

	private:
		const clang::SourceManager& SM_;
		std::vector<Info>& Out_;
	};

	/// Preprocesses the source file, expands all macros, and outputs the
	/// result while preserving #include and #pragma directives as-is.
	class MacroExpandAction : public clang::PreprocessorFrontendAction {
	protected:
		void ExecuteAction() override {
			clang::CompilerInstance& CI = getCompilerInstance();
			clang::Preprocessor& PP = CI.getPreprocessor();
			clang::SourceManager& SM = CI.getSourceManager();

			// 1. Read original source to find #pragma lines
			clang::FileID MainFID = SM.getMainFileID();
			llvm::StringRef OrigSrc = SM.getBufferData(MainFID);

			std::map<unsigned, std::string> PragmaLines;
			{
				llvm::SmallVector<llvm::StringRef, 0> Lines;
				OrigSrc.split(Lines, '\n');
				for (unsigned i = 0; i < Lines.size(); ++i) {
					llvm::StringRef Trimmed = Lines[i].ltrim();
					if (Trimmed.starts_with("#pragma")) {
						llvm::StringRef L = Lines[i];
						if (!L.empty() && L.back() == '\r') L = L.drop_back();
						PragmaLines[i + 1] = L.str();
					}
				}
			}

			// 2. Install PPCallbacks to collect #include directives
			std::vector<IncludeCollector::Info> Includes;
			PP.addPPCallbacks(
				std::make_unique<IncludeCollector>(SM, Includes));

			// 3. Run the preprocessor and collect tokens from the main file
			PP.EnterMainSourceFile();

			struct TokRec {
				unsigned Line, Col;
				std::string Text;
			};
			std::vector<TokRec> Tokens;

			clang::Token Tok;
			while (true) {
				PP.Lex(Tok);
				if (Tok.is(clang::tok::eof)) break;

				clang::SourceLocation Exp = SM.getExpansionLoc(Tok.getLocation());
				if (!SM.isWrittenInMainFile(Exp)) continue;

				Tokens.push_back({ SM.getExpansionLineNumber(Exp),
								   SM.getExpansionColumnNumber(Exp),
								   PP.getSpelling(Tok) });
			}

			// 4. Build preserved-line map  (#include + #pragma)
			std::map<unsigned, std::string> Preserved;
			for (auto& Inc : Includes)
				Preserved[Inc.Line] = Inc.Text;
			for (auto& [L, T] : PragmaLines)
				Preserved[L] = T;

			// 5. Build output lines from expanded tokens
			std::map<unsigned, std::string> OutLines;
			std::map<unsigned, unsigned> OutCols;

			for (auto& T : Tokens) {
				if (Preserved.count(T.Line)) continue;

				auto& Line = OutLines[T.Line];
				auto& Col = OutCols[T.Line];

				if (Col == 0) {
					if (T.Col > 1) Line.append(T.Col - 1, ' ');
					Line += T.Text;
					Col = T.Col + static_cast<unsigned>(T.Text.size());
				}
				else if (T.Col > Col) {
					Line.append(T.Col - Col, ' ');
					Line += T.Text;
					Col = T.Col + static_cast<unsigned>(T.Text.size());
				}
				else {
					Line += ' ';
					Line += T.Text;
					Col += 1 + static_cast<unsigned>(T.Text.size());
				}
			}

			// 6. Determine the last meaningful line
			unsigned MaxLine = 0;
			if (!OutLines.empty()) MaxLine = OutLines.rbegin()->first;
			for (auto& [L, _] : Preserved)
				MaxLine = std::max(MaxLine, L);

			// 7. Emit — collapse runs of 2+ blank lines into 1
			unsigned Blanks = 0;
			for (unsigned L = 1; L <= MaxLine; ++L) {
				auto pIt = Preserved.find(L);
				auto tIt = OutLines.find(L);
				if (pIt != Preserved.end()) {
					Blanks = 0;
					llvm::outs() << pIt->second << '\n';
				}
				else if (tIt != OutLines.end()) {
					Blanks = 0;
					llvm::outs() << tIt->second << '\n';
				}
				else {
					if (++Blanks <= 1) llvm::outs() << '\n';
				}
			}
		}
	};

} // namespace shaker
