//
// Created by Maxim.Popov on 20.07.2022.
//

#ifndef CODE_FORCEPLACER_H
#define CODE_FORCEPLACER_H

#include "../../utils/file_utils.h"
#include <map>
#include <set>
#include <vector>

struct HGNode {
    int id;
};

inline bool operator<(const HGNode& lhs, const HGNode& rhs)
{
    return lhs.id < rhs.id;
}

inline bool operator==(const HGNode& lhs, const HGNode& rhs)
{
    return lhs.id == rhs.id;
}

struct HGEdge {
    int id;
    std::set<HGNode> nodes;
};

inline bool operator<(const HGEdge& lhs, const HGEdge& rhs)
{
    return lhs.id < rhs.id;
}

struct HGGraph {
    std::set<HGEdge> edges;
    std::vector<HGNode> nodes;
    std::map<HGNode, std::set<HGEdge>> nodeEdges;
};

class FORCEPlacer {
  public:
    FORCEPlacer(std::vector<FormulaInfo>& formulas);
    FORCEPlacer(FormulaInfo &formula);

    int calculateGraphTotalSpan();
    std::vector<FormulaInfo> orderFormulasWithPlacement(std::vector<FormulaInfo> formulas, std::vector<HGNode> placement);
    FormulaInfo orderClausesWithPlacement(FormulaInfo formula, std::vector<HGNode> placement);
    std::vector<HGNode> findPlacement(bool output = false);
  protected:
    void createGraphEdgesFromNodes();

    const double iterationConstant = 10;
    const double spanEpsilon = 200;
    std::map<int, std::set<HGNode>> varNodes;
    HGGraph graph;
};

#endif // CODE_FORCEPLACER_H
