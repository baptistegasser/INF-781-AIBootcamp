#pragma once

#include "Graph.h"

#include <queue>

namespace AStar
{
	struct Arg {
		int node, previous;
		float costSoFar, estimatedCost;
		enum class Status {
			OPEN,
			CLOSED,
			UNVISITED
		} status;

		Arg()
			: node{ -1 },
			previous{ -1 },
			costSoFar{ 0.f },
			estimatedCost{ 0.f },
			status{ Status::UNVISITED }
		{}
	};

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
}

namespace std
{
	template<> struct less<AStar::Arg>
	{
		bool operator() (const AStar::Arg& lhs, const AStar::Arg& rhs) const
		{
			return lhs.estimatedCost < rhs.estimatedCost;
		}
	};
}


PosList searchPathAStar(const Graph& graph, const Graph::Node& start, const Graph::Node& goal, const AStar::Heuristic& heuristic) {
	using namespace AStar;
	using Status = Arg::Status;

	std::vector<Arg> records{ graph.size() };

	auto sortByEstimate = [&records](int lhs, int rhs) {
		return records[lhs].estimatedCost < records[rhs].estimatedCost;
	};
	std::priority_queue<int, std::vector<int>, decltype(sortByEstimate)> open (sortByEstimate);

	const int startID = graph.getNodeID(start),
		goalID = graph.getNodeID(goal);

	records[startID].node = startID;
	records[startID].previous = startID;
	records[startID].status = Status::OPEN;
	records[startID].estimatedCost = heuristic(start);

	open.emplace(startID);

	int current;
	while (!open.empty()) {
		current = open.top();
		open.pop();

		if (current == goalID) {
			break;
		}

		auto neigbors = graph.getNeighborsIDs(current);
		for (auto next : neigbors) {
			float nextCost = records[current].costSoFar + graph.getCost(current, next);
			float nextHeuristic;

			switch (records[next].status)
			{
			case Status::CLOSED:
				if (records[next].costSoFar <= nextCost) continue;
				nextHeuristic = records[next].estimatedCost - records[next].costSoFar;
				break;
			case Status::OPEN:
				if (records[next].costSoFar <= nextCost) continue;
				nextHeuristic = records[next].estimatedCost - records[next].costSoFar;
				break;
			case Status::UNVISITED:
				records[next].node = next;
				nextHeuristic = heuristic(graph.getNode(next));
				break;
			}

			records[next].previous = current;
			records[next].costSoFar = nextCost;
			records[next].estimatedCost = nextCost + nextHeuristic;

			if (records[next].status != Status::OPEN) {
				records[next].status = Status::OPEN;
				open.push(next);
			}
		}

		records[current].status = Status::CLOSED;
	}

	PosList path;

	if (current != goalID) {
		return path;
	}

	while (records[current].previous != current) {
		path.push_back(graph.getNode(current));
		current = records[current].previous;
	}

	path.push_back(start);

	std::reverse(path.begin(), path.end());

	return path;
}























/*


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
*/