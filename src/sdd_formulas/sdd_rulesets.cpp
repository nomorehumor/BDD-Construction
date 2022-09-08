#include "sdd_rulesets.h"
#include <chrono>
#include "spdlog/spdlog.h"
#include "sdd_formulas.h"

namespace chrono = std::chrono;

namespace sdd {

SddNode* createRuleset(SddManager* sm, RulesetInfo& info, bool progress_output, BDDBuildStatistic *statistic) {

    chrono::steady_clock::time_point process_begin =
        chrono::steady_clock::now();

    SddNode *tmp, *sdd;
    if (info.type == Form::CNF) {
        sdd = sdd_manager_true(sm);
    } else if (info.type == Form::DNF) {
        sdd = sdd_manager_false(sm);
    }
    bool sddSet = false;

    chrono::steady_clock::time_point iteration_begin;
    chrono::steady_clock::time_point iteration_end;
    BDDBuildStatistic localStatistic(info.clauselAmount, 50);
    if (statistic == nullptr) {
        statistic = &localStatistic;
    }
    for (int i = 0; i < info.formulas.size(); i++) {
        iteration_begin = chrono::steady_clock::now();

        SddNode *formula = sdd::createFormulaSdd(sm, info.formulas[i]);
        if (info.type == Form::CNF) {
            tmp = sdd_conjoin(sdd, formula, sm);
        } else if (info.type == Form::DNF) {
            tmp = sdd_disjoin(sdd, formula, sm);
        }
        sdd_ref(tmp, sm);
        if (sddSet) sdd_deref(sdd, sm);
        else sddSet = true;
        sdd = tmp;

        iteration_end = chrono::steady_clock::now();

        int stepTime_ms = chrono::duration_cast<chrono::milliseconds>(
                              iteration_end - iteration_begin)
                              .count();
        if (progress_output) {
            statistic->logStep(info.formulas.at(i), 1,
                              sdd_count(sdd), stepTime_ms);
            // statistic->logCudd(gbm);
            statistic->logTime(chrono::duration_cast<chrono::seconds>(
                                         iteration_end - process_begin)
                                         .count());
        }
    }

    spdlog::info(
        "Ruleset SDD generated in {0:d}ms",
        chrono::duration_cast<chrono::milliseconds>(iteration_end - process_begin)
            .count());

    return sdd;
}

}