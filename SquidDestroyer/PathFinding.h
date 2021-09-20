#pragma once

#include "SquidDestroyer/Graph.h"
#include <vector>

class HexaNode {
public:
	const int q, r, ID;
};

template <typename Node, typename HeuristicFunc>
std::vector<Node> AStar(const Graph<Node>& graph, const Node& start, const Node& end, const HeuristicFunc& heuristic);


