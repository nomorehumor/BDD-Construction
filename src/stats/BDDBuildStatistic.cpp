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
        plt::clf();

        plt::stem(nodeCountDelta, stepTimes_ms);
        plt::save("output/nodecountdelta_times.png");
        plt::clf();

        plt::stem(stepNodeCount, stepTimes_ms);
        plt::save("output/nodecount_times.png");
        plt::clf();

        plt::figure(1);
        plt::stem(formNodeDelta[Form::AMO], formTimes[Form::AMO]);
        plt::suptitle("AMO node count delta to step time");
        plt::figure(2);
        plt::stem(formNodeDelta[Form::CNF], formTimes[Form::CNF]);
        plt::suptitle("CNF node count delta to step time");
        plt::figure(3);
        plt::stem(formNodeDelta[Form::DNF], formTimes[Form::DNF]);
        plt::suptitle("DNF node count delta to step time");

        plt::legend();
        plt::save("output/nodecount_times_type_comparison.png");
        plt::clf();
    }

}
