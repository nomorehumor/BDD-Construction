
#include "bdd_formulas/bdd_formulas.h"
#include "bdd_formulas/static_ordering/formula_ordering.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"
#include "utils/BDDConfiguration.h"
#include "utils/file_utils.h"
#include "utils/minterm_utils.h"
#include "utils/output_utils.h"
#include <filesystem>
#include <iomanip>
#include <climits>
#include <iostream>

std::string getTimestamp() {
    std::time_t now = time(nullptr);
    std::tm *ltm = std::localtime(&now);
    std::stringstream transTime;
    transTime << std::put_time(ltm, "%y.%m.%d-%H.%M.%S");
    return transTime.str();
}

void setup_logger() {
    std::time_t now = time(nullptr);
    std::tm *ltm = std::localtime(&now);
    std::stringstream transTime;
    transTime << std::put_time(ltm, "%y.%m.%d-%H.%M.%S");
    std::string log_name = fmt::format("logs/{}.log", getTimestamp());

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_name));
    auto combined_logger =
        std::make_shared<spdlog::logger>("log", begin(sinks), end(sinks));

    combined_logger->flush_on(spdlog::level::info);
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
    if (minterms.empty()) spdlog::info("No minterms for this BDD");
    int varAmount = minterms[0].size();
    std::vector<int> mintermsBinary = convertBooleanMintermsToBinary(minterms);
    for (int i = 0; i <  mintermsBinary.size(); i++) {
        std::string mintermRepr = "";

        int mintermRest = mintermsBinary[i];
        for (int i = 0; i < varAmount; i++) {
            mintermRepr += std::to_string(mintermRest % 2);
            mintermRest = mintermRest >> 1;
        }
        spdlog::info("#{0:d} {1}", i, mintermRepr);
    }
}

int main(int argc, char *argv[]) {
    setup_logger();
    std::string configPath = "config/config.yaml";
    BDDConfiguration::getInstance()->load(configPath);
    BDDConfiguration::parseArgs(argc, argv);

    std::string outputDirName = "output/" + getTimestamp();
    if (!std::filesystem::exists("output")) std::filesystem::create_directories("output");
    std::filesystem::create_directories(outputDirName);
    BDDConfiguration::setOutputDirectory(outputDirName);
    std::filesystem::copy_file(configPath, outputDirName + "/config.yaml");

    RulesetInfo info = readClauselSetInfo(BDDConfiguration::getInputFilename());
    printRulesetStats(info);

    info = orderRuleset(info, BDDConfiguration::getOrderingStrategy(), BDDConfiguration::getClauseOrderingStrategy());

    DdManager *gbm; /* Global BDD manager. */
    gbm = Cudd_Init(info.variableAmount, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS,
                    0); /* Initialize a new BDD manager. */
    if (BDDConfiguration::isEnableDynamicOrdering()) {
        Cudd_AutodynEnable(gbm, CUDD_REORDER_SIFT);
        spdlog::info("Dynamic ordering enabled");
    } else {
        spdlog::info("Dynamic ordering disabled");
    }

    DdNode *bdd =
        createRuleset(gbm, info, BDDConfiguration::getPrintProgress());
    print_dd(gbm, bdd);
    spdlog::info("Minterm count: {0:d}",
                 (int) Cudd_CountMinterm(gbm, bdd, info.variableAmount));

    printMinterms(getMinterms(gbm, bdd, info.variableAmount, INT_MAX));

    //    char out_filename[30];
    //    sprintf(out_filename, "bdd.dot");
    //    bdd = Cudd_BddToAdd(gbm, bdd);
    //    write_dd(gbm, bdd, out_filename);

    Cudd_RecursiveDeref(gbm, bdd);

    int num_reference_nodes = Cudd_CheckZeroRef(gbm);
    spdlog::info("#Nodes with non-zero reference count (should be 0): {0:d}",
                 num_reference_nodes);
}