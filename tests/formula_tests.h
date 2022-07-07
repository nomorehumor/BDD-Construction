//
// Created by Maxim.Popov on 04.07.2022.
//


#include "gtest/gtest.h"
#include "utils/file_utils.h"
#include "bdd_formulas/bdd_formulas.h"
#include "utils/output_utils.h"
#include <random>
#include <algorithm>

struct TestFormula {
    FormulaInfo formula;
    std::vector<int> solutions;
};

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

TestFormula createRandomDNFFormula(int varAmount) {
    FormulaInfo formulaInfo;
    formulaInfo.type = Form::DNF;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> clauselAmountDist(1, varAmount / 2);
    std::uniform_int_distribution<std::mt19937::result_type> varSignDist(0,1);

    std::vector<int> solutions;
    int clauselAmount = clauselAmountDist(rng);

    std::cout << "Generating random formula, clausel amount: " << clauselAmount << std::endl;
    for (int i = 0; i < clauselAmount; i++) {

        std::vector<int> clausel;
        int binarySolution = 0;
        for (int i = 1; i <= varAmount; i++) {
            int signedVariable = varSignDist(rng) ? i : -i;
            clausel.push_back(signedVariable);

            binarySolution = signedVariable > 0 ? binarySolution | (1 << (i-1)) : binarySolution;
        }
        solutions.push_back(binarySolution);
        clausel.push_back(0);

        formulaInfo.symbols.insert(formulaInfo.symbols.end(), clausel.begin(), clausel.end());
    }
    formulaInfo.symbols.push_back(0);

    std::cout << "Solving formula with symbols: ";
    for (int symbol : formulaInfo.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl ;

    TestFormula testFormula;
    testFormula.formula = formulaInfo;
    testFormula.solutions = solutions;
    return testFormula;
}

TEST(FormulaTestSuite, CreateDNFFormula) {
    FormulaInfo formulaInfo;
    formulaInfo.type = Form::DNF;
    formulaInfo.symbols = {1, 2, 0, -1, 3, 0, 0};

    DdManager *gbm = Cudd_Init(formulaInfo.symbols.size(), 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    DdNode* bdd = createFormulaFromInfo(gbm, formulaInfo);
//    DdNode* replacement = Cudd_VerifySol(gbm, bdd, solution, yIndex, 3);

    std::vector<std::vector<bool>> minterms = getMinterms(gbm, bdd, 3, 10);
    std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
    std::vector<int> solutions = { 0b111, 0b011, 0b110, 0b100 };
    EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), solutions.begin()));
}

TEST(FormulaTestSuite, CreateCNFFormula) {
    FormulaInfo formulaInfo;
    formulaInfo.type = Form::CNF;
    formulaInfo.symbols = {1, 2, 0, -1, 3, 0, 0};

    DdManager *gbm = Cudd_Init(formulaInfo.symbols.size(), 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    DdNode* bdd = createFormulaFromInfo(gbm, formulaInfo);

    std::vector<std::vector<bool>> minterms = getMinterms(gbm, bdd, 3, 8);
    std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
    std::vector<int> solutions = { 0b010, 0b110, 0b101, 0b111 };
    EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), solutions.begin()));
}

TEST(FormulaTestSuite, CreateRandomDNFFormula) {
    int varAmount = 8;
    DdManager *gbm = Cudd_Init(varAmount, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    TestFormula testFormula = createRandomDNFFormula(varAmount);

    DdNode* bdd = createFormulaFromInfo(gbm, testFormula.formula);
    std::vector<std::vector<bool>> minterms = getMinterms(gbm, bdd, varAmount, 8);
    EXPECT_EQ(minterms.size(), testFormula.solutions.size());
}
