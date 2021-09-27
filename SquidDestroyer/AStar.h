#pragma once

#include "SquidDestroyer/types.h"
#include "Graph.h"

template <typename HeuristicFunc>
class AStar {
	const Graph& graph;
	const HeuristicFunc& func;

public:
	AStar(const Graph& graph, const HeuristicFunc& func);
	PosList calcPath(ConstPosRef src, ConstPosRef dest) const noexcept;
};
