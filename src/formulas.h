//
// Created by Maxim.Popov on 04.07.2022.
//

#ifndef CODE_FORMULAS_H
#define CODE_FORMULAS_H

#include "cudd.h"
#include "utils/file_utils.h"

DdNode* createFormulaFromInfo(DdManager *gbm, FormulaInfo info);
DdNode* createRuleset(DdManager *gbm, FormulaSetInfo setInfo);

#endif //CODE_FORMULAS_H
