// File: instrumentation/afl-diffuzzer-pass.so.cc

#include "llvm/Pass.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"  // Include PassBuilder.h

using namespace llvm;

namespace {

class MyCustomPass : public PassInfoMixin<MyCustomPass> {
public:
  // Entry point for the pass.
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
    errs() << "Running MyCustomPass\n";

    // Your custom logic here
    // Iterate over functions in the module
    for (Function &F : M) {
      errs() << "Function: " << F.getName() << "\n";
      // Add your analysis or transformation code here
    }

    // Return PreservedAnalyses::all() if the module was not modified
    // Return PreservedAnalyses::none() if the module was modified
    return PreservedAnalyses::all();
  }
};

} // namespace

// Register the pass with the PassPluginLibraryInfo
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "MyCustomPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            // Register the Module pass with the PassBuilder
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "my-custom-pass") {
                    MPM.addPass(MyCustomPass());
                    return true;
                  }
                  return false;
                });
          }};
}
