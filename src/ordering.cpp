#include "ordering.h"
#include "ruleset_ordering/clause_ordering.h"
#include "ruleset_ordering/ruleset_ordering.h"
#include "utils/configuration.h"
#include "variable_ordering/variable_ordering.h"
#include "spdlog/spdlog.h"

RulesetInfo orderRuleset(RulesetInfo &setInfo, std::string strategy) {
    if (strategy != "none") {
        if (strategy == "size") {
            return orderRulesetFormulaSize(
                setInfo, Configuration::isAscending());
        } else if (strategy == "random") {
            return orderRulesetRandom(setInfo);
        } else if (strategy == "var-frequency") {
            return orderRulesetFrequentVariables(
                setInfo, Configuration::isCountAllAppearances(),
                Configuration::isSkipMostFrequentVar());
        } else if (strategy == "force-modified") {
            return orderRulesetModifiedFORCE(setInfo);
        }  else {
            spdlog::warn(
                "Unknown ruleset ordering strategy, using 'none' instead");
        }
    }

    // 'none' strategy
    return setInfo;
}

std::vector<int> orderVariables(RulesetInfo &setInfo, std::string strategy) {
    if (strategy != "none") {
        if (strategy == "var-frequency") {
            return orderVariablesByFrequency(
                setInfo, Configuration::isSkipMostFrequentVar(),
                Configuration::isCountAllAppearances());
        } else if (strategy == "force-modified") {
            return orderVariablesModifiedFORCE(setInfo);
        } else if (strategy == "force") {
            return orderVariablesFORCE(setInfo);
        } else if (strategy == "random") {
            return orderVariablesRandom(setInfo);
        } else {
            spdlog::warn(
                "Unknown variable ordering strategy, using 'none' instead");
        }
    } 
    
    // 'none' strategy
    std::vector<int> orderedVariables;
    orderedVariables.reserve(setInfo.variableAmount);
    spdlog::debug(orderedVariables.size());
    for (int i = 1; i <= setInfo.variableAmount; i++) {
        orderedVariables.push_back(i);
    }
    return orderedVariables;
}
namespace bdd {

RulesetInfo performOrdering(DdManager *gbm, RulesetInfo &setInfo,
                         std::string setStrategy, std::string clauseStrategy,
                         std::string variableStrategy) {
    RulesetInfo reorderedSet = setInfo;

    BDDBuildTimeCounter::orderingStarted();

    spdlog::info("Using '{}' clause static ordering heuristic", clauseStrategy);
    if (clauseStrategy != "none") {
        if (clauseStrategy == "bottom-up") {
            reorderedSet = orderClausesBottomUp(setInfo);
        } else if (clauseStrategy == "force") {
            reorderedSet = orderClausesFORCE(setInfo);
        } else
            spdlog::warn(
                "Unknown clause ordering strategy, using 'none' instead");
    }

    spdlog::info("Using '{}' ruleset static ordering heuristic", setStrategy);
    reorderedSet = orderRuleset(setInfo, setStrategy);

    spdlog::info("Using '{}' variable static ordering heuristic",
                 variableStrategy);
    std::vector<int> orderedVariables = orderVariables(setInfo, variableStrategy);

    std::for_each(orderedVariables.begin(), orderedVariables.end(),
                    [&](int &var) { var--; });
    int *variableOrderingArray = &orderedVariables[0];
    Cudd_ShuffleHeap(gbm, variableOrderingArray);

    BDDBuildTimeCounter::orderingFinished();

    return reorderedSet;
}

}