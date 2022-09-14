//
// Created by Maxim.Popov on 04.08.2022.
//

#ifndef CODE_UTILS_H
#define CODE_UTILS_H

#include "matplotlibcpp.h"
#include "spdlog/spdlog.h"


namespace plt = matplotlibcpp;

void printVariableFrequencyStats(std::vector<int> &orderedVariables,
                                 std::map<int, int> &variableFrequencies) {
    int totalAppears = 0;
    for (auto varPair : variableFrequencies) {
        totalAppears += varPair.second;
    }
    SPDLOG_INFO("======= Variable stats =======");
    for (int i = 0; i < std::min(10, (int)orderedVariables.size()); i++) {
        double percentage = ((double)variableFrequencies[orderedVariables[i]]) /
                            ((double)totalAppears);
        SPDLOG_INFO("{} | frequency {} : [{}%]", orderedVariables[i],
                    variableFrequencies[orderedVariables[i]], percentage);
    }
}

void plotVariableFrequencyStats(std::vector<int> &orderedVariables,
                                std::map<int, int> &variableFrequencies) {
    spdlog::info("Plotting variable frequency stats");
    std::vector<int> frequencies;
    std::vector<int> top20Frequencies;
    for (int i = 0; i < orderedVariables.size(); i++) {
        frequencies.push_back(variableFrequencies[orderedVariables[i]]);
        if (i < 20)
            top20Frequencies.push_back(
                variableFrequencies[orderedVariables[i]]);
    }

    plt::figure_size(2500, 780);
    plt::subplot(1, 2, 1);
    plt::bar(frequencies);
    plt::title("All variables");
    plt::ylabel("Frequency");
    plt::xlabel("Position in ordered list");

    plt::subplot(1, 2, 2);
    plt::bar(top20Frequencies);
    plt::ylabel("Frequency");
    plt::xlabel("Position in ordered list");
    plt::title("Top 20 variables");
    plt::save("output/frequencies.png");
    plt::clf();
    plt::figure_size(1200, 780);
}

void plotFormulaPartition(std::map<int, int> formulaIdVarMap, int varAmount) {
    spdlog::info("Plotting formula partition");
    std::vector<int> varValues;
    for (auto formulaIdVar : formulaIdVarMap) {
        varValues.push_back(formulaIdVar.second);
    }

    plt::figure_size(2000, 780);
    plt::hist(varValues, varAmount);
    plt::ylim(0, 50);
    plt::save("output/formula_partition.png");
    plt::clf();

    plt::figure_size(1200, 780);
}



#endif // CODE_UTILS_H
