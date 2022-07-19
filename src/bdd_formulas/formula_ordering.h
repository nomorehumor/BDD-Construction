//
// Created by Maxim.Popov on 11.07.2022.
//

#ifndef CODE_FORMULA_ORDERING_H
#define CODE_FORMULA_ORDERING_H

#include "../utils/file_utils.h"

RulesetInfo orderRulesetRandom(RulesetInfo& setInfo);
RulesetInfo orderRulesetFormulaSize(RulesetInfo& setInfo, bool ascending);
RulesetInfo orderRulesetFrequentVariables(RulesetInfo& setInfo, bool skipFirst);
RulesetInfo orderRuleset(RulesetInfo& setInfo, std::string strategy);

#endif //CODE_FORMULA_ORDERING_H
