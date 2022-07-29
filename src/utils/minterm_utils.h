//
// Created by Maxim.Popov on 28.07.2022.
//

#ifndef CODE_MINTERM_UTILS_H
#define CODE_MINTERM_UTILS_H

#include <cudd.h>
#include <vector>

std::vector<std::vector<bool>> getMinterms(DdManager *gbm, DdNode *bdd,
                                           int numVars, int maxAmount,
                                           bool output = false);
std::vector<int> convertBooleanMintermsToBinary(std::vector<std::vector<bool>> minterms);
std::vector<int> convertMaxtermsToMinterms(std::vector<int> maxterms, int varAmount);
#endif // CODE_MINTERM_UTILS_H
