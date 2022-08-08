//
// Created by Maxim.Popov on 03.08.2022.
//

#ifndef CODE_HYPERGRAPH_H
#define CODE_HYPERGRAPH_H

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
    std::map<HGNode, std::set<HGEdge>> nodeEdgesCache;
};

#endif // CODE_HYPERGRAPH_H
