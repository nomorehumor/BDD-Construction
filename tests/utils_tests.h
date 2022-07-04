//
// Created by Maxim.Popov on 04.07.2022.
//

#include "gtest/gtest.h"
#include "utils/file_utils.h"

TEST(UtilsTestsSuite, ReadDNF) {
    char* line = "DNF 1 0 2 3 0 0";
    FormulaInfo formulaInfo = getFormulaInfoFromLine(line);
    std::vector<int> expectedSymbols = {1, 0, 2, 3, 0, 0};
    EXPECT_EQ(formulaInfo.symbols, expectedSymbols);
    EXPECT_EQ(formulaInfo.type, Form::DNF);
}

TEST(UtilsTestsSuite, ReadCNF) {
    char* line = "CNF 1 0 2 3 0 0";
    FormulaInfo formulaInfo = getFormulaInfoFromLine(line);
    std::vector<int> expectedSymbols = {1, 0, 2, 3, 0, 0};
    EXPECT_EQ(formulaInfo.symbols, expectedSymbols);
    EXPECT_EQ(formulaInfo.type, Form::CNF);
}

TEST(UtilsTestsSuite, ReadAMO) {
    char* line = "AMO 1 2 3 0";
    FormulaInfo formulaInfo = getFormulaInfoFromLine(line);
    std::vector<int> expectedSymbols = {1, -2, -3, 0, 2, -1, -3, 0, 3, -1, -2, 0, 0};
    EXPECT_EQ(formulaInfo.symbols, expectedSymbols);
    EXPECT_EQ(formulaInfo.type, Form::DNF);
}