//
// Created by Maxim.Popov on 11.07.2022.
//
#include "formula_ordering.h"
#include <algorithm>
#include <random>
#include <map>
#include <iostream>


void printVariableFrequencyStats(std::vector<int> orderedVariables, std::map<int,int> variableFrequencies) {
    int totalAppears = 0;
    for (auto varPair : variableFrequencies) {
        totalAppears += varPair.second;
    }
    for (int i = 0; i < std::min(10, (int) orderedVariables.size()); i++) {
        double percentage = ((double) variableFrequencies[orderedVariables[i]]) / ((double) totalAppears);
        std::cout << "Variable " << orderedVariables[i] << " | frequency: " << variableFrequencies[orderedVariables[i]] << " [" << percentage << "%]" << std::endl;
    }
}

RulesetInfo orderRulesetRandom(RulesetInfo& setInfo) {

    std::random_device rd;
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(setInfo.formulas), std::end(setInfo.formulas), rng);

    return setInfo;
}

RulesetInfo orderRulesetFormulaSize(RulesetInfo& setInfo, bool ascending) {
    std::sort(setInfo.formulas.begin(), setInfo.formulas.end(),
              [&](FormulaInfo info1, FormulaInfo info2) {return ascending ? info1.symbols.size() < info2.symbols.size()
              : info1.symbols.size() > info2.symbols.size();});
    return setInfo;
}

RulesetInfo orderRulesetFrequentVariables(RulesetInfo& setInfo, bool skipFirst) {
    std::map<int, int> variableFrequencyMap;
    std::map<int, int> formulaIdVarMap;
    std::map<int, std::vector<FormulaInfo>> formulaPartition;

    // Collect variables statistics
    for (FormulaInfo& formulaInfo : setInfo.formulas) {
        for (int symbol : formulaInfo.symbols) {
            if (symbol != 0) {
                variableFrequencyMap[std::abs(symbol)]++;
            }
        }
    }

    // Create formula partition
    for (FormulaInfo& formula : setInfo.formulas) {
        for (int symbol : formula.symbols) {
            if (symbol != 0) {
                if (formulaIdVarMap.find(formula.id) == formulaIdVarMap.end() || variableFrequencyMap[formulaIdVarMap[formula.id]] < variableFrequencyMap[std::abs(symbol)]) {
                    formulaIdVarMap[formula.id] = std::abs(symbol);
                }
            }
        }
    }

    // For statistics and get most frequent variable
    std::vector<int> varsSorted;
    for (int i = 1; i <= setInfo.variableAmount; i++) {
        varsSorted.push_back(i);
    }
    std::sort(varsSorted.begin(), varsSorted.end(), [&](int variable1, int variable2) { return variableFrequencyMap[variable1] > variableFrequencyMap[variable2];});
    printVariableFrequencyStats(varsSorted, variableFrequencyMap);


    // Sort formulas
    if (skipFirst) {
        variableFrequencyMap[varsSorted[0]] = 0;
    }
    std::vector<FormulaInfo> formulas = setInfo.formulas;
    std::sort(formulas.begin(), formulas.end(), [&] (FormulaInfo& a, FormulaInfo& b) {
        return variableFrequencyMap[formulaIdVarMap[a.id]] > variableFrequencyMap[formulaIdVarMap[b.id]];
    });

    RulesetInfo newSetInfo = setInfo;
    newSetInfo.formulas = formulas;
    return newSetInfo;
}

RulesetInfo orderRulesetSimilarVariables(RulesetInfo& setInfo) {

}