//
// Created by Maxim.Popov on 04.08.2022.
//

#ifndef CODE_VARIABLE_ORDERING_H
#define CODE_VARIABLE_ORDERING_H

#include "../../utils/file_utils.h"
#include <vector>
#include <map>

std::vector<int> orderVariablesByFrequency(RulesetInfo& setInfo, bool skipFirst, bool countAllAppearances, std::map<int, int> *variableFrequencyMap = nullptr);
std::vector<int> orderVariablesModifiedFORCE(RulesetInfo setInfo);
std::vector<int> orderVariablesFORCE(RulesetInfo info);

#endif // CODE_VARIABLE_ORDERING_H
