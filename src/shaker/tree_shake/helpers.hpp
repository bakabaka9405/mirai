#pragma once

#include <clang/AST/AST.h>

namespace shaker {

/// @brief 向上追溯到在词法上包含目标声明的翻译单元子声明。
/// @param D 目标声明。
/// @return 成功返回对应的顶层声明，失败返回 `nullptr`。
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

/// @brief 判断全局变量是否具有非平凡构造或带副作用的初始化。
/// @param VD 全局变量声明。
/// @param Ctx AST 上下文。
/// @return `true` 表示该变量初始化具有潜在副作用，应作为根保留。
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
