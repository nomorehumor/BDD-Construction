//
// Created by Maxim.Popov on 22.06.2022.
//
#include "file_utils.h"
#include "spdlog/spdlog.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

RulesetInfo readClauselSetInfo(std::string filename, bool convertAmo) {

    spdlog::info("Reading ruleset from {}", filename);

    char filenameChar[filename.size()];
    strcpy(filenameChar, filename.c_str());
    FILE* file = fopen(filenameChar, "r");

    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t length = 0;

    ssize_t read = getline(&line, &length, file);
    int whitespace_count = 0;
    std::string clauselAmount = "";
    std::string varAmount = "";
    std::string formulaType = "";
    for (int i = 0; i < read; i++) {
        if (line[i] == ' ') {
            whitespace_count++;
        } else if (whitespace_count == 1) {
            formulaType.append(1, line[i]);
        } else if (whitespace_count == 2) {
            varAmount.append(1, line[i]);
        } else if (whitespace_count == 3) {
            clauselAmount.append(1, line[i]);
        }
    }
    RulesetInfo info;

    if (formulaType == "cnf") {
        info.type = Form::CNF;
    } else if (formulaType == "dnf") {
        info.type = Form::DNF;
    } else {
        std::cout << "Unknown set form (nor DNF or CNF)" << std::endl;
        exit(EXIT_FAILURE);
    }

    info.clauselAmount = std::stoi(clauselAmount);
    info.variableAmount = std::stoi(varAmount);

    for (int i = 0; i < info.clauselAmount; i++) {
        getline(&line, &length, file);
        FormulaInfo formulaInfo = getFormulaInfoFromLine(line);
        if (formulaInfo.type == Form::AMO) {
            if (formulaInfo.containedVars.size() == 1) continue;
            if (convertAmo) {
                formulaInfo = transformAMOtoDNF(formulaInfo);
            }
        }
        formulaInfo.id = i;
        info.formulas.push_back(formulaInfo);
    }

    fclose(file);
    return info;
}

FormulaInfo transformAMOtoDNF(FormulaInfo info) {
    FormulaInfo transformedInfo;
    std::vector<int> formula;
    std::vector<int> zeroTerm;
    for (int i = 0; i < info.symbols.size() - 1; i++) {
        std::vector<int> clause;
        clause.push_back(info.symbols.at(i));
        zeroTerm.push_back(-info.symbols.at(i));
        for (int j = 0; j < info.symbols.size() - 1; j++) {
            if (i != j) {
                clause.push_back(-info.symbols.at(j));
            }
        }
        transformedInfo.clauses.push_back(clause);
        formula.insert(formula.end(), clause.begin(), clause.end());
        formula.push_back(0);
    }
    transformedInfo.clauses.push_back(zeroTerm);

    formula.insert(formula.end(), zeroTerm.begin(), zeroTerm.end());
    formula.push_back(0);
    formula.push_back(0);
    transformedInfo.symbols = formula;
    transformedInfo.type = Form::DNF;
    transformedInfo.id = info.id;
    return transformedInfo;
}

FormulaInfo getFormulaInfoFromLine(char* line) {
    FormulaInfo info;
    std::string type = "";
    std::string temp_term = "";
    int whitespaceCount = 0;
    int lineLength = strlen(line);
    for (int i = 0; i < lineLength; i++) {
        if ((line[i] == ' ' && whitespaceCount > 0) || line[i] == '\\n') {
            info.symbols.push_back(std::stoi(temp_term));
            temp_term = "";
        }

        if ((std::isdigit(line[i]) || line[i] == '-') && whitespaceCount == 0) {
            whitespaceCount = 1;
            type = "CNF";
        }

        if (line[i] == ' ') {
            whitespaceCount++;
        } else if (whitespaceCount == 0) {
            type.append(1, line[i]);
        } else {
            temp_term.append(1, line[i]);
        }
    }

    info.symbols.push_back(std::stoi(temp_term));

    std::vector<int> clause;
    std::for_each(info.symbols.begin(), info.symbols.end(), [&](int symbol) {
        if (symbol != 0) {
            info.containedVars.insert(std::abs(symbol));
            clause.push_back(symbol);
        } else if (!clause.empty()) {
            info.clauses.push_back(clause);
            clause = {};
        }
    });

    if (type == "DNF") {
        info.type = Form::DNF;
    } else if (type == "CNF") {
        info.type = Form::CNF;
    } else if (type == "AMO") {
        info.type = Form::AMO;
    } else {
        std::cout << "Unknown formula form (nor DNF or CNF): " << type << " on line: " << line << std::endl;
        exit(EXIT_FAILURE);
    }
    return info;
}
