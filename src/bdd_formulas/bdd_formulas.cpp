//
// Created by Maxim.Popov on 04.07.2022.
//

#include "bdd_formulas.h"
#include "../stats/BDDBuildStatistic.h"
#include "../utils/BDDConfiguration.h"
#include "../utils/output_utils.h"
#include "../utils/progress_bar.h"
#include "matplotlibcpp.h"
#include "spdlog/spdlog.h"
#include <chrono>

namespace chrono = std::chrono;



DdNode *createAMOFormulaFromInfo(DdManager *gbm, FormulaInfo amoInfo) {
    DdNode *tmp, *tmpVar;
    DdNode *bdd = Cudd_ReadLogicZero(gbm);
    Cudd_Ref(bdd);
    DdNode *notTerm = Cudd_ReadOne(gbm);
    Cudd_Ref(notTerm);
    DdNode *zeroTerm = Cudd_ReadLogicZero(gbm);
    Cudd_Ref(zeroTerm);

    for (int i = 0; i < amoInfo.symbols.size(); i++) {
        if (amoInfo.symbols.at(i) == 0)
            continue;
        tmpVar = Cudd_bddIthVar(gbm, std::abs(amoInfo.symbols.at(i)) - 1);
        if (amoInfo.symbols.at(i) < 0) {
            tmpVar = Cudd_Not(tmpVar);
        }

        tmp = Cudd_bddXor(gbm, tmpVar, bdd);
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, bdd);
        bdd = tmp;

        tmp = Cudd_bddAnd(gbm, tmpVar, notTerm);
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, notTerm);
        notTerm = tmp;

        tmp = Cudd_bddOr(gbm, tmpVar, zeroTerm);
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, zeroTerm);
        zeroTerm = tmp;
    }

    notTerm = Cudd_Not(notTerm);
    tmp = Cudd_bddAnd(gbm, bdd, notTerm);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(gbm, bdd);
    Cudd_RecursiveDeref(gbm, notTerm);
    bdd = tmp;

    zeroTerm = Cudd_Not(zeroTerm);
    tmp = Cudd_bddOr(gbm, bdd, zeroTerm);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(gbm, bdd);
    Cudd_RecursiveDeref(gbm, zeroTerm);
    bdd = tmp;

    return bdd;
}

DdNode *createCNFClause(DdManager *gbm, std::vector<int> clauseVars) {
    DdNode *clause = Cudd_ReadLogicZero(gbm);
    Cudd_Ref(clause);
    DdNode *tmpVar, *tmpClause;

    for (int var : clauseVars) {
        tmpVar = Cudd_bddIthVar(gbm, std::abs(var) - 1);
        if (var < 0) {
            tmpVar = Cudd_Not(tmpVar);
        }
        tmpClause = Cudd_bddOr(gbm, tmpVar, clause);
        Cudd_Ref(tmpClause);
        Cudd_RecursiveDeref(gbm, clause);
        clause = tmpClause;
    }
    return clause;
}

DdNode *createDNFClause(DdManager *gbm, std::vector<int> clauseVars) {
    DdNode *clause = Cudd_ReadOne(gbm);
    Cudd_Ref(clause);
    DdNode *tmpVar, *tmpClause;

    for (int var : clauseVars) {
        tmpVar = Cudd_bddIthVar(gbm, std::abs(var) - 1);
        if (var < 0) {
            tmpVar = Cudd_Not(tmpVar);
        }
        tmpClause = Cudd_bddAnd(gbm, tmpVar, clause);
        Cudd_Ref(tmpClause);
        Cudd_RecursiveDeref(gbm, clause);
        clause = tmpClause;
    }
    return clause;
}

DdNode *createNFFormulaFromClauses(DdManager *gbm, FormulaInfo info) {
    DdNode *clauseBdd, *tmpClause, *bdd;

    if (info.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
    } else if (info.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
    }
    Cudd_Ref(bdd);

    for (std::vector<int> clause : info.clauses) {

        if (info.type == Form::DNF) {
            clauseBdd = createDNFClause(gbm, clause);
        } else if (info.type == Form::CNF) {
            clauseBdd = createCNFClause(gbm, clause);
        }

        if (info.type == Form::DNF) {
            tmpClause = Cudd_bddOr(gbm, clauseBdd, bdd);
        } else if (info.type == Form::CNF) {
            tmpClause = Cudd_bddAnd(gbm, clauseBdd, bdd);
        }
        Cudd_Ref(tmpClause);
        Cudd_RecursiveDeref(gbm, clauseBdd);
        Cudd_RecursiveDeref(gbm, bdd);
        bdd = tmpClause;
    }
    return bdd;
}

DdNode *createNFFormulaMerge(DdManager *gbm, FormulaInfo info) {
    if (info.clauses.size() == 0) {
        DdNode *constant;
        if (info.type == Form::DNF) {
            constant = Cudd_ReadLogicZero(gbm);
        } else if (info.type == Form::CNF) {
            constant = Cudd_ReadOne(gbm);
        }
        Cudd_Ref(constant);
        return constant;
    } else if (info.clauses.size() == 1) {
        return info.type == Form::DNF ? createDNFClause(gbm, info.clauses[0])
                                      : createCNFClause(gbm, info.clauses[0]);
    } else {
        FormulaInfo formulaHalf1 = info;
        FormulaInfo formulaHalf2 = info;

        formulaHalf1.clauses =
            std::vector<std::vector<int>>(info.clauses.begin(),
                             info.clauses.begin() + info.clauses.size() / 2);
        formulaHalf2.clauses = std::vector<std::vector<int>>(
            info.clauses.begin() + (info.clauses.size() / 2),
            info.clauses.end());

        DdNode *firstHalf = createNFFormulaMerge(gbm, formulaHalf1);
        DdNode *secondHalf = createNFFormulaMerge(gbm, formulaHalf2);

        DdNode *bdd = info.type == Form::DNF ? Cudd_bddOr(gbm, firstHalf, secondHalf) : Cudd_bddAnd(gbm, firstHalf, secondHalf);
        Cudd_Ref(bdd);
        Cudd_RecursiveDeref(gbm, firstHalf);
        Cudd_RecursiveDeref(gbm, secondHalf);

        return bdd;
    }
}

DdNode *createNFFormulaFromInfo(DdManager *gbm, FormulaInfo info) {
    DdNode *lastVariables, *tmpLastVariables, *tmpVar, *bdd;
    lastVariables = NULL;
    if (info.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
        lastVariables = Cudd_ReadOne(gbm);
    } else if (info.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
        lastVariables = Cudd_ReadLogicZero(gbm);
    }
    Cudd_Ref(bdd);
    Cudd_Ref(lastVariables);

    for (int i = 0; i < info.symbols.size(); i++) {
        if (info.symbols.at(i) == 0 && (info.symbols.at(i - 1) != 0)) {
            if (info.type == Form::DNF) {
                tmpVar = Cudd_bddOr(gbm, lastVariables, bdd);
                Cudd_Ref(tmpVar);
                Cudd_RecursiveDeref(gbm, lastVariables);
                lastVariables = Cudd_ReadOne(gbm);
            } else if (info.type == Form::CNF) {
                tmpVar = Cudd_bddAnd(gbm, lastVariables, bdd);
                Cudd_Ref(tmpVar);
                Cudd_RecursiveDeref(gbm, lastVariables);
                lastVariables = Cudd_ReadLogicZero(gbm);
            }
            Cudd_Ref(lastVariables);
            Cudd_RecursiveDeref(gbm, bdd);
            bdd = tmpVar;
        } else if (info.symbols.at(i) != 0) {
            tmpVar = Cudd_bddIthVar(gbm, std::abs(info.symbols.at(i)) - 1);
            if (info.symbols.at(i) < 0) {
                tmpVar = Cudd_Not(tmpVar);
            }
            if (info.type == Form::DNF) {
                tmpLastVariables = Cudd_bddAnd(gbm, tmpVar, lastVariables);
            } else if (info.type == Form::CNF) {
                tmpLastVariables = Cudd_bddOr(gbm, tmpVar, lastVariables);
            }
            Cudd_Ref(tmpLastVariables);
            Cudd_RecursiveDeref(gbm, lastVariables);
            lastVariables = tmpLastVariables;
        }
    }

    Cudd_RecursiveDeref(gbm, lastVariables);

    return bdd;
}

DdNode *createRulesetMerged(DdManager *gbm, RulesetInfo setInfo, int partsAmount, bool progressOutput) {
    if (partsAmount > setInfo.formulas.size()) {
        partsAmount = setInfo.formulas.size();
    }

    std::vector<RulesetInfo> partialRulesets;
    int formulasInRuleset = ceil((double) setInfo.formulas.size() / (double) partsAmount);
    for (int i = 0; i < partsAmount; i++) {
        RulesetInfo rulesetPart = setInfo;
        if (i == partsAmount - 1) {
            rulesetPart.formulas = std::vector<FormulaInfo>(setInfo.formulas.begin() + i * formulasInRuleset, setInfo.formulas.end());
        } else {
            rulesetPart.formulas = std::vector<FormulaInfo>(setInfo.formulas.begin() + i * formulasInRuleset, setInfo.formulas.begin() + (i+1) * formulasInRuleset);
        }
        partialRulesets.push_back(rulesetPart);
    }

    DdNode *tmp, *bdd;
    if (setInfo.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
    } else if (setInfo.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
    }
    Cudd_Ref(bdd);

    spdlog::info("Splitted the ruleset into {0:d} parts", partialRulesets.size());
    int i = 0;
    for (RulesetInfo rulesetPart : partialRulesets) {
        spdlog::info("Creating BDD for part #{0:d}",i);
        i++;

        DdNode* bddPart = createRuleset(gbm, rulesetPart, progressOutput);

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

    spdlog::info("Using '{}' topological ordering strategy", BDDConfiguration::getTopologicalOrdering());

    chrono::steady_clock::time_point iteration_begin;
    chrono::steady_clock::time_point iteration_end;
    BDDBuildStatistic statistic(setInfo.clauselAmount, 50);
    for (int i = 0; i < setInfo.formulas.size(); i++) {
        iteration_begin = chrono::steady_clock::now();

        DdNode *formula;

        if (setInfo.formulas.at(i).type == Form::AMO) {
            formula = createAMOFormulaFromInfo(gbm, setInfo.formulas.at(i));
        } else {
            if (BDDConfiguration::getTopologicalOrdering() == "dfs") {
                formula = createNFFormulaFromInfo(gbm, setInfo.formulas.at(i));
            } else if (BDDConfiguration::getTopologicalOrdering() == "merge") {
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

    spdlog::info("BDD generated in {0:d}s", chrono::duration_cast<chrono::seconds>(
                                                iteration_end - process_begin)
                                                .count());

    return bdd;
}