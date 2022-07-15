/*
* FILENAME: transfer.c
* Overview: BDD tutorial
* AUTHOR: David Kebo Houngninou
*/

#include <iostream>
#include <iomanip>
#include "bdd_formulas/bdd_formulas.h"
#include "utils/output_utils.h"
#include "bdd_formulas/formula_ordering.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/stdout_color_sinks.h"


void setup_logger() {
    std::time_t now = time(nullptr);
    std::tm *ltm = std::localtime(&now);
    std::stringstream transTime;
    transTime << std::put_time(ltm, "%y.%m.%M-%H.%M.%S");
    std::string log_name = fmt::format("logs/{}.log", transTime.str());

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_name));
    auto combined_logger = std::make_shared<spdlog::logger>("log", begin(sinks), end(sinks));

    combined_logger->flush_on(spdlog::level::info);
    spdlog::set_default_logger(combined_logger);
    spdlog::set_pattern("[%H:%M:%S:%e] [%l] [%s:%#] %v");
}

// This program creates a single BDD variable
int main (int argc, char *argv[])
{
    setup_logger();

    char* filename = "C:\\dev\\Bachelor\\merlin-formulas\\wingas\\merlin-clauseset420509740828757424.txt";
//    char* filename = "C:\\dev\\Bachelor\\test-formulas\\test_amo.txt";
    RulesetInfo info = readClauselSetInfo(filename);

    info = orderRulesetFrequentVariables(info, false);

    DdManager *gbm; /* Global BDD manager. */
    gbm = Cudd_Init(info.variableAmount,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0); /* Initialize a new BDD manager. */
    Cudd_AutodynEnable(gbm, CUDD_REORDER_SIFT);

    DdNode* bdd = createRuleset(gbm, info, true);
    print_dd(gbm, bdd);
    SPDLOG_INFO("Minterm count: {0:f}", Cudd_CountMinterm(gbm, bdd, info.variableAmount));

    char out_filename[30];
    sprintf(out_filename, "bdd.dot");
//    bdd = Cudd_BddToAdd(gbm, bdd);
    write_dd(gbm, bdd, out_filename);

    Cudd_RecursiveDeref(gbm, bdd);

    int num_reference_nodes = Cudd_CheckZeroRef(gbm);
    SPDLOG_INFO("#Nodes with non-zero reference count (should be 0): {0:d}", num_reference_nodes);

}