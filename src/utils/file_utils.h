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
    int id;
    Form type;
    std::vector<int> symbols;
};

struct RulesetInfo{
    Form type;
    int clauselAmount;
    int variableAmount;
    std::vector<FormulaInfo> formulas;
};


RulesetInfo readClauselSetInfo(char filename[], bool convertAmo = false);

FormulaInfo transformAMOtoDNF(FormulaInfo info);

FormulaInfo getFormulaInfoFromLine(char* line, bool convertAmo = false);

#endif //CODE_FILE_UTILS_H
