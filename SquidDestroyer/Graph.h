#pragma once

#include <vector>
#include <algorithm>
#include "Logger.h"

/**
* Representation of a directed weighted graph.
*/
template <typename Node>
class Graph
{
public:
	using NodeID = std::size_t;
	using NodeList = std::vector<Node>;

	enum class Error {
		MAX_CAPACITY_REACHED
	};

private:
	int maxCapacity;
	NodeList nodes;
	std::vector<std::vector<float>> adjacency;

public:
	Graph(const int _maxCapacity)
		: maxCapacity(_maxCapacity)
	{
		adjacency.resize(maxCapacity);
		for (auto &row : adjacency)
			row.resize(maxCapacity);
	}
	NodeID addNode(const Node& node)
	{
		{ // Node already inside the graph
			if (contains(node))
				return getID(node);
		}
		
		const NodeID ID = nodes.size();
		if (ID >= maxCapacity) {
			throw Error::MAX_CAPACITY_REACHED;
		}

		nodes.push_back(node);
		return ID;
	}
	Node getNode(const NodeID id) const
	{
		return nodes[id];
	}
	void addEdge(const NodeID srcID, const NodeID destID, float cost = 1.0)
	{
		adjacency[srcID][destID] = cost;
	}
	double getCost(const Node &src, const Node &dest) const
	{
		int srcID = getID(src),
			destID = getID(dest);

		float cost = adjacency[srcID][destID];

		// case where no edges between nodes
		if (cost == 0)
			return 1000000;
		else
			return cost;
	}

	NodeID getID(const Node& node) const noexcept
	{
		return std::find(nodes.begin(), nodes.end(), node) - nodes.begin();
	}
	bool contains(const Node& node) const noexcept
	{
		return std::find(nodes.begin(), nodes.end(), node) != nodes.end();
	}

	NodeList getNeighbors(const Node &node) const
	{
		NodeList neighbors;

		int nodeID = getID(node);
		int currentID = 0;

		for (float weight : adjacency[nodeID]) {
			if (weight != 0) neighbors.push_back(getNode(currentID));
			++currentID;
		}

		return neighbors;
	}

	void print(Logger &logger)
	{
		for (auto& row : adjacency) {
			for (auto& cost : row)
				logger.Logf("%f,", cost);
			logger.Log("");
		}
	}
};
