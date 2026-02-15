#pragma once

#include "consumer.hpp"

#include <clang/Frontend/FrontendActions.h>

namespace shaker {

class TreeShakeAction : public clang::ASTFrontendAction {
public:
	std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
		clang::CompilerInstance& CI, llvm::StringRef) override {
		return std::make_unique<TreeShakeConsumer>(CI);
	}
};

} // namespace shaker
