//
// Created by Maxim.Popov on 20.07.2022.
//

#ifndef CODE_FORCEPLACER_H
#define CODE_FORCEPLACER_H

#include "../../utils/file_utils.h"
#include "hypergraph.h"
#include <map>
#include <set>
#include <vector>

class FORCEPlacer {
  public:
    FORCEPlacer() {}
    void initGraphWithRuleset(RulesetInfo& rulesetInfo);
    void initGraphWithRulesetModifiedFORCE(RulesetInfo& rulesetInfo);
    void initGraphWithFormula(FormulaInfo &formula);

    int calculateGraphTotalSpan();
    std::vector<FormulaInfo> orderFormulasWithPlacement(std::vector<FormulaInfo> formulas, std::vector<HGNode> placement);
    FormulaInfo orderClausesWithPlacement(FormulaInfo formula, std::vector<HGNode> placement);
    std::vector<int> orderVariablesWithPlacement(std::vector<HGNode> placement);
    std::vector<HGNode> findPlacement(bool output = false);
  protected:
    void createGraphEdgesFromNodes();

    const double iterationConstant = 10;
    const double spanEpsilon = 200;
    std::map<int, std::set<HGNode>> varNodes;
    HGGraph graph;
};

#endif // CODE_FORCEPLACER_H