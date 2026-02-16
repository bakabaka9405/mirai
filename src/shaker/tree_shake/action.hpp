#pragma once

#include "consumer.hpp"

#include <clang/Frontend/FrontendActions.h>

namespace shaker {

/// @brief Tree-shake 前端动作。
/// @details 为 clang 前端创建 `TreeShakeConsumer`，并在翻译单元解析完成后执行裁剪逻辑。
class TreeShakeAction : public clang::ASTFrontendAction {
public:
	/// @brief 创建 AST 消费器。
	/// @param CI 当前编译器实例。
	/// @param InputFile 输入文件信息（此处未使用）。
	/// @return 用于执行 tree-shake 的 AST 消费器实例。
	std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
		clang::CompilerInstance& CI, llvm::StringRef InputFile) override {
		(void)InputFile;
		return std::make_unique<TreeShakeConsumer>(CI);
	}
};

} // namespace shaker
