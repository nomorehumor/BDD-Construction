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

struct ClauselSetInfo{
    Form type;
    int clauselAmount;
    int variableAmount;
};

struct ClauselInfo {
    Form type;
    std::vector<int> terms;
};

ClauselSetInfo readClauselSetInfo(FILE* file);

ClauselInfo transformAMOtoDNF(ClauselInfo info);

ClauselInfo getClauselInfoFromLine(char* line);

#endif //CODE_FILE_UTILS_H
