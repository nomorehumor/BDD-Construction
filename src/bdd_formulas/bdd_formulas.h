//
// Created by Maxim.Popov on 04.07.2022.
//

#ifndef CODE_BDD_FORMULAS_H
#define CODE_BDD_FORMULAS_H

#include <iostream>
#include <cudd.h>
#include "utils/file_utils.h"

DdNode* createFormulaFromInfo(DdManager *gbm, FormulaInfo info);
DdNode* createRuleset(DdManager *gbm, FormulaSetInfo setInfo, bool progress_output = false);

#endif //CODE_BDD_FORMULAS_H
