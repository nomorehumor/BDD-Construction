//
// Created by Maxim.Popov on 04.08.2022.
//

#ifndef CODE_RULESET_ORDERING_H
#define CODE_RULESET_ORDERING_H

#include "../utils/file_utils.h"

RulesetInfo orderRulesetRandom(RulesetInfo& setInfo);
RulesetInfo orderRulesetFormulaSize(RulesetInfo& setInfo, bool ascending);
RulesetInfo orderRulesetFrequentVariables(RulesetInfo& setInfo, bool countAllAppearances, bool skipFirst);
RulesetInfo orderRulesetModifiedFORCE(RulesetInfo setInfo);
RulesetInfo orderRulesetFORCE(RulesetInfo setInfo);

#endif // CODE_RULESET_ORDERING_H
