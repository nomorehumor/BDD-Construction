//
// Created by Maxim.Popov on 28.07.2022.
//

#include <valarray>
#include <vector>
#include <cudd.h>

namespace bdd {
std::vector<std::vector<bool>> getMinterms(DdManager *gbm, DdNode *bdd,
                                           int numVars, int maxAmount,
                                           bool output) {
    DdNode **vars = new DdNode *[numVars]();
    std::vector<std::vector<bool>> minterms;

    for (int i = 0; i < numVars; i++) {
        vars[i] = Cudd_bddIthVar(gbm, i);
    }

    DdNode *currentNode, *temp;
    currentNode = bdd;
    Cudd_Ref(currentNode);
    while (currentNode != Cudd_ReadLogicZero(gbm) &&
           minterms.size() < maxAmount) {
        DdNode *minterm =
            Cudd_bddPickOneMinterm(gbm, currentNode, vars, numVars);

        Cudd_Ref(minterm);
        std::vector<bool> mintermSolution;
        for (int j = 0; j < numVars; j++) {
            mintermSolution.push_back(Cudd_bddLeq(gbm, minterm, vars[j]));
        }
        minterms.push_back(mintermSolution);

        temp = Cudd_bddAnd(gbm, currentNode, Cudd_Not(minterm));
        Cudd_Ref(temp);
        Cudd_RecursiveDeref(gbm, currentNode);
        Cudd_RecursiveDeref(gbm, minterm);
        currentNode = temp;
    }

    Cudd_RecursiveDeref(gbm, currentNode);
    delete[] vars;
    return minterms;
}

}

std::vector<int> convertBooleanMintermsToBinary(std::vector<std::vector<bool>> minterms) {
    std::vector<int> binaryMinterms;
    for (std::vector<bool> minterm : minterms) {
        int binaryMinterm = 0;
        for (int i = 0; i < minterm.size(); i++) {
            if (minterm[i]) binaryMinterm |= 1 << i;
        }
        binaryMinterms.push_back(binaryMinterm);
    }
    return binaryMinterms;
}

std::vector<int> convertMaxtermsToMinterms(std::vector<int> maxterms, int varAmount) {
    std::vector<int> minterms;
    for (int i = 0; i < std::pow(2, varAmount); i++) {
        if (std::find(maxterms.begin(), maxterms.end(), i) == maxterms.end()) {
            minterms.push_back(i);
        }
    }
    return minterms;
}