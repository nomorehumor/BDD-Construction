//
// Created by Maxim.Popov on 05.08.2022.
//

#ifndef CODE_CSV_UTILS_H
#define CODE_CSV_UTILS_H

#include "configuration.h"
#include "spdlog/fmt/bundled/format.h"
#include <filesystem>
#include <fstream>
namespace utils {
void logRunInfo(int nodeCount, int totalTime_s, int constructionTime_s,
                int orderingTime_s) {
    std::filesystem::path filepath(Configuration::getInputFilename());
    filepath = filepath.make_preferred();
    std::string rulesetName = filepath.filename();
    std::string benchName = filepath.parent_path().filename();

    std::string csvFileName;
    if (Configuration::getDiagramType() == "bdd") {
        csvFileName = fmt::format("output/{}_{}_{}.csv", benchName,
                    Configuration::getRulesetOrderingStrategy(),
                    Configuration::getVariableOrdering());
    } else if (Configuration::getDiagramType() == "sdd") {
        csvFileName = fmt::format("output/{}_sdd.csv", benchName);
    }

    bool fileExists = std::filesystem::exists(csvFileName);
    std::fstream fout;
    fout.open(csvFileName, std::ios::out | std::ios::app);
    if (!fileExists) {
        fout << "ruleset name, ordering time, construction time, total time, nodeCount \n";
    }

    fout << rulesetName << "," << orderingTime_s << "," << constructionTime_s
         << "," << totalTime_s << "," << nodeCount << "\n";
}
}
#endif // CODE_CSV_UTILS_H
