//
// Created by Maxim.Popov on 16.07.2022.
//

#include "BDDBuildStatistic.h"
#include "../utils/BDDConfiguration.h"
#include "matplotlibcpp.h"
#include "spdlog/spdlog.h"

namespace plt = matplotlibcpp;

BDDBuildStatistic::BDDBuildStatistic(int totalItems, int outputInterval) {
    this->outputInterval = outputInterval;
    this->progressBar = ProgressBar(totalItems);
}

void BDDBuildStatistic::logCudd(DdManager *gbm, int itemsDone) {
    memorySize.push_back(Cudd_ReadMemoryInUse(gbm));

    if (itemsDone % outputInterval == 0 && BDDConfiguration::isOutputPlots()) {
        plt::plot(memorySize);
        plt::title("Used memory (in bytes)");
        plt::save(BDDConfiguration::getOutputDirectory() + "/memory.png");
        plt::clf();
    }
}

void BDDBuildStatistic::logTime(int itemsDone, double totalTime_s) {
    if (itemsDone % outputInterval == 0) {
        double averageStepTime = ((double) std::reduce(stepTimes_ms.begin(), stepTimes_ms.end())) / stepTimes_ms.size();
        spdlog::info("Current elapsed time: {}s | Average step time: {:.3f}ms", totalTime_s, averageStepTime);
    }
}

void BDDBuildStatistic::logStep(const FormulaInfo& formula, int itemsDone, int nodeCount, int stepTime_ms) {
    progressBar.update(itemsDone, fmt::format("Node count: {} | Step time: {}ms", nodeCount, stepTime_ms));

    int delta = stepNodeCount.empty() ? nodeCount : nodeCount - stepNodeCount.at(stepNodeCount.size()-1);

    this->stepNodeCount.push_back(nodeCount);
    this->stepTimes_ms.push_back(stepTime_ms);
    this->nodeCountDelta.push_back(delta);

    formNodeCount[formula.type].push_back(nodeCount);
    formTimes[formula.type].push_back(stepTime_ms);
    formNodeDelta[formula.type].push_back(delta);

    if (itemsDone % outputInterval == 0 && BDDConfiguration::isOutputPlots()) {
        plt::figure_size(1200, 780);
        plt::plot(stepTimes_ms);
        plt::save(BDDConfiguration::getOutputDirectory() + "/step_times.png");
        plt::clf();

        plt::stem(nodeCountDelta, stepTimes_ms);
        plt::save(BDDConfiguration::getOutputDirectory() + "/nodecountdelta_times.png");
        plt::clf();

        plt::plot(stepNodeCount);
        plt::save(BDDConfiguration::getOutputDirectory() + "/nodecount.png");
        plt::clf();

        plt::stem(stepNodeCount, stepTimes_ms);
        plt::save(BDDConfiguration::getOutputDirectory() + "/nodecount_times.png");
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
            plt::save(BDDConfiguration::getOutputDirectory() + "/nodecountdelta_times_type_comparison.png");
        }
        plt::close();
        plt::cla();
        plt::clf();
        plt::figure_size(1200, 780);
    }

}
