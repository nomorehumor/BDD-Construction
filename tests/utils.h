#ifndef UTILS_H
#define UTILS_H

struct TestFormula {
    FormulaInfo formula;
    std::vector<int> solutions;
};

TestFormula createRandomCNFFormula(int varAmount) {
    FormulaInfo formulaInfo;
    formulaInfo.type = Form::CNF;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> clauselAmountDist(1, std::pow(2, varAmount - 1));
    std::uniform_int_distribution<std::mt19937::result_type> varSignDist(0,1);

    std::vector<int> maxterms;
    int clauselAmount = clauselAmountDist(rng);

    std::cout << "Generating random formula, clausel amount: " << clauselAmount << std::endl;
    for (int i = 0; i < clauselAmount; i++) {

        std::vector<int> clause;
        int binaryMaxterm = 0;
        for (int i = 1; i <= varAmount; i++) {
            int signedVariable = varSignDist(rng) ? i : -i;
            clause.push_back(signedVariable);

            binaryMaxterm = signedVariable < 0 ? binaryMaxterm | (1 << (i - 1)) : binaryMaxterm;
        }
        maxterms.push_back(binaryMaxterm);

        formulaInfo.clauses.push_back(clause);
        
        clause.push_back(0);
        formulaInfo.symbols.insert(formulaInfo.symbols.end(), clause.begin(), clause.end());
    }
    formulaInfo.symbols.push_back(0);

    TestFormula testFormula;
    testFormula.formula = formulaInfo;
    testFormula.solutions = convertMaxtermsToMinterms(maxterms, varAmount);
    return testFormula;
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

        std::vector<int> clause;
        int binarySolution = 0;
        for (int i = 1; i <= varAmount; i++) {
            int signedVariable = varSignDist(rng) ? i : -i;
            clause.push_back(signedVariable);

            binarySolution = signedVariable > 0 ? binarySolution | (1 << (i-1)) : binarySolution;
        }
        solutions.push_back(binarySolution);

        formulaInfo.clauses.push_back(clause);
        
        clause.push_back(0);
        formulaInfo.symbols.insert(formulaInfo.symbols.end(), clause.begin(), clause.end());
    }
    formulaInfo.symbols.push_back(0);



    TestFormula testFormula;
    testFormula.formula = formulaInfo;
    testFormula.solutions = solutions;
    return testFormula;
}

#endif