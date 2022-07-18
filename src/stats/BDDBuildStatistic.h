//
// Created by Maxim.Popov on 16.07.2022.
//

#ifndef CODE_BDDBUILDSTATISTIC_H
#define CODE_BDDBUILDSTATISTIC_H

#include <vector>
#include "../utils/progress_bar.h"
#include "../utils/file_utils.h"
#include <map>

class BDDBuildStatistic {
public:
    BDDBuildStatistic(int totalItems, int outputInterval);
    void logStep(const FormulaInfo& formula, int itemsDone, int nodeCount, int stepTime_ms);
private:
    int outputInterval;
    int formulaCount;
    ProgressBar progressBar;
    std::vector<int> nodeCountDelta;
    std::vector<int> stepNodeCount;
    std::vector<int> stepTimes_ms;

    std::map<Form, std::vector<int>> formNodeCount;
    std::map<Form, std::vector<int>> formNodeDelta;
    std::map<Form, std::vector<int>> formTimes;
};


#endif //CODE_BDDBUILDSTATISTIC_H
