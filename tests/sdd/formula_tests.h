//
// Created by Maxim.Popov on 04.07.2022.
//
#ifndef SDD_FORMULA_TESTS_H
#define SDD_FORMULA_TESTS_H

extern "C" {
    #include "sddapi.h"
}
#include "sdd_formulas/sdd_formulas.h"
#include "spdlog/spdlog.h"
#include "utils/file_utils.h"
#include "utils/minterm_utils.h"
#include "utils/output_utils.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include "../utils.h"

namespace sdd {

void checkMinterms(SddNode *sdd, SddManager* gbm, std::vector<int> minterms, int varAmount) {
    for (int i = 0; i < std::pow(2, varAmount); i++) {
        SddNode* delta = sdd;
        for (int var = 1; var <= varAmount; var++) {
            SddLiteral literal = (i >> (var - 1)) % 2 == 1 ? var : -var;
            delta = sdd_condition(literal, delta, gbm);
        }
        if (std::find(minterms.begin(), minterms.end(), i) != minterms.end()) {
            EXPECT_TRUE(sdd_node_is_true(delta));  
        } else {
            EXPECT_TRUE(sdd_node_is_false(delta));
        }
    }
}
}

TEST(SDDFormulaTestSuite, CreateDNFFormula) {
    FormulaInfo formulaInfo;
    formulaInfo.type = Form::DNF;
    formulaInfo.symbols = {1, 2, 0, -1, 3, 0, 0};

    SddManager *gbm = sdd_manager_create(3, 1); 
    SddNode* sdd = sdd::createNFFormulaFromInfo(gbm, formulaInfo);    
    
    std::vector<int> solutions = { 0b111, 0b011, 0b110, 0b100 };
    sdd::checkMinterms(sdd, gbm, solutions, 3);
    
    sdd_deref(sdd, gbm);
}

TEST(SDDFormulaTestSuite, CreateCNFFormula) {
    FormulaInfo formulaInfo;
    formulaInfo.type = Form::CNF;
    formulaInfo.symbols = {1, 2, 0, -1, 3, 0, 0};

    SddManager *gbm = sdd_manager_create(3, 1); 
    SddNode* sdd = sdd::createNFFormulaFromInfo(gbm, formulaInfo);    
    
    std::vector<int> solutions = { 0b010, 0b110, 0b101, 0b111 };
    sdd::checkMinterms(sdd, gbm, solutions, 3);
    sdd_deref(sdd, gbm);
}

TEST(SDDFormulaTestSuite, CreateRandomDNFFormula) {
    int varAmount = 8;
    SddManager *gbm = sdd_manager_create(varAmount, 1);

    TestFormula testFormula = createRandomDNFFormula(varAmount);

    std::cout << "Solving DNF formula with symbols: ";
    for (int symbol : testFormula.formula.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;

    SddNode* sdd = sdd::createNFFormulaFromInfo(gbm, testFormula.formula);
    sdd::checkMinterms(sdd, gbm, testFormula.solutions, varAmount);
    sdd_deref(sdd, gbm);
}

TEST(SDDFormulaTestSuite, CreateRandomCNFFormula) {
    int varAmount = 8;
    SddManager *gbm = sdd_manager_create(varAmount, 1);

    TestFormula testFormula = createRandomCNFFormula(varAmount);

    std::cout << "Solving CNF formula with symbols: ";
    for (int symbol : testFormula.formula.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;

    SddNode* sdd = sdd::createNFFormulaFromInfo(gbm, testFormula.formula);
    sdd::checkMinterms(sdd, gbm, testFormula.solutions, varAmount);
    sdd_deref(sdd, gbm);
}

TEST(SDDFormulaTestSuite, CreateRandomDNFFormulaFromClauses) {
    int varAmount = 8;
    SddManager *gbm = sdd_manager_create(varAmount, 1);

    TestFormula testFormula = createRandomDNFFormula(varAmount);

    std::cout << "Solving DNF formula with symbols: ";
    for (int symbol : testFormula.formula.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;

    SddNode* sdd = sdd::createNFFormulaFromClauses(gbm, testFormula.formula);
    sdd::checkMinterms(sdd, gbm, testFormula.solutions, varAmount);
    sdd_deref(sdd, gbm);
}

TEST(SDDFormulaTestSuite, CreateRandomCNFFormulaFromClauses) {
    int varAmount = 8;
    SddManager *gbm = sdd_manager_create(varAmount, 1);

    TestFormula testFormula = createRandomCNFFormula(varAmount);

    std::cout << "Solving CNF formula with symbols: ";
    for (int symbol : testFormula.formula.symbols) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;

    SddNode* sdd = sdd::createNFFormulaFromClauses(gbm, testFormula.formula);
    sdd::checkMinterms(sdd, gbm, testFormula.solutions, varAmount);
    sdd_deref(sdd, gbm);
}

// TEST(FormulaTestSuite, CreateRandomDNFFormulaMerge) {
//     int varAmount = 8;
//     DdManager *gbm = Cudd_Init(varAmount, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */

//     TestFormula testFormula = createRandomDNFFormula(varAmount);

//     std::cout << "Solving DNF formula with symbols: ";
//     for (int symbol : testFormula.formula.symbols) {
//         std::cout << symbol << " ";
//     }
//     std::cout << std::endl ;

//     DdNode* bdd = createNFFormulaMerge(gbm, testFormula.formula);
//     std::vector<std::vector<bool>> minterms = getMinterms(gbm, bdd, varAmount, std::pow(2, varAmount));
//     std::vector<int> binaryMinterms = convertBooleanMintermsToBinary(minterms);
//     EXPECT_TRUE(std::is_permutation(binaryMinterms.begin(), binaryMinterms.end(), testFormula.solutions.begin()));
// }

#endif