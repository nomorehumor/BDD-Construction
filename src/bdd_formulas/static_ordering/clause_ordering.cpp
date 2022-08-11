//
// Created by Maxim.Popov on 11.07.2022.
//
#include "clause_ordering.h"
#include "../../utils/BDDConfiguration.h"
#include "../../utils/progress_bar.h"
#include "FORCEPlacer.h"
#include "matplotlibcpp.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <cudd.h>
#include <iostream>
#include <map>
#include <random>

RulesetInfo orderClausesBottomUp(RulesetInfo &setInfo) {
    RulesetInfo orderedRuleset = setInfo;
    orderedRuleset.formulas = {};

    for (FormulaInfo formula : setInfo.formulas) {
        std::vector<int> newSymbols;
        auto firstInClause = formula.symbols.begin();
        for (auto it = formula.symbols.begin() + 1; it < formula.symbols.end();
             it++) {
            if (*it == 0 && *firstInClause != 0) {
                std::vector<int> clause(firstInClause, it);
                std::sort(clause.begin(), clause.end(), [](int a, int b) {
                    return std::abs(a) > std::abs(b);
                });
                newSymbols.insert(newSymbols.end(), clause.begin(),
                                  clause.end());
                newSymbols.push_back(0);
                firstInClause = it + 1;
            }
        }

        if (formula.type == Form::DNF) {
            newSymbols.push_back(0);
        }

        FormulaInfo newFormula = formula;
        newFormula.symbols = newSymbols;
        orderedRuleset.formulas.push_back(newFormula);
    }

    return orderedRuleset;
}

RulesetInfo orderClausesFORCE(RulesetInfo setInfo) {
    std::vector<FormulaInfo> formulas;

    ProgressBar bar(setInfo.formulas.size());
    int i = 1;
    spdlog::info("Ordering clauses with FORCE");
    for (FormulaInfo &formula : setInfo.formulas) {
        if (formula.clauses.size() > 2) {
            FORCEPlacer placer;
            placer.initGraphWithFormula(formula);
            bar.update(i);
            formulas.push_back(placer.orderClausesWithPlacement(
                formula, placer.findPlacement()));
        } else {
            formulas.push_back(formula);
        }

        i++;
    }
    bar.update(i);
    setInfo.formulas = formulas;
    return setInfo;
}