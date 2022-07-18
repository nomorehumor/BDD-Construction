//
// Created by Maxim.Popov on 11.07.2022.
//
#include "formula_ordering.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <random>
#include <map>
#include <iostream>
#include "matplotlibcpp.h"


void printVariableFrequencyStats(std::vector<int> orderedVariables, std::map<int,int> variableFrequencies) {
    int totalAppears = 0;
    for (auto varPair : variableFrequencies) {
        totalAppears += varPair.second;
    }
    SPDLOG_INFO("======= Variable stats =======");
    for (int i = 0; i < std::min(10, (int) orderedVariables.size()); i++) {
        double percentage = ((double) variableFrequencies[orderedVariables[i]]) / ((double) totalAppears);
        SPDLOG_INFO("{} | frequency {} : [{}%]", orderedVariables[i], variableFrequencies[orderedVariables[i]], percentage);
    }
}

void plotVariableFrequencyStats(std::vector<int> orderedVariables, std::map<int, int> variableFrequencies) {
    std::vector<int> frequencies;
    std::vector<int> top20Frequencies;
    for (int i = 0; i < orderedVariables.size(); i++) {
        frequencies.push_back(variableFrequencies[orderedVariables[i]]);
        if (i < 20) top20Frequencies.push_back(variableFrequencies[orderedVariables[i]]);
    }
    matplotlibcpp::plot(frequencies);
    matplotlibcpp::ylabel("Frequency");
    matplotlibcpp::xlabel("Position in ordered list");
    matplotlibcpp::save("output/frequencies.png");
    matplotlibcpp::clf();

    matplotlibcpp::plot(top20Frequencies);
    matplotlibcpp::ylabel("Frequency");
    matplotlibcpp::xlabel("Position in ordered list");
    matplotlibcpp::save("output/top_20_Frequencies.png");
    matplotlibcpp::clf();
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
    // Sort formulas
    std::sort(varsSorted.begin(), varsSorted.end(), [&](int variable1, int variable2) { return variableFrequencyMap[variable1] > variableFrequencyMap[variable2];});

    if (skipFirst) {
        variableFrequencyMap[varsSorted[0]] = 0;
        int firstVar = varsSorted[0];
        varsSorted.erase(varsSorted.begin());
        varsSorted.push_back(firstVar);
    }

    printVariableFrequencyStats(varsSorted, variableFrequencyMap);
    plotVariableFrequencyStats(varsSorted, variableFrequencyMap);

    std::vector<FormulaInfo> formulas = setInfo.formulas;
    std::sort(formulas.begin(), formulas.end(), [&] (FormulaInfo& a, FormulaInfo& b) {
        return variableFrequencyMap[formulaIdVarMap[a.id]] > variableFrequencyMap[formulaIdVarMap[b.id]];
    });

    RulesetInfo newSetInfo = setInfo;
    newSetInfo.formulas = formulas;
    return newSetInfo;
}

RulesetInfo orderRulesetSimilarVariables(RulesetInfo& setInfo) {
    return setInfo;
}