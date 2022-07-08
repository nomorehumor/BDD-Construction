//
// Created by Maxim.Popov on 04.07.2022.
//

#ifndef CODE_BDD_FORMULAS_H
#define CODE_BDD_FORMULAS_H

#include <iostream>
#include <cudd.h>
#include <vector>
#include "../utils/file_utils.h"

DdNode* createFormulaFromInfo(DdManager *gbm, FormulaInfo info);
DdNode* createRuleset(DdManager *gbm, FormulaSetInfo setInfo, bool progress_output = false);
std::vector<std::vector<bool>> getMinterms(DdManager* gbm, DdNode* bdd, int numVars, int maxAmount, bool output = false);

#endif //CODE_BDD_FORMULAS_H
