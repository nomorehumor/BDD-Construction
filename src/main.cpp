/*
* FILENAME: transfer.c
* Overview: BDD tutorial
* AUTHOR: David Kebo Houngninou
*/

#include <iostream>
#include "bdd_formulas/bdd_formulas.h"
#include "utils/output_utils.h"
#include "bdd_formulas/formula_ordering.h"



// This program creates a single BDD variable
int main (int argc, char *argv[])
{

    char* filename = "C:\\dev\\Bachelor\\merlin-formulas\\wingas\\merlin-clauseset420509740828757424.txt";
//    char* filename = "C:\\dev\\Bachelor\\test-formulas\\test_amo.txt";
    RulesetInfo info = readClauselSetInfo(filename);

    info = orderRulesetFrequentVariables(info, true);

    DdManager *gbm; /* Global BDD manager. */
    gbm = Cudd_Init(info.variableAmount,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0); /* Initialize a new BDD manager. */
    Cudd_AutodynEnable(gbm, CUDD_REORDER_SIFT);

    DdNode* bdd = createRuleset(gbm, info, true);
    print_dd(gbm, bdd);
    std::cout << "Minterm count: " << Cudd_CountMinterm(gbm, bdd, info.variableAmount) << std::endl;

    char out_filename[30];
    sprintf(out_filename, "bdd.dot");
//    bdd = Cudd_BddToAdd(gbm, bdd);
    write_dd(gbm, bdd, out_filename);

    Cudd_RecursiveDeref(gbm, bdd);

    int num_reference_nodes = Cudd_CheckZeroRef(gbm);
    std::cout << "#Nodes with non-zero reference count (should be 0): " << num_reference_nodes;

}