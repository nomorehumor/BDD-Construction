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
        skipMostFrequentVar = config["ordering"]["set"]["skip_most_frequent_var"].as<bool>();
        printProgress = config["output"]["print_progress"].as<bool>();
        setOrderingStrategy = config["ordering"]["set"]["strategy"].as<std::string>();
        ascending = config["ordering"]["set"]["ascending"].as<bool>();
        countAllAppearances = config["ordering"]["set"]["count_all_appearances"].as<bool>();
        enableDynamicOrdering = config["ordering"]["enable_dynamic_ordering"].as<bool>();
        clauseOrderingStrategy = config["ordering"]["clause"]["strategy"].as<std::string>();
    }

    static std::string getInputFilename() {
        return inputFilename;
    }

    static bool getPrintProgress() {
        return printProgress;
    }

    static bool isOutputPlots() {
        return outputPlots;
    }

    static bool isSkipMostFrequentVar() {
        return skipMostFrequentVar;
    }

    static bool isEnableDynamicOrdering() {
        return enableDynamicOrdering;
    }

    static std::string getOrderingStrategy() {
        return setOrderingStrategy;
    }

    static bool isAscending() {
        return ascending;
    }

    static bool isCountAllAppearances() {
        return countAllAppearances;
    }

    static std::string getClauseOrderingStrategy() {
        return clauseOrderingStrategy;
    }

    BDDConfiguration(BDDConfiguration const&) = delete;
    void operator=(BDDConfiguration const&) = delete;


protected:
    inline static std::string inputFilename = "";
    inline static bool outputPlots = true;
    inline static bool printProgress = true;
    inline static bool enableDynamicOrdering = true;
    inline static std::string setOrderingStrategy = "none";
    inline static bool ascending = true; // for 'size' ordering
    inline static bool skipMostFrequentVar = true; // for 'var_frequency' ordering
    inline static bool countAllAppearances = true; // for 'var_frequency' ordering
    inline static std::string clauseOrderingStrategy = "none";

  protected:
    BDDConfiguration() {}
    inline static BDDConfiguration* configuration_ = nullptr;
};


#endif //CODE_BDDCONFIGURATION_H
