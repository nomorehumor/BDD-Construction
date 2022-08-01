//
// Created by Maxim.Popov on 01.08.2022.
//

#include "bdd_rulesets.h"

DdNode *createRulesetMerged(DdManager *gbm, RulesetInfo setInfo,
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

DdNode *createRuleset(DdManager *gbm, RulesetInfo setInfo,
                      bool progress_output) {
    chrono::steady_clock::time_point process_begin =
        chrono::steady_clock::now();

    DdNode *tmp, *bdd;
    if (setInfo.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
    } else if (setInfo.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
    }
    Cudd_Ref(bdd);

    spdlog::info("Using '{}' topological ordering strategy",
                 BDDConfiguration::getConstructionFormulaOrdering());

    chrono::steady_clock::time_point iteration_begin;
    chrono::steady_clock::time_point iteration_end;
    BDDBuildStatistic statistic(setInfo.clauselAmount, 50);
    for (int i = 0; i < setInfo.formulas.size(); i++) {
        iteration_begin = chrono::steady_clock::now();

        DdNode *formula;

        if (setInfo.formulas.at(i).type == Form::AMO) {
            formula = createAMOFormulaFromInfo(gbm, setInfo.formulas.at(i));
        } else {
            if (BDDConfiguration::getConstructionFormulaOrdering() == "dfs") {
                formula = createNFFormulaFromInfo(gbm, setInfo.formulas.at(i));
            } else if (BDDConfiguration::getConstructionFormulaOrdering() ==
                       "merge") {
                formula = createNFFormulaMerge(gbm, setInfo.formulas.at(i));
            } else {
                formula = createNFFormulaFromInfo(gbm, setInfo.formulas.at(i));
            }
        }

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
            statistic.logStep(setInfo.formulas.at(i), i + 1,
                              Cudd_ReadNodeCount(gbm), stepTime_ms);
            statistic.logCudd(gbm, i + 1);
            statistic.logTime(i + 1, chrono::duration_cast<chrono::seconds>(
                                         iteration_end - process_begin)
                                         .count());
        }
    }

    spdlog::info(
        "BDD generated in {0:d}s",
        chrono::duration_cast<chrono::seconds>(iteration_end - process_begin)
            .count());

    return bdd;
}