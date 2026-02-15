#pragma once

#include <clang/AST/AST.h>

namespace shaker {

/// Walk up DeclContext to the TU-child declaration that lexically
/// contains @p D.  Returns nullptr on failure.
static const clang::Decl* topLevelOf(const clang::Decl* D) {
	while (D) {
		const auto* DC = D->getDeclContext();
		if (!DC || llvm::isa<clang::TranslationUnitDecl>(DC)
			|| llvm::isa<clang::LinkageSpecDecl>(DC))
			return D;
		D = llvm::dyn_cast<clang::Decl>(DC);
	}
	return nullptr;
}

/// Does this global VarDecl have a non-trivial constructor or
/// side-effecting initialiser?
static bool hasSideEffectInit(const clang::VarDecl* VD,
							  const clang::ASTContext& Ctx) {
	if (!VD->hasGlobalStorage()) return false;
	if (const auto* Init = VD->getInit()) {
		if (const auto* CE = llvm::dyn_cast<clang::CXXConstructExpr>(Init))
			if (!CE->getConstructor()->isTrivial())
				return true;
		if (Init->HasSideEffects(Ctx))
			return true;
	}
	return false;
}

} // namespace shaker
