//
// Created by Maxim.Popov on 25.06.2022.
//

#include "progress_bar.h"
#include "spdlog/fmt/bundled/format.h"
#include "spdlog/spdlog.h"
#include <iostream>

ProgressBar::ProgressBar(int totalItems): totalItems(totalItems) {}

void ProgressBar::update(int itemsDone, int nodeCount, int stepTime_ms) {
    int barWidth = 50;
    double progress = ((double) itemsDone) / this->totalItems;

    std::string line = "";
    line += "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) line += "=";
        else if (i == pos) line +=  ">";
        else line += " ";
    }
    line += fmt::format("] {}% [{}/{}] | Node count: {} | Step time: {}ms",int(progress * 100.0), itemsDone, totalItems, nodeCount, stepTime_ms);
    SPDLOG_INFO(line);
}