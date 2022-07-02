/*
* FILENAME: transfer.c
* Overview: BDD tutorial
* AUTHOR: David Kebo Houngninou
*/

#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"
#include <vector>
#include <iostream>
#include "utils/file_utils.h"
#include "tutorial.h"
#include "utils/output_utils.h"
#include "progress_bar.h"


DdNode* createFormulaFromInfo(DdManager *gbm, FormulaInfo info) {
    DdNode *lastVariables, *tmpLastVariables, *tmpVar, *tmpVar2, *bdd;
    lastVariables = NULL;
    if (info.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
        lastVariables = Cudd_ReadOne(gbm);
        Cudd_Ref(lastVariables);
    } else if (info.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
        lastVariables = Cudd_ReadLogicZero(gbm);
        Cudd_Ref(lastVariables);
    }

    for (int i = 0; i < info.symbols.size(); i++) {
        if (info.symbols.at(i) == 0 && info.symbols.at(i - 1) != 0) {
            if (info.type == Form::DNF) {
                tmpVar = Cudd_bddOr(gbm, lastVariables, bdd);
                Cudd_RecursiveDeref(gbm,lastVariables);
                lastVariables = Cudd_ReadOne(gbm);
                Cudd_Ref(lastVariables);
            } else if (info.type == Form::CNF) {
                tmpVar = Cudd_bddAnd(gbm, lastVariables, bdd);
                Cudd_RecursiveDeref(gbm,lastVariables);
                lastVariables = Cudd_ReadLogicZero(gbm);
                Cudd_Ref(lastVariables);
            }
            Cudd_Ref(tmpVar);
            Cudd_RecursiveDeref(gbm,bdd);
            bdd = tmpVar;
        } else if (info.symbols.at(i) != 0){
            tmpVar = Cudd_bddIthVar(gbm, std::abs(info.symbols.at(i)));
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

    return bdd;
}



std::vector<DdNode*> createVariables(DdManager *gbm, int variablesAmount) {
    DdNode* var;
    std::vector<DdNode*> vars;
    for (int i = 0; i < variablesAmount; i++) {
        var = Cudd_bddNewVar(gbm); /*Create a new BDD variable*/
        Cudd_Ref(var);
        vars.push_back(var);
    }
    return vars;
}

DdNode* createRulesetFromFile(DdManager *gbm, FormulaSetInfo setInfo, FILE* file) {
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t length = 0;

    DdNode *tmp, *bdd;
    if (setInfo.type == Form::CNF) {
        bdd = Cudd_ReadOne(gbm);
    } else if (setInfo.type == Form::DNF) {
        bdd = Cudd_ReadLogicZero(gbm);
    }
    Cudd_Ref(bdd);

    size_t read;
    int i = 0;
    progress_bar bar(setInfo.clauselAmount);
    while (read = getline(&line, &length, file) != -1) {
        FormulaInfo clauselInfo = getClauselInfoFromLine(line);
        DdNode* formula = createFormulaFromInfo(gbm, clauselInfo);

//        char filename[30];
//        sprintf(filename, "formula.dot");
//        write_dd(gbm, formula, filename);
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

//        char out_filename[30];
//        sprintf(out_filename, "bdd.dot");
//        write_dd(gbm, bdd, out_filename);
    }

    return bdd;
}

// This program creates a single BDD variable
int main (int argc, char *argv[])
{
//    char* filename = "C:\\dev\\Bachelor\\merlin-formulas\\wingas\\merlin-clauseset420509740828757424.txt";
    char* filename = "C:\\dev\\Bachelor\\test-formulas\\test1.txt";
    FILE* file = fopen(filename, "r");
    FormulaSetInfo info = readClauselSetInfo(file);

    DdManager *gbm; /* Global BDD manager. */
    gbm = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0); /* Initialize a new BDD manager. */
    Cudd_AutodynEnable(gbm, CUDD_REORDER_SIFT);

    DdNode* bdd = createRulesetFromFile(gbm, info, file);
    print_dd(gbm, bdd, 4, 4);
    Cudd_RecursiveDeref(gbm, bdd);


    int num_reference_nodes = Cudd_CheckZeroRef(gbm);
    std::cout << "#Nodes with non-zero reference count (should be 0): " << num_reference_nodes;


//    char out_filename[30];
//    sprintf(out_filename, "bdd.dot");
//    bdd = Cudd_BddToAdd(gbm, bdd);
//    write_dd(gbm, bdd, out_filename);

//    tutorial();
    fclose(file);
}