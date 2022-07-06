//
// Created by Maxim.Popov on 04.07.2022.
//


#include "gtest/gtest.h"
#include "utils/file_utils.h"
#include "bdd_formulas/bdd_formulas.h"

TEST(FormulaTestSuite, CreateDNFFormula) {
    FormulaInfo info;
    info.type = Form::DNF;
    info.symbols = {1, 2, 0, -1, 3, 0, 0};

    DdManager *gbm = Cudd_Init(info.symbols.size(), 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS,0); /* Initialize a new BDD manager. */

    DdNode* bdd = createFormulaFromInfo(gbm, info);
    std::cout << Cudd_CountMinterm(gbm, bdd, 3) << std::endl;
}


