//
// Created by Maxim.Popov on 16.07.2022.
//

#include "BDDBuildStatistic.h"
#include "matplotlibcpp.h"
#include "../utils/BDDConfiguration.h"

namespace plt = matplotlibcpp;

BDDBuildStatistic::BDDBuildStatistic(int totalItems, int outputInterval) {
    this->formulaCount = totalItems;
    this->outputInterval = outputInterval;
    this->progressBar = ProgressBar(totalItems);
}

void BDDBuildStatistic::logCudd(DdManager *gbm, int itemsDone) {
    memorySize.push_back(Cudd_ReadMemoryInUse(gbm));

    if (itemsDone % outputInterval == 0 && BDDConfiguration::getOutputPlots()) {
        plt::plot(memorySize);
        plt::title("Used memory (in bytes)");
        plt::save("output/memory.png");
        plt::clf();
    }
}

void BDDBuildStatistic::logStep(const FormulaInfo& formula, int itemsDone, int nodeCount, int stepTime_ms) {
    progressBar.update(itemsDone, nodeCount, stepTime_ms);

    int delta = stepNodeCount.empty() ? nodeCount : nodeCount - stepNodeCount.at(stepNodeCount.size()-1);

    this->stepNodeCount.push_back(nodeCount);
    this->stepTimes_ms.push_back(stepTime_ms);
    this->nodeCountDelta.push_back(delta);

    formNodeCount[formula.type].push_back(nodeCount);
    formTimes[formula.type].push_back(stepTime_ms);
    formNodeDelta[formula.type].push_back(delta);

    if (itemsDone % outputInterval == 0 && BDDConfiguration::getOutputPlots()) {

        plt::figure_size(1200, 780);
        plt::plot(stepTimes_ms);
        plt::save("output/step_times.png");
        plt::clf();

        plt::stem(nodeCountDelta, stepTimes_ms);
        plt::save("output/nodecountdelta_times.png");
        plt::clf();

        plt::stem(stepNodeCount, stepTimes_ms);
        plt::save("output/nodecount_times.png");
        plt::clf();

        plt::figure_size(2000, 780);
        int subplotAmount = !formNodeDelta[Form::AMO].empty() + !formNodeDelta[Form::CNF].empty() + !formNodeDelta[Form::DNF].empty();
        int currentSubplotAmount = 1;
        if (!formNodeDelta[Form::AMO].empty()) {
            plt::subplot(1, subplotAmount, currentSubplotAmount);
            plt::stem(formNodeDelta[Form::AMO], formTimes[Form::AMO]);
            plt::title("AMO");
            currentSubplotAmount++;
        }

        if (!formNodeDelta[Form::CNF].empty()) {
            plt::subplot(1, subplotAmount, currentSubplotAmount);
            plt::stem(formNodeDelta[Form::CNF], formTimes[Form::CNF]);
            plt::title("CNF");
            currentSubplotAmount++;
        }

        if (!formNodeDelta[Form::DNF].empty()) {
            plt::subplot(1, subplotAmount, currentSubplotAmount);
            plt::stem(formNodeDelta[Form::DNF], formTimes[Form::DNF]);
            plt::title("DNF");
            currentSubplotAmount++;
        }
        if (currentSubplotAmount != 1) {
            plt::legend();
            plt::save("output/nodecountdelta_times_type_comparison.png");
        }
        plt::clf();
        plt::figure_size(1200, 780);
    }

}
