//
// Created by Maxim.Popov on 23.06.2022.
//

#include "cudd.h"
#include "utils/output_utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

int tutorial() {

    char filename[30];
    DdManager *gbm; /* Global BDD manager. */
    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS,
                    0); /* Initialize a new BDD manager. */
    DdNode *bdd, *var, *tmp_neg, *tmp;
    int i;
    bdd = Cudd_ReadOne(gbm); /*Returns the logic one constant of the manager*/
    Cudd_Ref(bdd);           /*Increases the reference count of a node*/

    for (i = 3; i >= 0; i--) {
        var = Cudd_bddIthVar(gbm, i);         /*Create a new BDD variable*/
        tmp_neg = Cudd_Not(var);              /*Perform NOT Boolean operation*/
        tmp = Cudd_bddAnd(gbm, tmp_neg, bdd); /*Perform AND Boolean operation*/
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, bdd);
        bdd = tmp;
    }

    bdd = Cudd_BddToAdd(gbm, bdd); /*Convert BDD to ADD for display purpose*/
    print_dd(gbm, bdd, 2, 4);      /*Print the dd to standard output*/
    sprintf(filename, "tutorial.dot"); /*Write .dot filename to a string*/
    write_dd(gbm, bdd, filename); /*Write the resulting cascade dd to a file*/
    Cudd_Quit(gbm);
    return 0;
}
