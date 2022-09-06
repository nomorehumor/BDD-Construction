//
// Created by Maxim.Popov on 01.08.2022.
//

#include "bdd_rulesets.h"
#include "../utils/configuration.h"
#include "spdlog/spdlog.h"
#include <chrono>
#include "bdd_formulas.h"

namespace chrono = std::chrono;

DdNode *createRulesetMergedParts(DdManager *gbm, RulesetInfo setInfo,
                            int partsAmount, bool progressOutput) {
    if (partsAmount > setInfo.formulas.size()) {
        partsAmount = setInfo.formulas.size();
    }

    std::vector<RulesetInfo> partialRulesets;
    int formulasInRuleset =
        ceil((double)setInfo.formulas.size() / (double)partsAmount);
    for (int i = 0; i < partsAmount; i++) {
        RulesetInfo rulesetPart = setInfo;
        if (i == partsAmount - 1) {
            rulesetPart.formulas = std::vector<FormulaInfo>(
                setInfo.formulas.begin() + i * formulasInRuleset,
                setInfo.formulas.end());
        } else {
            rulesetPart.formulas = std::vector<FormulaInfo>(
                setInfo.formulas.begin() + i * formulasInRuleset,
                setInfo.formulas.begin() + (i + 1) * formulasInRuleset);
        }
        rulesetPart.clauselAmount = rulesetPart.formulas.size();
        partialRulesets.push_back(rulesetPart);
    }

    DdNode *tmp, *bdd;
    if (setInfo.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
    } else if (setInfo.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
    }
    Cudd_Ref(bdd);

    spdlog::info("Splitted the ruleset into {0:d} parts",
                 partialRulesets.size());
    int i = 0;
    for (RulesetInfo rulesetPart : partialRulesets) {
        spdlog::info("Creating BDD for part #{0:d}", i);
        i++;

        DdNode *bddPart = createRuleset(gbm, rulesetPart, progressOutput);

        if (setInfo.type == Form::CNF) {
            tmp = Cudd_bddAnd(gbm, bddPart, bdd);
        } else if (setInfo.type == Form::DNF) {
            tmp = Cudd_bddOr(gbm, bddPart, bdd);
        }
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, bddPart);
        Cudd_RecursiveDeref(gbm, bdd);
        bdd = tmp;
    }

    return bdd;
}



DdNode *createRulesetRecursion(DdManager *gbm, RulesetInfo setInfo, int maxRecursionLevel, BDDBuildStatistic* progress) {
    if (maxRecursionLevel <= 0 || setInfo.formulas.size() == 2) {
        return createRuleset(gbm, setInfo, true, progress);
    }
    RulesetInfo set1 = setInfo;
    RulesetInfo set2 = setInfo;
    set1.formulas = std::vector<FormulaInfo>(
        setInfo.formulas.begin(),
        setInfo.formulas.begin() + setInfo.formulas.size() / 2);
    set2.formulas = std::vector<FormulaInfo>(
        setInfo.formulas.begin() + setInfo.formulas.size() / 2,
        setInfo.formulas.end());

    spdlog::info("Dividing bdd, recursion levels left: {}", maxRecursionLevel - 1);
    DdNode* bdd1 = createRulesetRecursion(gbm, set1, maxRecursionLevel-1, progress);
    DdNode* bdd2 = createRulesetRecursion(gbm, set2, maxRecursionLevel-1, progress);

    spdlog::info("Merging {} and {} clauses", set1.formulas.size(), set2.formulas.size());
    chrono::steady_clock::time_point merge_begin =
        chrono::steady_clock::now();
    chrono::steady_clock::time_point merge_end;

    DdNode *bdd;
    if (setInfo.type == Form::CNF) {
        bdd = Cudd_bddAnd(gbm, bdd1, bdd2);
    } else if (setInfo.type == Form::DNF) {
        bdd = Cudd_bddOr(gbm, bdd1, bdd2);
    }
    Cudd_Ref(bdd);
    Cudd_RecursiveDeref(gbm, bdd1);
    Cudd_RecursiveDeref(gbm, bdd2);

    merge_end = chrono::steady_clock::now();
    spdlog::info("Merged within {}ms", chrono::duration_cast<chrono::milliseconds>(
                                           merge_end - merge_begin)
                                           .count());

    return bdd;
}

DdNode *createRulesetRecursively(DdManager *gbm, RulesetInfo setInfo) {
    BDDBuildStatistic statisticForAllRuns(setInfo.clauselAmount, 50);
    chrono::steady_clock::time_point process_begin =
        chrono::steady_clock::now();
    chrono::steady_clock::time_point process_end;

    DdNode* bdd = createRulesetRecursion(gbm, setInfo, 10, &statisticForAllRuns);

    process_end = chrono::steady_clock::now();

    spdlog::info(
        "Ruleset BDD generated in {0:d}ms",
        chrono::duration_cast<chrono::milliseconds>(process_end - process_begin)
            .count());
    return bdd;
}

DdNode *createRuleset(DdManager *gbm, RulesetInfo setInfo, bool progress_output, BDDBuildStatistic* statistic) {
    chrono::steady_clock::time_point process_begin =
        chrono::steady_clock::now();

    DdNode *tmp, *bdd;
    if (setInfo.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
    } else if (setInfo.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
    }
    Cudd_Ref(bdd);

    chrono::steady_clock::time_point iteration_begin;
    chrono::steady_clock::time_point iteration_end;
    BDDBuildStatistic localStatistic(setInfo.clauselAmount, 50);
    if (statistic == nullptr) {
        statistic = &localStatistic;
    }
    for (int i = 0; i < setInfo.formulas.size(); i++) {
        iteration_begin = chrono::steady_clock::now();

        DdNode *formula = createFormulaBdd(gbm, setInfo.formulas[i]);

        if (setInfo.type == Form::CNF) {
            tmp = Cudd_bddAnd(gbm, formula, bdd);
        } else if (setInfo.type == Form::DNF) {
            tmp = Cudd_bddOr(gbm, formula, bdd);
        }
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, formula);
        Cudd_RecursiveDeref(gbm, bdd);
        bdd = tmp;

        iteration_end = chrono::steady_clock::now();

        int stepTime_ms = chrono::duration_cast<chrono::milliseconds>(
                              iteration_end - iteration_begin)
                              .count();
        if (progress_output) {
            statistic->logStep(setInfo.formulas.at(i), 1,
                              Cudd_ReadNodeCount(gbm), stepTime_ms);
            statistic->logCudd(gbm);
            statistic->logTime(chrono::duration_cast<chrono::seconds>(
                                         iteration_end - process_begin)
                                         .count());
        }
    }

    spdlog::info(
        "Ruleset BDD generated in {0:d}ms",
        chrono::duration_cast<chrono::milliseconds>(iteration_end - process_begin)
            .count());

    return bdd;
}