#pragma once

#include "SquidDestroyer/types.h"
#include "Graph.h"

#include <functional>
#include <map>
#include <queue>

class Heuristic {
	const Graph::Node& goal;

public:
	Heuristic(const Graph::Node& goal) : goal{ goal }
	{}

	virtual float operator()(const Graph::Node& src) const noexcept
	{
		return (abs(src.q - goal.q)
			+ abs(src.q + src.r - goal.q - goal.r)
			+ abs(src.r - goal.r)) / 2.f;
	}
};

struct NodeRecord {
	Graph::Node previousNode;
	float costSoFar;
};

using PrioritizedNode = std::pair<float, Graph::Node>;

namespace std {
	template<> struct less<Graph::Node>
	{
		bool operator() (const Graph::Node& lhs, const Graph::Node& rhs) const
		{
			return lhs.q < rhs.q && lhs.r < rhs.r;
		}
	};
	template<> struct greater<PrioritizedNode>
	{
		bool operator() (const PrioritizedNode& lhs, const PrioritizedNode& rhs) const
		{
			return lhs.first > rhs.first;
		}
	};
}



class Record {
	float costSoFar = 0.f;
	float estimatedCostSoFar = 0.f;
	enum class Status {
		OPEN,
		CLOSED,
		UNVISITED
	} status;
};

PosList searchPathAStar(const Graph& graph, const Graph::Node& start, const Graph::Node& goal, Heuristic heuristic)
{
	std::vector<Record> records{ graph.size() };
	
	const startID = graph.getID(start);




	std::map<Graph::Node, NodeRecord> map;
	NodeRecord startRecord{ start, 0 };
	map[start] = startRecord;

	auto mapContains = [&map](const Graph::Node& node) -> bool {
		return map.find(node) != map.end();
	};

	std::priority_queue<PrioritizedNode, std::vector<PrioritizedNode>, std::greater<PrioritizedNode>> queue;
	PrioritizedNode p{ 0.f, start };
	queue.push(p);

	int i = 0;
	while (!queue.empty()) {
		++i;
		Graph::Node current = queue.top().second;
		queue.pop();

		if (current == goal) {
			++i;
			break;
		}

		auto neighbors = graph.getNeighbors(current);
		for (const Graph::Node& next : neighbors) {
			float nextCost = map[current].costSoFar + graph.getCost(current, next);

			if (!mapContains(next) || nextCost < map[next].costSoFar) {
				map[next] = NodeRecord { current, nextCost };
				PrioritizedNode pn{ nextCost + heuristic(next), next };
				queue.push(pn);
			}
		}
	}

	PosList path;

	Graph::Node current = goal;
	while (true) {
		path.push_back(current);
		if (map[current].previousNode == current) break;
		current = map[current].previousNode;
	}

	std::reverse(path.begin(), path.end());

	return path;
}
