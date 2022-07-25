//
// Created by Maxim.Popov on 22.06.2022.
//

#ifndef CODE_FILE_UTILS_H
#define CODE_FILE_UTILS_H

#include <cstdio>
#include <set>
#include <string>
#include <vector>

enum Form {
    DNF,
    CNF,
    AMO
};

struct FormulaInfo {
    int id;
    Form type;
    std::vector<int> symbols;
    std::vector<std::vector<int>> clauses;
    std::set<int> containedVars;
};

struct RulesetInfo{
    Form type;
    int clauselAmount;
    int variableAmount;
    std::vector<FormulaInfo> formulas;
};


RulesetInfo readClauselSetInfo(std::string filename, bool convertAmo = false);

FormulaInfo transformAMOtoDNF(FormulaInfo info);

FormulaInfo getFormulaInfoFromLine(char* line, bool convertAmo = false);

#endif //CODE_FILE_UTILS_H
