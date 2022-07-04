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

struct FormulaInfo {
    Form type;
    std::vector<int> symbols;
};

struct FormulaSetInfo{
    Form type;
    int clauselAmount;
    int variableAmount;
    std::vector<FormulaInfo> formulas;
};


FormulaSetInfo readClauselSetInfo(char filename[]);

FormulaInfo transformAMOtoDNF(FormulaInfo info);

FormulaInfo getFormulaInfoFromLine(char* line);

#endif //CODE_FILE_UTILS_H
