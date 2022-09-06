//
// Created by Maxim.Popov on 04.08.2022.
//

#include "ruleset_ordering.h"
#include "../../utils/configuration.h"
#include "../../utils/info_structs.h"
#include "FORCEPlacer.h"
#include "utils.h"
#include "variable_ordering.h"
#include <map>
#include <random>

RulesetInfo orderRulesetRandom(RulesetInfo &setInfo) {

    std::random_device rd;
    auto rng = std::default_random_engine{rd()};
    std::shuffle(std::begin(setInfo.formulas), std::end(setInfo.formulas), rng);

    return setInfo;
}

RulesetInfo orderRulesetFormulaSize(RulesetInfo &setInfo, bool ascending) {
    std::sort(setInfo.formulas.begin(), setInfo.formulas.end(),
              [&](FormulaInfo info1, FormulaInfo info2) {
                  return ascending
                             ? info1.symbols.size() < info2.symbols.size()
                             : info1.symbols.size() > info2.symbols.size();
              });
    return setInfo;
}

RulesetInfo orderRulesetFrequentVariables(RulesetInfo &setInfo,
                                          bool countAllAppearances,
                                          bool skipFirst) {
    std::map<int, int> formulaIdVarMap;
    std::map<int, int> variableFrequencyMap;

    std::vector<int> variableOrdering = orderVariablesByFrequency(setInfo, skipFirst, countAllAppearances, &variableFrequencyMap);

    // Create formula partition
    for (FormulaInfo &formula : setInfo.formulas) {
        for (int symbol : formula.symbols) {
            if (symbol != 0) {
                if (formulaIdVarMap.find(formula.id) == formulaIdVarMap.end() ||
                    variableFrequencyMap[formulaIdVarMap[formula.id]] <
                        variableFrequencyMap[std::abs(symbol)]) {
                    formulaIdVarMap[formula.id] = std::abs(symbol);
                }
            }
        }
    }

    printVariableFrequencyStats(variableOrdering, variableFrequencyMap);
    if (Configuration::isOutputPlots()) {
        plotVariableFrequencyStats(variableOrdering, variableFrequencyMap);
        //        plotFormulaPartition(formulaIdVarMap, setInfo.variableAmount);
    }

    std::vector<FormulaInfo> formulas = setInfo.formulas;
    std::sort(formulas.begin(), formulas.end(),
              [&](FormulaInfo &a, FormulaInfo &b) {
                  return std::find(variableOrdering.begin(), variableOrdering.end(), formulaIdVarMap[a.id]) <
                         std::find(variableOrdering.begin(), variableOrdering.end(), formulaIdVarMap[b.id]);
              });

    RulesetInfo newSetInfo = setInfo;
    newSetInfo.formulas = formulas;
    return newSetInfo;
}

RulesetInfo orderRulesetModifiedFORCE(RulesetInfo setInfo) {
    FORCEPlacer placer;
    placer.initGraphWithRulesetModifiedFORCE(setInfo);
    setInfo.formulas = placer.orderFormulasWithPlacement(
        setInfo.formulas, placer.findPlacement(true));
    return setInfo;
}
