//
// Created by Maxim.Popov on 04.07.2022.
//

#include "bdd_formulas.h"
#include "../utils/progress_bar.h"
#include "../utils/output_utils.h"
#include "matplotlibcpp.h"
#include "../stats/BDDBuildStatistic.h"
#include <chrono>

std::vector<std::vector<bool>> getMinterms(DdManager* gbm, DdNode* bdd, int numVars, int maxAmount, bool output) {
    DdNode **vars = new DdNode*[numVars]();
    std::vector<std::vector<bool>> minterms;

    for (int i = 0; i < numVars; i++) {
        vars[i] = Cudd_bddIthVar(gbm, i);
    }

    DdNode *currentNode, *temp;
    currentNode = bdd;
    Cudd_Ref(currentNode);
    while (currentNode != Cudd_ReadLogicZero(gbm) && minterms.size() < maxAmount) {
        DdNode* minterm = Cudd_bddPickOneMinterm(gbm, currentNode, vars, numVars);

        if (output) {
            print_dd(gbm, minterm);
        }

        Cudd_Ref(minterm);
        std::vector<bool> mintermSolution;
        for (int i = 0; i < numVars; i++) {
            mintermSolution.push_back(Cudd_bddLeq(gbm, minterm, vars[i]));
        }
        minterms.push_back(mintermSolution);

        temp = Cudd_bddAnd(gbm, currentNode, Cudd_Not(minterm));
        Cudd_Ref(temp);
        Cudd_RecursiveDeref(gbm, currentNode);
        Cudd_RecursiveDeref(gbm, minterm);
        currentNode = temp;
    }

    Cudd_RecursiveDeref(gbm, currentNode);
    delete[] vars;
    return minterms;
}

DdNode* createAMOFormulaFromInfo(DdManager *gbm, FormulaInfo amoInfo) {
    DdNode *tmp, *tmpVar;
    DdNode* bdd = Cudd_ReadLogicZero(gbm);
    Cudd_Ref(bdd);
    DdNode* notTerm = Cudd_ReadOne(gbm);
    Cudd_Ref(notTerm);
    DdNode* zeroTerm = Cudd_ReadLogicZero(gbm);
    Cudd_Ref(zeroTerm);

    for (int i = 0; i < amoInfo.symbols.size(); i++) {
        if (amoInfo.symbols.at(i) == 0) continue;
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

DdNode* createNFFormulaFromInfo(DdManager *gbm, FormulaInfo info) {
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
                Cudd_RecursiveDeref(gbm,lastVariables);
                lastVariables = Cudd_ReadOne(gbm);
            } else if (info.type == Form::CNF) {
                tmpVar = Cudd_bddAnd(gbm, lastVariables, bdd);
                Cudd_Ref(tmpVar);
                Cudd_RecursiveDeref(gbm,lastVariables);
                lastVariables = Cudd_ReadLogicZero(gbm);
            }
            Cudd_Ref(lastVariables);
            Cudd_RecursiveDeref(gbm,bdd);
            bdd = tmpVar;
        } else if (info.symbols.at(i) != 0){
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

DdNode* createRuleset(DdManager *gbm, RulesetInfo setInfo, bool progress_output) {

    DdNode *tmp, *bdd;
    if (setInfo.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
    } else if (setInfo.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
    }
    Cudd_Ref(bdd);

    std::chrono::steady_clock::time_point iteration_begin;
    std::chrono::steady_clock::time_point iteration_end;
    BDDBuildStatistic statistic(setInfo.clauselAmount, 10);
    for (int i = 0; i < setInfo.formulas.size(); i++) {
        iteration_begin = std::chrono::steady_clock::now();

        DdNode* formula;

        if (setInfo.formulas.at(i).type == Form::AMO) {
            formula = createAMOFormulaFromInfo(gbm, setInfo.formulas.at(i));
        } else {
            formula = createNFFormulaFromInfo(gbm, setInfo.formulas.at(i));
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

        iteration_end = std::chrono::steady_clock::now();

        int stepTime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(iteration_end - iteration_begin).count();
        if (progress_output) {
            statistic.logStep(i+1, Cudd_ReadNodeCount(gbm), stepTime_ms);
        }
    }

    return bdd;
}