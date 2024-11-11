// File: instrumentation/afl-diffuzzer-pass.so.cc

#include "llvm/Pass.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "debug.h"

using namespace llvm;

namespace {

class DiffuzzerPass : public PassInfoMixin<DiffuzzerPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
    SAYF(cCYA "afl-diffuzzer-pass" VERSION cRST
                " WIEHOEEE\n");
    errs() << "Running Diffuzzer pass\n";

    for (Function &F : M) {
      errs() << "Function: " << F.getName() << "\n";
    }

    return PreservedAnalyses::all();
  }
};

} 

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "diffuzzer-pass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                  MPM.addPass(DiffuzzerPass());
                });
          }};
}
