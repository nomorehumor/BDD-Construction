//
// Created by Maxim.Popov on 16.07.2022.
//

#include "BDDBuildStatistic.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

BDDBuildStatistic::BDDBuildStatistic(int totalItems, int outputInterval) {
    this->formulaCount = totalItems;
    this->outputInterval = outputInterval;
    this->progressBar = ProgressBar(totalItems);
}

void BDDBuildStatistic::logStep(int itemsDone, int nodeCount, int stepTime_ms) {
    progressBar.update(itemsDone, nodeCount, stepTime_ms);
    this->stepNodeCount.push_back(nodeCount);
    this->stepTimes_ms.push_back(stepTime_ms);
    if (stepNodeCount.size() == 1) {
        nodeCountDelta.push_back(nodeCount);
    } else {
        nodeCountDelta.push_back(stepNodeCount[stepNodeCount.size()-2] - nodeCount);
    }

    if (itemsDone % outputInterval == 0) {
        plt::stem(nodeCountDelta, stepTimes_ms);
        plt::save("output/nodecount_times.png");
    }

}
