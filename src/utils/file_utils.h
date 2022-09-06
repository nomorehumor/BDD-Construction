//
// Created by Maxim.Popov on 22.06.2022.
//

#ifndef CODE_FILE_UTILS_H
#define CODE_FILE_UTILS_H

#include <cstdio>
#include <set>
#include <string>
#include <vector>
#include "info_structs.h"

RulesetInfo readClauselSetInfo(std::string filename, bool convertAmo = false);

FormulaInfo transformAMOtoDNF(FormulaInfo info);

FormulaInfo getFormulaInfoFromLine(char* line, bool convertAmo = false);

#endif //CODE_FILE_UTILS_H
