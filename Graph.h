#pragma once

#include <vector>

/**
* Representation of a (un)directed weighted graph.
*/
template <typename Node>
class Graph
{
private:
	std::vector<Node> nodeList;
	std::vector<std::vector<int>> adjacencyMatrix;

	int getNodeID(const Node& node) const;

public:
	const bool oriented;
	const int maxNodeCount;

	Graph(int maxNodeCount, bool oriented = false);

	void addNode(const Node& node);
	void addEdge(const Node& src, const Node& dest, int weight = 1);
	std::vector<Node> getNeighbors(const Node& node) const;
	float getCost(const Node& src, const Node& dest) const;
};


template <typename Node, typename HeuristicFunc>
std::vector<Node> AStar(const Node& start, const Node& end, const HeuristicFunc& heuristic)
{

}
