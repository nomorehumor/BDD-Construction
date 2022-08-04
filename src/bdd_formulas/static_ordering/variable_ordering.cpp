//
// Created by Maxim.Popov on 04.08.2022.
//

#include "variable_ordering.h"
#include "FORCEPlacer.h"

std::vector<int> orderVariablesByFrequency(RulesetInfo& setInfo, bool skipFirst, bool countAllAppearances, std::map<int, int> *variableFrequencyMap) {

    std::map<int,int> localVariableFrequencyMap;
    if (variableFrequencyMap == nullptr) {
        variableFrequencyMap = &localVariableFrequencyMap;
    }
    // Collect variables statistics
    for (FormulaInfo &formulaInfo : setInfo.formulas) {
        std::map<int, bool> varAppearance;
        for (int symbol : formulaInfo.symbols) {
            if (symbol != 0) {
                if (!countAllAppearances) {
                    if (varAppearance[symbol])
                        continue;
                    else
                        varAppearance[symbol] = true;
                }
                (*variableFrequencyMap)[std::abs(symbol)]++;
            }
        }
    }

    // For statistics and get most frequent variable
    std::vector<int> varsSorted;
    for (int i = 1; i <= setInfo.variableAmount; i++) {
        varsSorted.push_back(i);
    }
    // Sort formulas
    std::sort(varsSorted.begin(), varsSorted.end(),
              [&](int variable1, int variable2) {
                  return (*variableFrequencyMap)[variable1] >
                         (*variableFrequencyMap)[variable2];
              });

    if (skipFirst) {
        (*variableFrequencyMap)[varsSorted[0]] = 0;
        int firstVar = varsSorted[0];
        varsSorted.erase(varsSorted.begin());
        varsSorted.push_back(firstVar);
    }

    return varsSorted;
}

std::vector<int> orderVariablesFORCE(RulesetInfo setInfo) {
    FORCEPlacer placer(setInfo);
    return placer.orderVariablesWithPlacement(placer.findPlacement(true));
}