#include <iostream>
#include <llvm/Pass.h>
#include <llvm/Analysis/CallGraphSCCPass.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
using namespace llvm;

#include <fstream>
#include <unordered_set>
namespace {
    struct CallDump : public CallGraphSCCPass {
        static char ID;
        static std::unordered_set<Function*> finished;
        CallDump() : CallGraphSCCPass(ID) {}
        bool runOnSCC(CallGraphSCC &SCC) override {
            auto& g = SCC.getCallGraph();
            for(auto const &i : g) {
                const CallGraphNode& node = *i.second;
                Function* function = node.getFunction();
                if(finished.count(function)) continue;
                if(function)
                    std::cerr << function->getName().str() << ", ";
                else
                    std::cerr << "<null function>" << ", ";
                std::cerr << node.getNumReferences();
                if(auto m = node.size()) {
                    std::cerr << ", ";
                    for (auto &k : node) {
                        m--;
                        auto t = k.second->getFunction();
                        if (t) std::cerr << t->getName().str();
                        else
                            std::cerr << "<null function>";
                        if (m) std::cerr << ", ";
                    }
                }
                std::cerr << std::endl;
                finished.insert(function);
            }
            return false;
        }
    };

    char CallDump::ID = 0;
    std::unordered_set<Function*> CallDump::finished{};
    RegisterPass<CallDump> __DUMP_CALL__("dumpcalls", "call graph pass", false, false);

    RegisterStandardPasses __DUMP_CALL_PASS__{
        PassManagerBuilder::EP_EarlyAsPossible,
        [](const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
            PM.add(new CallDump());
        }
    };
}

