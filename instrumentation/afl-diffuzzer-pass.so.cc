// File: instrumentation/afl-diffuzzer-pass.so.cc

#include "llvm/Pass.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

namespace {

class MyCustomPass : public PassInfoMixin<MyCustomPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
    errs() << "Running MyCustomPass\n";

    for (Function &F : M) {
      errs() << "Function: " << F.getName() << "\n";
    }

    return PreservedAnalyses::all();
  }
};

} 

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "MyCustomPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
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
