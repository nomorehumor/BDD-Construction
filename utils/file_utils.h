//
// Created by Maxim.Popov on 22.06.2022.
//

#ifndef CODE_FILE_UTILS_H
#define CODE_FILE_UTILS_H

#include <vector>
#include <cstdio>

enum Form {
    DNF,
    CNF,
    AMO
};

struct FormulaSetInfo{
    Form type;
    int clauselAmount;
    int variableAmount;

};

struct FormulaInfo {
    Form type;
    std::vector<int> symbols;
};

FormulaSetInfo readClauselSetInfo(FILE* file);

FormulaInfo transformAMOtoDNF(FormulaInfo info);

FormulaInfo getClauselInfoFromLine(char* line);

#endif //CODE_FILE_UTILS_H
