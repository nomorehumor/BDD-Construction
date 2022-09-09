//
// Created by Maxim.Popov on 04.07.2022.
//

#include "utils_tests.h"
#include "bdd/formula_tests.h"
#include "sdd/formula_tests.h"
#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}