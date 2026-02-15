#pragma once

#include <clang/AST/AST.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <llvm/Support/raw_ostream.h>

namespace shaker {

	class TreeDumperVisitor : public clang::RecursiveASTVisitor<TreeDumperVisitor> {
	public:
		explicit TreeDumperVisitor(clang::ASTContext& ctx)
			: ctx_(ctx), sm_(ctx.getSourceManager()) {}

		bool TraverseDecl(clang::Decl* D) {
			if (!D) return true;

			if (sOnlyMainFile) {
				clang::SourceLocation L = sm_.getExpansionLoc(D->getLocation());
				if (L.isValid() && !sm_.isWrittenInMainFile(L)) return true;
			}

			printIndent();
			llvm::outs() << "Decl: " << D->getDeclKindName();

			if (auto* ND = clang::dyn_cast<clang::NamedDecl>(D)) {
				if (!ND->getName().empty())
					llvm::outs() << " '" << ND->getNameAsString() << "'";
			}

			printLoc(D->getLocation());
			llvm::outs() << "\n";

			indent_ += 2;
			RecursiveASTVisitor::TraverseDecl(D);
			indent_ -= 2;
			return true;
		}

		bool TraverseStmt(clang::Stmt* S) {
			if (!S) return true;

			printIndent();
			llvm::outs() << "Stmt: " << S->getStmtClassName();

			if (auto* E = clang::dyn_cast<clang::Expr>(S)) {
				llvm::outs() << " <type=" << E->getType().getAsString() << ">";
			}

			printLoc(S->getBeginLoc());
			llvm::outs() << "\n";

			indent_ += 2;
			RecursiveASTVisitor::TraverseStmt(S);
			indent_ -= 2;
			return true;
		}

	private:
		void printIndent() const {
			for (unsigned i = 0; i < indent_; ++i) llvm::outs() << ' ';
		}

		void printLoc(clang::SourceLocation Loc) {
			if (!Loc.isValid()) return;
			clang::SourceLocation L = sm_.getExpansionLoc(Loc);
			clang::PresumedLoc P = sm_.getPresumedLoc(L);
			if (P.isInvalid()) return;
			llvm::outs() << " @"
						 << P.getFilename() << ":" << P.getLine() << ":" << P.getColumn();
		}

	private:
		clang::ASTContext& ctx_;
		clang::SourceManager& sm_;
		unsigned indent_ = 0;
		static constexpr bool sOnlyMainFile = true;
	};

	class TreeDumperConsumer : public clang::ASTConsumer {
	public:
		explicit TreeDumperConsumer(clang::ASTContext& ctx) : visitor_(ctx) {}

		void HandleTranslationUnit(clang::ASTContext& ctx) override {
			visitor_.TraverseDecl(ctx.getTranslationUnitDecl());
		}

	private:
		TreeDumperVisitor visitor_;
	};

	class TreeDumperAction : public clang::ASTFrontendAction {
	public:
		std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& ci, llvm::StringRef InFile) override {
			return std::make_unique<TreeDumperConsumer>(ci.getASTContext());
		}
	};

} // namespace shaker
