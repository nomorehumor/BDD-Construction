//
// Created by Maxim.Popov on 25.06.2022.
//

#include "ProgressBar.h"
#include <iostream>

ProgressBar::ProgressBar(int totalItems): totalItems(totalItems) {}

void ProgressBar::update(int itemsDone) {
    int barWidth = 70;
    int progress = itemsDone / this->totalItems;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " % [" << itemsDone << "/" << totalItems << "]\r";
    std::cout.flush();


    std::cout << std::endl;
}