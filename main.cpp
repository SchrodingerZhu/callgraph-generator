#include <iostream>
#include <llvm/Pass.h>
#include <llvm/Analysis/CallGraphSCCPass.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <absl/container/flat_hash_set.h>
#include <sstream>

using namespace llvm;
namespace {
    struct CallDump : public CallGraphSCCPass {
        static char ID;
        static absl::flat_hash_set<Function*> finished;
        static bool visited;
        CallDump() : CallGraphSCCPass(ID) {}
        bool runOnSCC(CallGraphSCC &SCC) override {
            if (visited) { return false; }
            auto& g = SCC.getCallGraph();
            std::vector<std::pair<const CallGraphNode *, Function *>> functions{};
            for (auto &i : g) {
                const CallGraphNode& node = *i.second;
                if (!node.getFunction() && !finished.contains(node.getFunction()))
                    functions.emplace_back(&node, node.getFunction());
            }
#pragma omp parallel for default(shared)
            for(size_t i = 0; i < functions.size(); i++) {
                auto * function = functions[i].second;
                auto & node = *functions[i].first;
                std::stringstream ss;
                if(function)
                    ss << function->getName().str() << ", ";
                else
                    ss << "<null function>" << ", ";
                ss << node.getNumReferences();
                if(auto m = node.size()) {
                    ss << ", ";
                    for (auto &k : node) {
                        m--;
                        auto t = k.second->getFunction();
                        if (t) ss << t->getName().str();
                        else
                            ss << "<null function>";
                        if (m) ss << ", ";
                    }
                }
                ss << std::endl;
#pragma omp critical
                {
                    std::cerr << ss.str();
                    std::cerr.flush();
                    finished.insert(function);
                }
            }
            return false;
        }
    };

    char CallDump::ID = 0;
    bool CallDump::visited = false;
    absl::flat_hash_set<Function*> CallDump::finished{};
    RegisterPass<CallDump> __DUMP_CALL__("dumpcalls", "call graph pass", false, false);

    RegisterStandardPasses __DUMP_CALL_PASS__{
        PassManagerBuilder::EP_EarlyAsPossible,
        [](const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
            PM.add(new CallDump());
        }
    };
}

