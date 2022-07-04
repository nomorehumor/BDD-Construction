//
// Created by Maxim.Popov on 23.06.2022.
//

#ifndef CODE_OUTPUT_UTILS_H
#define CODE_OUTPUT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"


void print_dd (DdManager *gbm, DdNode *dd, int n = 4, int pr = 4);
void write_dd (DdManager *gbm, DdNode *dd, char* filename);

#endif //CODE_OUTPUT_UTILS_H
