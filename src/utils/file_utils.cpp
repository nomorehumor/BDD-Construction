//
// Created by Maxim.Popov on 22.06.2022.
//
#include <vector>
#include <iostream>
#include <cstring>
#include <cudd.h>
#include "file_utils.h"
#include <cstdio>


RulesetInfo readClauselSetInfo(std::string filename, bool convertAmo) {
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
    std::cout << clauselAmount + " " + varAmount << std::endl;
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
        FormulaInfo formulaInfo = getFormulaInfoFromLine(line, convertAmo);
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
        formula.push_back(info.symbols.at(i));
        zeroTerm.push_back(-info.symbols.at(i));
        for (int j = 0; j < info.symbols.size() - 1; j++) {
            if (i != j) {
                formula.push_back(-info.symbols.at(j));
            }
        }
        formula.push_back(0);
    }
    formula.insert(formula.end(), zeroTerm.begin(), zeroTerm.end());
    formula.push_back(0);
    formula.push_back(0);
    transformedInfo.symbols = formula;
    transformedInfo.type = Form::DNF;
    return transformedInfo;
}

FormulaInfo getFormulaInfoFromLine(char* line, bool convertAmo) {
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

    if (type == "DNF") {
        info.type = Form::DNF;
    } else if (type == "CNF") {
        info.type = Form::CNF;
    } else if (type == "AMO") {
        info.type = Form::AMO;
        if (convertAmo) {
            info = transformAMOtoDNF(info);
        }
    } else {
        std::cout << "Unknown formula form (nor DNF or CNF): " << type << " on line: " << line << std::endl;
        exit(EXIT_FAILURE);
    }
    return info;
}
