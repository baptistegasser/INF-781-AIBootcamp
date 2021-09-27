#pragma once

#include "Logger.h"

#include "SquidDestroyer/types.h"

#include <vector>

/**
* Representation of a directed weighted graph of axial cells positions.
*/
class Graph {
public:
	using size_type = std::size_t;
	using Node = ConstPosRef;
	using NodeID = size_type;
	using NodeWeight = float;
	using NodeList = std::vector<Node>;
	using Edge = std::pair<NodeID, NodeWeight>;

	enum class Error {
		MAX_CAPACITY_REACHED,
		INVALID_NODE,
		NODE_NOT_NEIGHBOR
	};

private:
	NodeList vertices;
	vector2D<Edge> adjacencyList;

public:
	Graph(const size_type maxCapacity);

	void addNode(Node node);
	void addEdge(Node src, Node dest, NodeWeight weight) noexcept;

	NodeWeight getCost(Node src, Node dest);
	NodeList getNeighbors(Node src);

private:
	NodeID getNodeID(Node node) const;
	bool contains(Node node) const noexcept;

public:
	// TODO delete
	void print(Logger& logger) const noexcept {
		
	}
};
