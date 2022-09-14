#ifndef ORDERING_H
#define ORDERING_H

#include "utils/file_utils.h"
#include "stats/BDDBuildTimeCounter.h"
#include <cudd.h>

std::vector<int> orderVariables(RulesetInfo &setInfo, std::string strategy);
RulesetInfo orderRuleset(RulesetInfo &setInfo, std::string strategy);

namespace bdd {
    RulesetInfo performOrdering(DdManager *gbm, RulesetInfo &setInfo,
                            std::string setStrategy, std::string clauseStrategy,
                            std::string variableStrategy);
}

#endif