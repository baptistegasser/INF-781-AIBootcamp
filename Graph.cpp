#include "Graph.h"
#include <stdexcept>
#include <algorithm>

template<typename Node>
Graph<Node>::Graph(int maxNodeCount, bool oriented)
	: oriented(oriented),
	  maxNodeCount(maxNodeCount)
{
	nodeList.reserve(maxNodeCount);
	adjacencyMatrix.reserve(maxNodeCount);
}

template<typename Node>
int Graph<Node>::getNodeID(const Node& node) const
{
	auto it = std::find(nodeList.begin(), nodeList.end(), node);

	if (it == nodeList.end())
		return -1;
	else
		return it - nodeList.begin();
}

template<typename Node>
void Graph<Node>::addNode(const Node& node)
{
	if (nodeList.size() + 1 >= maxNodeCount) {
		throw std::runtime_error("Adding a node would exceed the graph size.");
	}

	nodeList.push_back(node);
	adjacencyMatrix.push_back(std::vector());
	adjacencyMatrix[adjacencyMatrix.size() - 1].reserve(maxNodeCount);
}

template<typename Node>
void Graph<Node>::addEdge(const Node& src, const Node& dest, int weight)
{
	int srcID = getNodeId(src);
	if (srcID == -1) throw std::invalid_argument("The source node is not in the graph.");
	
	int destID = getNodeId(dest);
	if (destID == -1) throw std::invalid_argument("The destination node is not in the graph.");

	adjacencyMatrix[srcID][destID] = weight;

	if (!oriented) {
		adjacencyMatrix[destID][srcID] = weight;
	}
}

template<typename Node>
std::vector<Node> Graph<Node>::getNeighbors(const Node& node) const
{
	std::vector<Node> neighbors;

	int nodeID = getNodeId(node);
	if (nodeID == -1) throw std::invalid_argument("The node is not in the graph.");

	for (int i = 0, l = adjacencyMatrix[nodeID].size(); i < l; ++i) {
		if (adjacencyMatrix[nodeID][i] != 0) {
			neighbors.push_back(nodeList[i]);
		}
	}

	return neighbors;
}

template<typename Node>
float Graph<Node>::getCost(const Node& src, const Node& dest) const
{
	int srcID = getNodeId(src);
	if (srcID == -1) throw std::invalid_argument("The source node is not in the graph.");

	int destID = getNodeId(dest);
	if (destID == -1) throw std::invalid_argument("The destination node is not in the graph.");

	// The cost is currently the weight for now
	return adjacencyMatrix[srcID][destID];
}
