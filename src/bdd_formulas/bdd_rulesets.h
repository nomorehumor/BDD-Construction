//
// Created by Maxim.Popov on 01.08.2022.
//

#ifndef CODE_BDD_RULESETS_H
#define CODE_BDD_RULESETS_H

#include "../utils/file_utils.h"
#include <cudd.h>

DdNode* createRuleset(DdManager *gbm, RulesetInfo setInfo, bool progress_output = false);
DdNode *createRulesetMerged(DdManager *gbm, RulesetInfo setInfo, int partsAmount, bool progressOutput);

#endif // CODE_BDD_RULESETS_H
