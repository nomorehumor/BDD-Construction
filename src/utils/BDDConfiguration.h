//
// Created by Maxim.Popov on 17.07.2022.
//

#ifndef CODE_BDDCONFIGURATION_H
#define CODE_BDDCONFIGURATION_H

#include <string>
#include "yaml-cpp/yaml.h"

class InputParser{
  public:
    InputParser (int &argc, char **argv){

        std::string currentOption = "";
        for (int i=1; i < argc; i++) {
            if (argv[i][0] == '-' && argv[i][1] == '-') {
                currentOption = argv[i];
            } else {
                if (currentOption == "") {
                    this->tokens.push_back(std::string(argv[i]));
                } else {
                    arguments[currentOption] = argv[i];
                    currentOption = "";
                }
            }
        }
    }
    const std::string& getCmdOption(const std::string &option) const {
        std::vector<std::string>::const_iterator itr;
        itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()){
            return *itr;
        }
        static const std::string empty_string("");
        return empty_string;
    }

    const std::string getCmdArgument(const std::string &arg) const {
        return arguments.at(arg);
    }

    bool cmdOptionExists(const std::string &option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }

    bool cmdArgumentExists(const std::string &arg) const {
        return arguments.contains(arg);
    }

  private:
    std::vector <std::string> tokens;
    std::map<std::string, std::string> arguments;
};


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
        orderingStrategy = config["ordering"]["set"]["strategy"].as<std::string>();
        ascending = config["ordering"]["set"]["ascending"].as<bool>();
        countAllAppearances = config["ordering"]["set"]["count_all_appearances"].as<bool>();
        enableDynamicOrdering = config["ordering"]["enable_dynamic_ordering"].as<bool>();
        clauseOrderingStrategy = config["ordering"]["clause"]["strategy"].as<std::string>();
        topologicalOrdering = config["ordering"]["topological"].as<std::string>();
    }

    static void parseArgs(int argc, char *argv[]) {
        InputParser parser(argc, argv);
        if (parser.cmdArgumentExists("--ordering")) {
            orderingStrategy = parser.getCmdArgument("--ordering");
        }
        if (parser.cmdArgumentExists("--topological")) {
            topologicalOrdering = parser.getCmdArgument("--topological");
        }
        if (parser.cmdArgumentExists("--filename")) {
            inputFilename = parser.getCmdArgument("--filename");
        }
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
        return orderingStrategy;
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

    static std::string getTopologicalOrdering() {
        return topologicalOrdering;
    }

    static std::string getOutputDirectory() {
        return outputDirectory;
    }

    static void setOutputDirectory(std::string newOutputDirectory) {
        outputDirectory = newOutputDirectory;
    }

    BDDConfiguration(BDDConfiguration const&) = delete;
    void operator=(BDDConfiguration const&) = delete;


protected:
    inline static std::string inputFilename = "";
    inline static bool outputPlots = true;
    inline static bool printProgress = true;
    inline static bool enableDynamicOrdering = true;
    inline static std::string orderingStrategy = "none";
    inline static bool ascending = true; // for 'size' ordering
    inline static bool skipMostFrequentVar = true; // for 'var_frequency' ordering
    inline static bool countAllAppearances = true; // for 'var_frequency' ordering
    inline static std::string clauseOrderingStrategy = "none";
    inline static std::string topologicalOrdering = "dfs";
    inline static std::string outputDirectory = "output";

    BDDConfiguration() {}
    inline static BDDConfiguration* configuration_ = nullptr;
};


#endif //CODE_BDDCONFIGURATION_H
