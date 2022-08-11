//
// Created by Maxim.Popov on 03.08.2022.
//

#include "FORCEPlacer.h"
#include "../../utils/progress_bar.h"
#include "hypergraph.h"
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

void FORCEPlacer::initGraphWithRuleset(RulesetInfo& rulesetInfo) {

    for (int i = 1; i <= rulesetInfo.variableAmount; i++) {
        HGNode node{i};
        graph.nodes.push_back(node);
    }

    for (FormulaInfo& formula : rulesetInfo.formulas) {
        for (auto clause : formula.clauses) {
            std::set<HGNode> nodesInEdge;
            for (int var : clause) {
                nodesInEdge.insert(graph.nodes.at(std::abs(var)-1));
            }
            HGEdge edge{(int)graph.edges.size(), nodesInEdge};
            for (HGNode node : nodesInEdge) {
                graph.nodeEdges[node].insert(edge);
            }
            graph.edges.insert(edge);
        }
    }
}

void FORCEPlacer::initGraphWithRulesetModifiedFORCE(RulesetInfo& rulesetInfo) {
    for (FormulaInfo &formula : rulesetInfo.formulas) {
        HGNode node{formula.id};
        graph.nodes.push_back(node);

        for (int var : formula.containedVars) {
            varNodes[var].insert(node);
        }
    }

    this->createGraphEdgesFromNodes();
}

void FORCEPlacer::initGraphWithFormula(FormulaInfo &formula) {
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

std::vector<int> FORCEPlacer::orderVariablesWithPlacement(std::vector<HGNode> placement) {
    std::vector<int> orderedVariables;
    for (HGNode node : placement) {
        orderedVariables.push_back(node.id);
    }
    return orderedVariables;
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

    std::vector<int> lastTotalSpans = {calculateGraphTotalSpan()};
    for (int i = 0;
         i < iterationsNum
         && (lastTotalSpans.size() < 6 || lastTotalSpans[lastTotalSpans.size()-1] != lastTotalSpans[lastTotalSpans.size()-6]);
         i++) {

        if (output) {
            if (i < iterationsNum) {
                bar.update(i + 1, fmt::format("Total span: {}", lastTotalSpans[lastTotalSpans.size()-1]));
            } else {
                bar.update(i + 1,
                           fmt::format("(Continuing iterations) Total span: {}",
                                       lastTotalSpans[lastTotalSpans.size()-1]));
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

        lastTotalSpans.push_back(calculateGraphTotalSpan());
    }
    return graph.nodes;
}