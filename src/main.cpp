
#include "bdd_formulas/bdd_rulesets.h"
#include "bdd_formulas/static_ordering/formula_ordering.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"
#include "utils/BDDConfiguration.h"
#include "utils/file_utils.h"
#include "utils/minterm_utils.h"
#include "utils/output_utils.h"
#include <climits>
#include <filesystem>
#include <future>
#include <iomanip>

namespace chrono = std::chrono;

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
    std::string log_name = fmt::format(
        "{}/{}.log", BDDConfiguration::getOutputDirectory(), getTimestamp());

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

void printRulesetStats(RulesetInfo &setInfo) {
    spdlog::info("======= Ruleset info =======");
    spdlog::info("Variables amount: {0:d} | Formulas amount: {1:d}",
                 setInfo.variableAmount, setInfo.clauselAmount);

    int amoFormulasAmount = 0;
    int dnfFormulasAmount = 0;
    int cnfFormulasAmount = 0;
    int unknownFormulaTypeAmount = 0;
    for (FormulaInfo &info : setInfo.formulas) {
        switch (info.type) {
        case Form::AMO:
            amoFormulasAmount++;
            break;
        case Form::DNF:
            dnfFormulasAmount++;
            break;
        case Form::CNF:
            cnfFormulasAmount++;
            break;
        default:
            unknownFormulaTypeAmount++;
        }
    }
    spdlog::info("DNF: {0:d}", dnfFormulasAmount);
    spdlog::info("CNF: {0:d}", cnfFormulasAmount);
    spdlog::info("AMO: {0:d}", amoFormulasAmount);
    spdlog::info("Unknown type: {0:d}", unknownFormulaTypeAmount);
}

void printMinterms(std::vector<std::vector<bool>> minterms) {
    if (minterms.empty())
        spdlog::warn("No minterms for this BDD");
    for (int i = 0; i < minterms.size(); i++) {
        std::string mintermRepr = "";

        for (int j = 0; j < minterms[i].size(); j++) {
            mintermRepr += std::to_string(minterms[i][j]);
        }
        spdlog::info("#{0:d} {1}", i, mintermRepr);
    }
}

DdNode *createBDD(RulesetInfo info, DdManager *gbm) {

    info = orderRuleset(info, BDDConfiguration::getOrderingStrategy(),
                        BDDConfiguration::getClauseOrderingStrategy());

    // Dynamic reordering
    if (BDDConfiguration::isEnableDynamicOrdering()) {
        Cudd_AutodynEnable(gbm, CUDD_REORDER_SIFT);
        spdlog::info("Dynamic ordering enabled");
    } else {
        spdlog::info("Dynamic ordering disabled");
    }

    spdlog::info("Using '{}' ruleset construction strategy",
                 BDDConfiguration::getConstructionRulesetOrdering());

    // Construct BDD
    if (BDDConfiguration::getConstructionRulesetOrdering() == "dfs") {
        return createRuleset(gbm, info, BDDConfiguration::getPrintProgress());
    } else if (BDDConfiguration::getConstructionRulesetOrdering() == "merge_parts") {
        return createRulesetMergedParts(gbm, info,
                                   BDDConfiguration::getMergePartsAmount(),
                                   BDDConfiguration::getPrintProgress());
    } else if (BDDConfiguration::getConstructionRulesetOrdering() == "merge_recursive") {

        return createRulesetRecursively(gbm, info);
    } else {
        spdlog::info(
            "No known ruleset construction strategy specified, using 'dfs'");
        return createRuleset(gbm, info, BDDConfiguration::getPrintProgress());
    }
}

void createBddInThread(RulesetInfo info, DdManager* gbm, std::atomic<bool>* done, DdNode* bdd) {
    bdd = createBDD(info, gbm);
    *done = true;
}

int main(int argc, char *argv[]) {
    std::string configPath = "config/config.yaml";
    BDDConfiguration::getInstance()->load(configPath);
    BDDConfiguration::parseArgs(argc, argv);

    std::string outputDirName = "output/" + getTimestamp();
    if (!std::filesystem::exists("output"))
        std::filesystem::create_directories("output");
    std::filesystem::create_directories(outputDirName);
    BDDConfiguration::setOutputDirectory(outputDirName);
    std::filesystem::copy_file(configPath, outputDirName + "/config.yaml");

    setup_logger();

    RulesetInfo info = readClauselSetInfo(BDDConfiguration::getInputFilename());
    printRulesetStats(info);

    DdManager *gbm = Cudd_Init(info.variableAmount, 0, CUDD_UNIQUE_SLOTS,
                               CUDD_CACHE_SLOTS, 0);
    DdNode* bdd;
    std::atomic<bool> constructionDone = false;
    std::thread bddConstruction(&createBddInThread, info, gbm, &constructionDone, bdd);
    chrono::steady_clock::time_point process_begin =
        chrono::steady_clock::now();

    while(!constructionDone) {
        if (chrono::duration_cast<chrono::minutes>(chrono::steady_clock::now() - process_begin).count() >= 5) {
            spdlog::warn("The task has been executing for over 5 minutes, stopping now");
            exit(EXIT_FAILURE);
        }
    }

    double mintermsCount = Cudd_CountMinterm(gbm, bdd, info.variableAmount);
    spdlog::info("Minterm count: {0:f}", mintermsCount);

    printMinterms(getMinterms(gbm, bdd, info.variableAmount, 50));

    Cudd_RecursiveDeref(gbm, bdd);
    int num_reference_nodes = Cudd_CheckZeroRef(gbm);
    spdlog::debug("#Nodes with non-zero reference count (should be 0): {0:d}",
                  num_reference_nodes);
    return 0;
}