//
// Created by Maxim.Popov on 22.06.2022.
//
#include <vector>
#include <iostream>
#include <cstring>
#include <cudd.h>
#include "file_utils.h"


ClauselSetInfo readClauselSetInfo(FILE* file) {
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
    std::cout << clauselAmount + " " + varAmount << std::endl;
    ClauselSetInfo info;

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
    return info;
}

ClauselInfo transformAMOtoDNF(ClauselInfo info) {
    ClauselInfo transformedInfo;
    std::vector<int> formula;
    for (int i = 0; i < info.terms.size(); i++) {
        formula.push_back(i);
        for (int j = 0; j < info.terms.size(); j++) {
            if (i != j) {
                formula.push_back(-j);
            }
        }
        formula.push_back(0);
    }
    formula.push_back(0);
    transformedInfo.type = Form::DNF;
    return transformedInfo;
}

ClauselInfo getClauselInfoFromLine(char* line) {
    ClauselInfo info;
    std::string type = "";
    std::string temp_term = "";
    int whitespaceCount = 0;
    int lineLength = strlen(line);
    for (int i = 0; i < lineLength; i++) {
        if ((line[i] == ' ' || line[i] == '\\n') && whitespaceCount > 0) {
            info.terms.push_back(std::stoi(temp_term));
            temp_term = "";
        }

        if ((std::isdigit(line[i]) || line[i] == '-') && whitespaceCount == 0) {
            whitespaceCount = 1;
            type = 'CNF';
        }

        if (line[i] == ' ') {
            whitespaceCount++;
        } else if (whitespaceCount == 0) {
            type.append(1, line[i]);
        } else {
            temp_term.append(1, line[i]);
        }
    }

    if (type == "DNF") {
        info.type = Form::DNF;
    } else if (type == "CNF") {
        info.type = Form::CNF;
    } else if (type == "AMO") {
        info = transformAMOtoDNF(info);
    } else {
        std::cout << "Unknown set form (nor DNF or CNF)" << std::endl;
        exit(EXIT_FAILURE);
    }

    info.terms.push_back(std::stoi(temp_term));

    return info;
}
