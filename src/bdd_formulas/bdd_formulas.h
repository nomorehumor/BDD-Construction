//
// Created by Maxim.Popov on 04.07.2022.
//

#ifndef CODE_BDD_FORMULAS_H
#define CODE_BDD_FORMULAS_H

#include <iostream>
#include <cudd.h>
#include <vector>
#include "../utils/file_utils.h"

std::vector<std::vector<bool>> getMinterms(DdManager* gbm, DdNode* bdd, int numVars, int maxAmount, bool output = false);
DdNode* createNFFormulaFromInfo(DdManager *gbm, FormulaInfo info);
DdNode* createRuleset(DdManager *gbm, RulesetInfo setInfo, bool progress_output = false);
DdNode* createNFFormulaFromClauses(DdManager *gbm, FormulaInfo info);
DdNode* createNFFormulaMerge(DdManager *gbm, FormulaInfo info);

#endif //CODE_BDD_FORMULAS_H
