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
	using Node = Pos;
	using NodeID = size_type;
	using NodeWeight = float;
	using NodeList = std::vector<Node>;
	using NodeIDList = std::vector<NodeID>;
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
	size_type size() const noexcept;

	void addNode(Node node);
	void addEdge(Node src, Node dest, NodeWeight weight) noexcept;

	NodeWeight getCost(Node src, Node dest) const;
	NodeList getNeighbors(Node src) const;

	// Operations by id
	NodeID getNodeID(Node node) const;
	Node getNode(const NodeID id) const noexcept;
	NodeWeight getCost(NodeID src, NodeID dest) const;
	NodeIDList getNeighborsIDs(const NodeID src) const noexcept;

private:
	bool contains(Node node) const noexcept;

public:
	// TODO delete
	void print(Logger& logger) const noexcept {
		for (auto& row : adjacencyList) {
			for (auto pair : row) {
				logger.Logf("%d, ", pair.first);
			}
			logger.Log("");
		}
	}
};
