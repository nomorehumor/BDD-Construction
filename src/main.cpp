
extern "C" {
    #include "sddapi.h"
    // #include "sdd.h"
}
#include "sdd_formulas/sdd_rulesets.h"
#include "bdd_formulas/bdd_rulesets.h"
#include "ordering.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"
#include "stats/BDDBuildTimeCounter.h"
#include "utils/configuration.h"
#include "utils/file_utils.h"
#include "utils/minterm_utils.h"
#include "utils/output_utils.h"
#include "utils/results_output.h"
#include <climits>
#include <filesystem>
#include <future>
#include <iomanip>
#include <cudd.h>


namespace chrono = std::chrono;

using std::chrono::operator""min;
using std::chrono::operator""s;

std::string getTimestamp() {
    std::time_t now = time(nullptr);
    std::tm *ltm = std::localtime(&now);
    std::stringstream transTime;
    transTime << std::put_time(ltm, "%y.%m.%d-%H.%M.%S");
    return transTime.str() + "." +
           std::to_string(std::chrono::high_resolution_clock::now()
                              .time_since_epoch()
                              .count());
}

void setup_logger() {
    std::time_t now = time(nullptr);
    std::tm *ltm = std::localtime(&now);
    std::stringstream transTime;
    transTime << std::put_time(ltm, "%y.%m.%d-%H.%M.%S");
    std::string log_name =
        fmt::format("{}/{}.log", Configuration::getOutputDirectory(), "run");

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_name));
    auto combined_logger =
        std::make_shared<spdlog::logger>("log", begin(sinks), end(sinks));

    combined_logger->flush_on(spdlog::level::debug);
    spdlog::set_default_logger(combined_logger);
    //    spdlog::set_pattern("[%H:%M:%S:%e] [%l] [%s:%#] %v");
    spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v");
}



DdNode *createBDD(RulesetInfo info, DdManager *gbm) {

    // Dynamic reordering
    if (Configuration::isEnableDynamicOrdering()) {
        Cudd_AutodynEnable(gbm, CUDD_REORDER_SIFT);
//        Cudd_ReduceHeap(gbm, CUDD_REORDER_SYMM_SIFT, 3000);
        spdlog::info("Dynamic ordering enabled");
    } else {
        spdlog::info("Dynamic ordering disabled");
    }

    // Static reordering
    info =
        bdd::performOrdering(gbm, info, Configuration::getRulesetOrderingStrategy(),
                     Configuration::getClauseOrderingStrategy(),
                     Configuration::getVariableOrdering());

    spdlog::info("Using '{}' ruleset construction strategy",
                 Configuration::getConstructionRulesetOrdering());

    // Construct BDD
    DdNode *bdd;
    BDDBuildTimeCounter::constructionStarted();
    if (Configuration::getConstructionRulesetOrdering() == "dfs") {
        bdd = createRuleset(gbm, info, Configuration::getPrintProgress());
    } else if (Configuration::getConstructionRulesetOrdering() ==
               "merge_parts") {
        bdd = createRulesetMergedParts(gbm, info,
                                       Configuration::getMergePartsAmount(),
                                       Configuration::getPrintProgress());
    } else if (Configuration::getConstructionRulesetOrdering() ==
               "merge_recursive") {
        bdd = createRulesetRecursively(gbm, info);
    } else {
        spdlog::info(
            "No known ruleset construction strategy specified, using 'dfs'");
        bdd = createRuleset(gbm, info, Configuration::getPrintProgress());
    }

    BDDBuildTimeCounter::constructionFinished();

    spdlog::info("Reordering and construction time: {}ms",
                 BDDBuildTimeCounter::getTotalTimeInMilliseconds());
    return bdd;
}

void createBddInThread(RulesetInfo info, DdManager *gbm,
                       std::atomic<bool> *done, DdNode **bdd) {
    *bdd = createBDD(info, gbm);
    *done = true;
}

SddNode* createSdd(RulesetInfo& info, SddManager *sm) {

    BDDBuildTimeCounter::constructionStarted();
    SddNode* sdd = sdd::createRuleset(sm, info, Configuration::getPrintProgress());
    BDDBuildTimeCounter::constructionFinished();
    return sdd;
}

void createSddInThread(RulesetInfo info, SddManager *sm, std::atomic<bool> *done, SddNode **sdd) {
    *sdd = createSdd(info, sm);
    *done = true;
}

void startBDDPipeline(RulesetInfo& info) {
    DdManager *gbm = Cudd_Init(info.variableAmount, 0, CUDD_UNIQUE_SLOTS,
                               CUDD_CACHE_SLOTS, 0);
    DdNode *bdd;

    std::atomic<bool> constructionDone = false;
    std::thread bddConstruction(&createBddInThread, info, gbm,
                                &constructionDone, &bdd);
    chrono::steady_clock::time_point process_begin =
        chrono::steady_clock::now();

    const chrono::minutes timeLimitMin = chrono::minutes(Configuration::getTimeLimitMin());
    while (chrono::duration_cast<chrono::minutes>(
               chrono::steady_clock::now() - process_begin) < timeLimitMin &&
           !constructionDone) {
        std::this_thread::sleep_for(30s);
    }

    if (!constructionDone) {
        spdlog::warn(
            "The task has been executing for over {} minutes, stopping now", timeLimitMin.count());
        utils::logRunInfo(-1, -1, -1,
                          BDDBuildTimeCounter::getOrderingTimeInMilliseconds());
        exit(EXIT_FAILURE);
    }

    bddConstruction.join();

    utils::logRunInfo(Cudd_ReadNodeCount(gbm),
                      BDDBuildTimeCounter::getTotalTimeInMilliseconds(),
                      BDDBuildTimeCounter::getConstructionTimeInMilliseconds(),
                      BDDBuildTimeCounter::getOrderingTimeInMilliseconds());

    double mintermsCount = Cudd_CountMinterm(gbm, bdd, info.variableAmount);
    spdlog::info("Minterm count: {0:f}", mintermsCount);

    // printMinterms(getMinterms(gbm, bdd, info.variableAmount, 50));

    Cudd_RecursiveDeref(gbm, bdd);
    int num_reference_nodes = Cudd_CheckZeroRef(gbm);
    spdlog::debug("#Nodes with non-zero reference count (should be 0): {0:d}",
                  num_reference_nodes);
}

void startSDDPipeline(RulesetInfo& info) {
    SddManager* manager;
    if (Configuration::getVtreePath() != "") {
        spdlog::info("Setting vtree from {}", Configuration::getVtreePath());
        Vtree* saved_vtree = sdd_vtree_read(Configuration::getVtreePath().c_str());
        manager = sdd_manager_new(saved_vtree);
    } else if (Configuration::getVariableOrdering() != "") {
        std::vector<int> orderedVariablesInt = orderVariables(info, Configuration::getVariableOrdering());
        std::vector<long> orderedVariables(orderedVariablesInt.begin(), orderedVariablesInt.end());   
        SddLiteral *variableOrderingArray = &orderedVariables[0];
        Vtree* orderedVtree = sdd_vtree_new_with_var_order(info.variableAmount, variableOrderingArray, "balanced");
        manager = sdd_manager_new(orderedVtree);
        sdd_manager_auto_gc_and_minimize_on(manager);
    } else {
        SddLiteral varCount = info.variableAmount;
        int autoGcAndMinimize = 1;
        manager = sdd_manager_create(varCount, autoGcAndMinimize);
    }

    info = orderRuleset(info, Configuration::getRulesetOrderingStrategy());

    SddNode *sdd;
    std::atomic<bool> constructionDone = false;
    std::thread sddConstruction(&createSddInThread, info, manager,
                                &constructionDone, &sdd);
    chrono::steady_clock::time_point process_begin =
        chrono::steady_clock::now();

    const chrono::minutes timeLimitMin = chrono::minutes(Configuration::getTimeLimitMin());
    while (chrono::duration_cast<chrono::minutes>(
               chrono::steady_clock::now() - process_begin) < timeLimitMin &&
           !constructionDone) {
        std::this_thread::sleep_for(30s);
    }
    

    if (!constructionDone) {
        spdlog::warn(
            "The task has been executing for over {} minutes, stopping now", timeLimitMin.count());
        utils::logRunInfo(-1, -1, -1,-1);
        exit(EXIT_FAILURE);
    }

    sddConstruction.join();

    utils::logRunInfo(sdd_count(sdd),
                      BDDBuildTimeCounter::getTotalTimeInMilliseconds(),
                      BDDBuildTimeCounter::getConstructionTimeInMilliseconds(),
                      BDDBuildTimeCounter::getOrderingTimeInMilliseconds());

    // const char *vtree_filename = ((std::string)"vtree.txt").c_str();

    sdd_deref(sdd, manager);
    sdd_manager_free(manager);
}

int main(int argc, char *argv[]) {
    std::string configPath = "config/config.yaml";
    Configuration::getInstance()->load(configPath);
    Configuration::parseArgs(argc, argv);

    std::string outputDirName = "output/" + getTimestamp();
    if (!std::filesystem::exists("output"))
        std::filesystem::create_directories("output");
    std::filesystem::create_directories(outputDirName);
    Configuration::setOutputDirectory(outputDirName);

    std::filesystem::copy_file(
        configPath, Configuration::getOutputDirectory() + "/config.yaml");
    setup_logger();


    RulesetInfo info = readClauselSetInfo(Configuration::getInputFilename());
    printRulesetStats(info);

    if (Configuration::getDiagramType() == "bdd") {
        startBDDPipeline(info);
    } else if (Configuration::getDiagramType() == "sdd") {
        startSDDPipeline(info);
    }
    
    return 0;
}