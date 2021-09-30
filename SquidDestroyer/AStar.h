#pragma once

#include "Graph.h"

#include <queue>

namespace AStar
{
	// Store data about a Node during exploration in AStar
	struct NodeRecord {
		Graph::NodeID node, previous;
		float costSoFar, estimatedCost;
		enum class Status {
			OPEN,
			CLOSED,
			UNVISITED
		} status;

		NodeRecord()
			: node{ 0 },
			previous{ 0 },
			costSoFar{ 0.f },
			estimatedCost{ 0.f },
			status{ Status::UNVISITED }
		{}
	};

	// Heurisitc functor, used to calculate an heuristic value from a node to a
	// goal node during AStar search, default implementation is a Manhattan distance
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

PosList searchPathAStar(const Graph& graph, const Graph::Node& start, const Graph::Node& goal, const AStar::Heuristic& heuristic) {
	using namespace AStar;
	using Status = NodeRecord::Status;

	std::vector<NodeRecord> records{ graph.size() };

	// A sorted priority queue that put the node with the lowest estimated cost fist
	auto sortByEstimate = [&records](Graph::NodeID lhs, Graph::NodeID rhs) {
		return records[lhs].estimatedCost > records[rhs].estimatedCost;
	};
	std::priority_queue<Graph::NodeID, std::vector<Graph::NodeID>, decltype(sortByEstimate)> open (sortByEstimate);

	const Graph::NodeID
		startID = graph.getNodeID(start),
		goalID = graph.getNodeID(goal);

	// Init the algorithm with the start node
	records[startID].node = startID;
	records[startID].previous = startID;
	records[startID].status = Status::OPEN;
	records[startID].estimatedCost = heuristic(start);
	open.emplace(startID);

	Graph::NodeID current = startID;
	while (!open.empty()) {
		// Get the best node
		current = open.top();
		open.pop();

		// AStar suppose that the heuristic is good engouht that
		if (current == goalID) {
			break;
		}

		auto neigbors = graph.getNeighborsIDs(current);
		for (auto next : neigbors) {
			float nextCost = records[current].costSoFar + graph.getCost(current, next);
			float nextHeuristic = 0;

			switch (records[next].status)
			{
			case Status::CLOSED:
				if (records[next].costSoFar <= nextCost) continue; // not a better score than before, ignore
				nextHeuristic = records[next].estimatedCost - records[next].costSoFar;
				break;
			case Status::OPEN:
				if (records[next].costSoFar <= nextCost) continue; // not a better score than before, ignore
				nextHeuristic = records[next].estimatedCost - records[next].costSoFar;
				break;
			case Status::UNVISITED:
				records[next].node = next;
				nextHeuristic = heuristic(graph.getNode(next));
				break;
			}

			// Update the nodes's record
			records[next].previous = current;
			records[next].costSoFar = nextCost;
			records[next].estimatedCost = nextCost + nextHeuristic;

			// If the node is not considered open... open it
			if (records[next].status != Status::OPEN) {
				records[next].status = Status::OPEN;
				open.push(next);
			}
		}

		records[current].status = Status::CLOSED;
	}

	PosList path;
	// If the goal was not found, return the empty list
	if (current != goalID) {
		return path;
	}

	// Follow back the nodes
	while (records[current].previous != current) {
		path.push_back(graph.getNode(current));
		current = records[current].previous;
	}
	// we went the start node in this implementation
	path.push_back(start);
	// reverse obligatory
	std::reverse(path.begin(), path.end());

	return path;
}
