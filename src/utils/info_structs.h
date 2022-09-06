#ifndef INFO_STRUCTS_H
#define INFO_STRUCTS_H

#include <vector>
#include <set>

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

#endif