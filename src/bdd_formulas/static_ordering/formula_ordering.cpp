//
// Created by Maxim.Popov on 11.07.2022.
//
#include "formula_ordering.h"
#include "../../utils/BDDConfiguration.h"
#include "../../utils/progress_bar.h"
#include "FORCEPlacer.h"
#include "matplotlibcpp.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <random>

namespace plt = matplotlibcpp;

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
    spdlog::info("Plotting variable frequency stats");
    std::vector<int> frequencies;
    std::vector<int> top20Frequencies;
    for (int i = 0; i < orderedVariables.size(); i++) {
        frequencies.push_back(variableFrequencies[orderedVariables[i]]);
        if (i < 20) top20Frequencies.push_back(variableFrequencies[orderedVariables[i]]);
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

RulesetInfo orderRulesetFrequentVariables(RulesetInfo& setInfo, bool countAllAppearances, bool skipFirst) {
    std::map<int, int> variableFrequencyMap;
    std::map<int, int> formulaIdVarMap;
//    std::map<int, std::vector<FormulaInfo>> formulaPartition;

    // Collect variables statistics
    for (FormulaInfo& formulaInfo : setInfo.formulas) {
        std::map<int, bool> varAppearance;
        for (int symbol : formulaInfo.symbols) {
            if (symbol != 0) {
                if (!countAllAppearances) {
                    if (varAppearance[symbol]) continue;
                    else varAppearance[symbol] = true;
                }
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
    if (BDDConfiguration::isOutputPlots()) {
        plotVariableFrequencyStats(varsSorted, variableFrequencyMap);
//        plotFormulaPartition(formulaIdVarMap, setInfo.variableAmount);
    }

    std::vector<FormulaInfo> formulas = setInfo.formulas;
    std::sort(formulas.begin(), formulas.end(), [&] (FormulaInfo& a, FormulaInfo& b) {
        return variableFrequencyMap[formulaIdVarMap[a.id]] > variableFrequencyMap[formulaIdVarMap[b.id]];
    });

    RulesetInfo newSetInfo = setInfo;
    newSetInfo.formulas = formulas;
    return newSetInfo;
}

RulesetInfo orderRulesetFORCE(RulesetInfo setInfo) {
    FORCEPlacer placer(setInfo.formulas);
    setInfo.formulas = placer.orderFormulasWithPlacement(setInfo.formulas, placer.findPlacement(true));
    return setInfo;
}

RulesetInfo orderClausesBottomUp(RulesetInfo& setInfo) {
    RulesetInfo orderedRuleset = setInfo;
    orderedRuleset.formulas = {};

    for (FormulaInfo formula : setInfo.formulas) {
        std::vector<int> newSymbols;
        auto firstInClause = formula.symbols.begin();
        for (auto it = formula.symbols.begin() + 1; it < formula.symbols.end(); it++) {
            if (*it == 0 && *firstInClause != 0) {
                std::vector<int> clause(firstInClause, it);
                std::sort(clause.begin(), clause.end(), [](int a, int b) { return std::abs(a) > std::abs(b); });
                newSymbols.insert(newSymbols.end(), clause.begin(), clause.end());
                newSymbols.push_back(0);
                firstInClause = it + 1;
            }
        }

        if (formula.type == Form::DNF) {
            newSymbols.push_back(0);
        }

        FormulaInfo newFormula = formula;
        newFormula.symbols = newSymbols;
        orderedRuleset.formulas.push_back(newFormula);
    }

    return orderedRuleset;
}

RulesetInfo orderClausesFORCE(RulesetInfo setInfo) {
    std::vector<FormulaInfo> formulas;

    ProgressBar bar(setInfo.formulas.size());
    int i = 1;
    spdlog::info("Ordering clauses with FORCE");
    for (FormulaInfo& formula : setInfo.formulas) {
        bar.update(i);
        if (formula.clauses.size() > 2) {
            FORCEPlacer placer(formula);
            formulas.push_back(placer.orderClausesWithPlacement(formula, placer.findPlacement()));
        } else {
            formulas.push_back(formula);
        }

        i++;
    }
    setInfo.formulas = formulas;
    return setInfo;
}

RulesetInfo orderRuleset(RulesetInfo& setInfo, std::string setStrategy, std::string clauseStrategy) {
    RulesetInfo inputSet = setInfo;

    spdlog::info("Using '{}' topological ordering strategy", BDDConfiguration::getTopologicalOrdering());
    spdlog::info("Using '{}' clause ordering strategy", clauseStrategy);
    if (clauseStrategy == "bottom_up") {
        inputSet = orderClausesBottomUp(setInfo);
    } else if (clauseStrategy == "force") {
        inputSet = orderClausesFORCE(setInfo);
    } else spdlog::info("Clause ordering is disabled");

    spdlog::info("Using '{}' static ordering strategy", setStrategy);
    if (setStrategy == "none") {
        return inputSet;
    } else if (setStrategy == "size") {
        return orderRulesetFormulaSize(inputSet,
                                       BDDConfiguration::isAscending());
    } else if (setStrategy == "random") {
        return orderRulesetRandom(inputSet);
    } else if (setStrategy == "var_frequency") {
        return orderRulesetFrequentVariables(
            inputSet, BDDConfiguration::isCountAllAppearances(),
            BDDConfiguration::isSkipMostFrequentVar());
    } else if (setStrategy == "force") {
        return orderRulesetFORCE(inputSet);
    } else {
        spdlog::warn("Unknown strategy, using 'none' instead");
        return inputSet;
    }
}