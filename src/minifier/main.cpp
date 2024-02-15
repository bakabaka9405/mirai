#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
    struct RemoveUnusedFunctions : public ModulePass {
        static char ID;
        RemoveUnusedFunctions() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            std::vector<Function*> UnusedFunctions;
            for (auto &F : M) {
                if (F.user_empty()) { // Check if the function is unused
                    UnusedFunctions.push_back(&F);
                }
            }

            for (Function *F : UnusedFunctions) {
                F->eraseFromParent(); // Remove the unused function
            }

            return !UnusedFunctions.empty();
        }
    };
}

char RemoveUnusedFunctions::ID = 0;
static RegisterPass<RemoveUnusedFunctions> X("remove-unused-functions", "Remove Unused Functions Pass");
