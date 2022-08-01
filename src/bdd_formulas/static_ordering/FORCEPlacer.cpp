//
// Created by Maxim.Popov on 20.07.2022.
//

#include "FORCEPlacer.h"
#include "../../utils/progress_bar.h"
#include "spdlog/spdlog.h"
#include <climits>
#include <cmath>
#include <numeric>

void FORCEPlacer::createGraphEdgesFromNodes() {
    for (auto varNode : varNodes) {
        HGEdge edge{(int)graph.edges.size(), varNode.second};
        for (HGNode node : varNode.second) {
            graph.nodeEdges[node].insert(edge);
        }
        graph.edges.insert(edge);
    }
}

FORCEPlacer::FORCEPlacer(std::vector<FormulaInfo> &formulas) {
    for (FormulaInfo &formula : formulas) {
        HGNode node{formula.id};
        graph.nodes.push_back(node);

        for (int var : formula.containedVars) {
            varNodes[var].insert(node);
        }
    }

    this->createGraphEdgesFromNodes();
}

FORCEPlacer::FORCEPlacer(FormulaInfo &formula) {
    for (int i = 0; i < formula.clauses.size(); i++) {
        HGNode node{i};
        graph.nodes.push_back(node);

        for (int var : formula.clauses[i]) {
            varNodes[var].insert(node);
        }
    }

    this->createGraphEdgesFromNodes();
}

std::vector<FormulaInfo>
FORCEPlacer::orderFormulasWithPlacement(std::vector<FormulaInfo> formulas,
                                        std::vector<HGNode> placement) {
    std::vector<FormulaInfo> orderedFormulas;
    for (int i = 0; i < placement.size(); i++) {
        FormulaInfo nodeFormula = *std::find_if(
            formulas.begin(), formulas.end(),
            [&](FormulaInfo info) { return info.id == placement[i].id; });
        orderedFormulas.push_back(nodeFormula);
    }
    return orderedFormulas;
}

FormulaInfo
FORCEPlacer::orderClausesWithPlacement(FormulaInfo formula,
                                       std::vector<HGNode> placement) {
    FormulaInfo orderedFormula = formula;
    orderedFormula.symbols = {};
    orderedFormula.clauses = {};
    for (int i = 0; i < placement.size(); i++) {
        orderedFormula.clauses.push_back(formula.clauses[placement[i].id]);
    }
    for (auto clause : orderedFormula.clauses) {
        for (int symbol : clause) {
            orderedFormula.symbols.push_back(symbol);
        }
        orderedFormula.symbols.push_back(0);
    }

    return orderedFormula;
}

int FORCEPlacer::calculateGraphTotalSpan() {
    int totalSpan = 0;
    for (HGEdge edge : graph.edges) {
        int maxIndex = -1;
        int minIndex = INT_MAX;
        for (HGNode node : edge.nodes) {
            int nodeIndex =
                std::find(graph.nodes.begin(), graph.nodes.end(), node) -
                graph.nodes.begin();
            if (nodeIndex > maxIndex) {
                maxIndex = nodeIndex;
            }
            if (nodeIndex < minIndex) {
                minIndex = nodeIndex;
            }
        }
        totalSpan += maxIndex - minIndex;
    }
    return totalSpan;
}

std::vector<HGNode> FORCEPlacer::findPlacement(bool output) {
    int iterationsNum = std::round(iterationConstant * log(graph.nodes.size()));
    ProgressBar bar(iterationsNum);

    if (output)
        spdlog::info("Finding placement with FORCE");
    int totalSpan = calculateGraphTotalSpan();
    int newTotalSpan = totalSpan;
    for (int i = 0;
         i < iterationsNum || newTotalSpan - totalSpan > spanEpsilon ||
         newTotalSpan < totalSpan;
         i++) {
        totalSpan = newTotalSpan;
        if (output) {
            if (i < iterationsNum) {
                bar.update(i + 1, fmt::format("Total span: {}", totalSpan));
            } else {
                bar.update(i + 1,
                           fmt::format("(Continuing iterations) Total span: {}",
                                       totalSpan));
            }
        }

        std::map<HGEdge, double> cogs;
        for (HGEdge edge : graph.edges) {
            std::vector<int> nodeIndices;
            for (HGNode node : edge.nodes) {
                int nodeIndex = std::distance(
                    graph.nodes.begin(),
                    std::find(graph.nodes.begin(), graph.nodes.end(), node));
                nodeIndices.push_back(nodeIndex);
            }
            double cog =
                ((double)std::reduce(nodeIndices.begin(), nodeIndices.end())) /
                edge.nodes.size();
            cogs[edge] = cog;
        }

        std::map<HGNode, double> newIndices;
        for (HGNode node : graph.nodes) {
            std::vector<double> nodeEdgesCog;
            for (HGEdge edge : graph.nodeEdges[node]) {
                nodeEdgesCog.push_back(cogs[edge]);
            }
            double newIndex = ((double)std::reduce(nodeEdgesCog.begin(),
                                                   nodeEdgesCog.end())) /
                              graph.nodeEdges[node].size();
            newIndices[node] = newIndex;
        }

        std::sort(graph.nodes.begin(), graph.nodes.end(),
                  [&](HGNode node1, HGNode node2) {
                      return newIndices[node1] < newIndices[node2];
                  });

        newTotalSpan = calculateGraphTotalSpan();
    }
    return graph.nodes;
}