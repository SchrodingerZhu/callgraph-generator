#include <iostream>
#include <llvm/Pass.h>
#include <llvm/Analysis/CallGraphSCCPass.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <fstream>
#include <llvm/Support/raw_os_ostream.h>

using namespace llvm;
namespace {
    struct CallDump : public CallGraphSCCPass {
        static char ID;
        static std::ofstream _out;
        CallDump() : CallGraphSCCPass(ID) {}

        auto runOnSCC(CallGraphSCC &SCC) -> bool override {
          auto &g = SCC.getCallGraph();
          raw_os_ostream out(_out);
          g.print(out);
          return false;
        }
    };

    char CallDump::ID = 0;
    std::ofstream CallDump::_out { std::getenv("CALLGRAPH_STORE")};
    RegisterPass<CallDump> __DUMP_CALL__("dumpcalls", "call graph pass", false, false);

    RegisterStandardPasses __DUMP_CALL_PASS__{
        PassManagerBuilder::EP_EarlyAsPossible,
        [](const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
            PM.add(new CallDump());
        }
    };
}

