#include "tree_dumper.hpp"
#include "tree_shake.hpp"
#include "macro_expand.hpp"
#include "header_embed.hpp"

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>

using namespace clang;
using namespace clang::tooling;

static llvm::cl::OptionCategory Cat("shaker options");

static llvm::cl::opt<bool> ExpandMacros(
	"expand-macros",
	llvm::cl::desc("Expand macros in source (keeping #include / #pragma)"),
	llvm::cl::cat(Cat));

static llvm::cl::opt<bool> EmbedHeaders(
	"embed-headers",
	llvm::cl::desc("Embed user headers into a single file"),
	llvm::cl::cat(Cat));

static llvm::cl::opt<std::string> EmbedPrefix(
	"embed-prefix",
	llvm::cl::desc("Only embed user headers matching this include-name prefix (e.g. mirai/)"),
	llvm::cl::init(""),
	llvm::cl::cat(Cat));

static llvm::cl::opt<bool> TreeShake(
	"tree-shake",
	llvm::cl::desc("Remove unreferenced functions, classes, and global variables"),
	llvm::cl::cat(Cat));

static llvm::cl::opt<unsigned> ShakeStrength(
	"shake-strength",
	llvm::cl::desc("Tree-shake aggressiveness (0=functions, 1=+vars, 2=+classes, 3=+typedefs/enums)"),
	llvm::cl::init(2),
	llvm::cl::cat(Cat));

auto a = 0; // atest

int main(int argc, const char** argv) {
	auto ExpParser = CommonOptionsParser::create(argc, argv, Cat);
	if (!ExpParser) {
		llvm::errs() << llvm::toString(ExpParser.takeError()) << "\n";
		return 1;
	}

	CommonOptionsParser& OptionsParser = ExpParser.get();
	ClangTool Tool(OptionsParser.getCompilations(),
				   OptionsParser.getSourcePathList());

	// 硬编码额外的编译参数
	Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(
		{
			"-std=c++23",
			"-Wno-everything",
			"-stdlib=libc++",
			"-isystem",
			"C:/Tools/msys64/clang64/include/c++/v1",
			"-isystem",
			"C:/Tools/msys64/clang64/lib/clang/21/include",
			"-isystem",
			"C:/Tools/msys64/clang64/include",
			"-IE:/Working/proj/mirai/include",
		},
		ArgumentInsertPosition::BEGIN));
	if (ExpandMacros)
		return Tool.run(newFrontendActionFactory<shaker::MacroExpandAction>().get());
	if (EmbedHeaders) {
		shaker::HeaderEmbedAction::Prefix = EmbedPrefix;
		return Tool.run(newFrontendActionFactory<shaker::HeaderEmbedAction>().get());
	}
	if (TreeShake) {
		shaker::ShakeStrength = ShakeStrength;
		return Tool.run(newFrontendActionFactory<shaker::TreeShakeAction>().get());
	}
	return Tool.run(newFrontendActionFactory<shaker::TreeDumperAction>().get());
}
