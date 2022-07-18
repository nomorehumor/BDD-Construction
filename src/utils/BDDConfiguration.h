//
// Created by Maxim.Popov on 17.07.2022.
//

#ifndef CODE_BDDCONFIGURATION_H
#define CODE_BDDCONFIGURATION_H

#include <string>
#include "yaml-cpp/yaml.h"


class BDDConfiguration {
public:
    static BDDConfiguration* getInstance() {
        if(configuration_ == nullptr) {
            configuration_ = new BDDConfiguration();
        }
        return configuration_;
    }

    static void load(const std::string& filename) {
        YAML::Node config = YAML::LoadFile(filename);

        inputFilename = config["filename"].as<std::string>();
        outputPlots = config["output"]["output_plots"].as<bool>();
        skipMostFrequentVar = config["ordering"]["skip_most_frequent_var"].as<bool>();
        printProgress = config["output"]["print_progress"].as<bool>();
    }

    static std::string getInputFilename() {
        return inputFilename;
    }

    static bool getPrintProgress() {
        return printProgress;
    }

    static bool getOutputPlots() {
        return outputPlots;
    }

    static bool getSkipMostFrequentVar() {
        return skipMostFrequentVar;
    }

    BDDConfiguration(BDDConfiguration const&) = delete;
    void operator=(BDDConfiguration const&) = delete;


protected:
    inline static std::string inputFilename = "";
    inline static bool outputPlots = true;
    inline static bool printProgress = true;
    inline static bool skipMostFrequentVar = true;

    BDDConfiguration() {}
    inline static BDDConfiguration* configuration_ = nullptr;
};


#endif //CODE_BDDCONFIGURATION_H
