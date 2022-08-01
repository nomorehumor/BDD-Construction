//
// Created by Maxim.Popov on 04.07.2022.
//

#ifndef CODE_BDD_FORMULAS_H
#define CODE_BDD_FORMULAS_H

#include <iostream>
#include <cudd.h>
#include <vector>
#include "../utils/file_utils.h"

DdNode* createNFFormulaFromInfo(DdManager *gbm, FormulaInfo info);
DdNode* createNFFormulaFromClauses(DdManager *gbm, FormulaInfo info);
DdNode* createNFFormulaMerge(DdManager *gbm, FormulaInfo info);
DdNode* createAMOFormulaFromInfo(DdManager *gbm, FormulaInfo amoInfo);

DdNode* createFormulaBdd(DdManager *gbm, FormulaInfo info);

#endif //CODE_BDD_FORMULAS_H
