//
// Created by Maxim.Popov on 16.07.2022.
//

#ifndef CODE_BDDBUILDSTATISTIC_H
#define CODE_BDDBUILDSTATISTIC_H

#include <vector>
#include "../utils/progress_bar.h"

class BDDBuildStatistic {
public:
    BDDBuildStatistic(int totalItems, int outputInterval);
    void logStep(int itemsDone, int nodeCount, int stepTime_ms);
private:
    int outputInterval;
    int formulaCount;
    ProgressBar progressBar;
    std::vector<int> nodeCountDelta;
    std::vector<int> stepNodeCount;
    std::vector<int> stepTimes_ms;
};


#endif //CODE_BDDBUILDSTATISTIC_H
