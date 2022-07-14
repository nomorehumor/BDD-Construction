//
// Created by Maxim.Popov on 25.06.2022.
//

#ifndef CODE_PROGRESS_BAR_H
#define CODE_PROGRESS_BAR_H


#include <string>

class ProgressBar {
public:
    ProgressBar(int totalItems);
    void update(int itemsDone, int nodeCount);
private:
    int totalItems;
};


#endif //CODE_PROGRESS_BAR_H
