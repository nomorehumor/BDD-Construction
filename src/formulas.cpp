//
// Created by Maxim.Popov on 04.07.2022.
//

#include "cudd.h"
#include "utils/file_utils.h"
#include "progress_bar.h"

DdNode* createFormulaFromInfo(DdManager *gbm, FormulaInfo info) {
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

DdNode* createRuleset(DdManager *gbm, FormulaSetInfo setInfo) {
    DdNode *tmp, *bdd;
    if (setInfo.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
    } else if (setInfo.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
    }
    Cudd_Ref(bdd);

    ProgressBar bar(setInfo.clauselAmount);
    for (int i = 0; i < setInfo.formulas.size(); i++) {
        DdNode* formula = createFormulaFromInfo(gbm, setInfo.formulas.at(i));

        if (setInfo.type == Form::CNF) {
            tmp = Cudd_bddAnd(gbm, formula, bdd);
        } else if (setInfo.type == Form::DNF) {
            tmp = Cudd_bddOr(gbm, formula, bdd);
        }
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, formula);
        Cudd_RecursiveDeref(gbm, bdd);
        bdd = tmp;

        i++;
        bar.update(i );
    }

    return bdd;
}