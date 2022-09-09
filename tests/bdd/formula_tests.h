//
// Created by Maxim.Popov on 04.07.2022.
//
#ifndef BDD_FORMULA_TESTS_H
#define BDD_FORMULA_TESTS

#include "bdd_formulas/bdd_formulas.h"
#include "spdlog/spdlog.h"
#include "utils/file_utils.h"
#include "utils/minterm_utils.h"
#include "utils/output_utils.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include "../utils.h"

TEST(FormulaTestSuite, CreateDNFFormula) {
    FormulaInfo formulaInfo;
    formulaInfo.type = Form::DNF;
    formulaInfo.symbols = {1, 2, 0, -1, 3, 0, 0};

    DdManager *gbm = Cudd_Init(formulaInfo.symbols.size(), 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    DdNode* bdd = createNFFormulaFromInfo(gbm, formulaInfo);
//    DdNode* replacement = Cudd_VerifySol(gbm, bdd, solution, yIndex, 3);

    std::vector<std::vector<bool>> minterms = bdd::getMinterms(gbm, bdd, 3, 10);
    std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
    std::vector<int> solutions = { 0b111, 0b011, 0b110, 0b100 };
    EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), solutions.begin()));
}

TEST(FormulaTestSuite, CreateCNFFormula) {
    FormulaInfo formulaInfo;
    formulaInfo.type = Form::CNF;
    formulaInfo.symbols = {1, 2, 0, -1, 3, 0, 0};

    DdManager *gbm = Cudd_Init(formulaInfo.symbols.size(), 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    DdNode* bdd = createNFFormulaFromInfo(gbm, formulaInfo);

    std::vector<std::vector<bool>> minterms = bdd::getMinterms(gbm, bdd, 3, 8);
    std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
    std::vector<int> solutions = { 0b010, 0b110, 0b101, 0b111 };
    EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), solutions.begin()));
}

TEST(FormulaTestSuite, CreateRandomDNFFormula) {
    int varAmount = 8;
    DdManager *gbm = Cudd_Init(varAmount, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    TestFormula testFormula = createRandomDNFFormula(varAmount);

    std::cout << "Solving DNF formula with symbols: ";
    for (int symbol : testFormula.formula.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl ;

    DdNode* bdd = createNFFormulaFromInfo(gbm, testFormula.formula);
    std::vector<std::vector<bool>> minterms = bdd::getMinterms(gbm, bdd, varAmount, std::pow(2, varAmount));
    std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
    EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), testFormula.solutions.begin()));
}

TEST(FormulaTestSuite, CreateRandomCNFFormula) {
    int varAmount = 8;
    DdManager *gbm = Cudd_Init(varAmount, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    TestFormula testFormula = createRandomCNFFormula(varAmount);

    spdlog::info("Solving CNF formula with symbols: ");
    for (int symbol : testFormula.formula.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;

    DdNode* bdd = createNFFormulaFromInfo(gbm, testFormula.formula);
    std::vector<std::vector<bool>> minterms = bdd::getMinterms(gbm, bdd, varAmount, std::pow(2, varAmount));
    std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
    EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), testFormula.solutions.begin()));

    Cudd_RecursiveDeref(gbm, bdd);
    EXPECT_EQ(Cudd_CheckZeroRef(gbm), 0);
}

TEST(FormulaTestSuite, CreateRandomDNFFormulaFromClauses) {
    int varAmount = 8;
    DdManager *gbm = Cudd_Init(varAmount, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    TestFormula testFormula = createRandomDNFFormula(varAmount);

    std::cout << "Solving DNF formula with symbols: ";
    for (int symbol : testFormula.formula.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl ;

    DdNode* bdd = createNFFormulaFromClauses(gbm, testFormula.formula);
    std::vector<std::vector<bool>> minterms = bdd::getMinterms(gbm, bdd, varAmount, std::pow(2, varAmount));
    std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
    EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), testFormula.solutions.begin()));
}

TEST(FormulaTestSuite, CreateRandomDNFFormulaMerge) {
    int varAmount = 8;
    DdManager *gbm = Cudd_Init(varAmount, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

    TestFormula testFormula = createRandomDNFFormula(varAmount);

    std::cout << "Solving DNF formula with symbols: ";
    for (int symbol : testFormula.formula.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl ;

    DdNode* bdd = createNFFormulaMerge(gbm, testFormula.formula);
    std::vector<std::vector<bool>> minterms = bdd::getMinterms(gbm, bdd, varAmount, std::pow(2, varAmount));
    std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
    EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), testFormula.solutions.begin()));
}

#endif