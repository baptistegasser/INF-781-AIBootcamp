#include "Graph.h"

#include <algorithm>

Graph::Graph(const size_type maxCapacity)
{
	vertices.reserve(maxCapacity);
	adjacencyList.resize(maxCapacity);
}

Graph::size_type Graph::size() const noexcept
{
	return vertices.size();
}

void Graph::addNode(Node node)
{
	// Design choice : ignore Nodes already in the graph
	if (contains(node))
		return;

	//if (vertices.size() >= vertices.capacity()) {
	//	throw Error::MAX_CAPACITY_REACHED;
	//}

	vertices.push_back(node);
}

void Graph::addEdge(Node src, Node dest, NodeWeight weight) noexcept
{
	NodeID srcID  = getNodeID(src),
		   destID = getNodeID(dest);

	adjacencyList[srcID].push_back({ destID, weight });
}

Graph::NodeWeight Graph::getCost(Node src, Node dest) const
{
	NodeID srcID  = getNodeID(src),
           destID = getNodeID(dest);

	return getCost(srcID, destID);
}

Graph::NodeList Graph::getNeighbors(Node src) const
{
	std::function<Node(Edge)> edgeToNode = [this](Edge p) {
		return vertices[p.first];
	};
	auto rawNeighbors = adjacencyList[getNodeID(src)];
	return map(rawNeighbors.begin(), rawNeighbors.end(), edgeToNode);
}

Graph::NodeID Graph::getNodeID(Node node) const
{
	auto it = std::find(vertices.begin(), vertices.end(), node);

	if (it == vertices.end()) {
		throw Error::INVALID_NODE;
	}
	else {
		return it - vertices.begin();
	}
}

Graph::Node Graph::getNode(const NodeID id) const noexcept
{
	return vertices[id];
}


Graph::NodeWeight Graph::getCost(NodeID src, NodeID dest) const
{
	auto isDestNode = [&dest](auto edge) {
		return edge.first == dest;
	};

	auto rawNeighbors = adjacencyList[src];
	auto it = std::find_if(rawNeighbors.begin(), rawNeighbors.end(), isDestNode);

	if (it == rawNeighbors.end()) {
		throw Error::NODE_NOT_NEIGHBOR;
	}
	else {
		return (*it).second;
	}
}

Graph::NodeIDList Graph::getNeighborsIDs(const NodeID src) const noexcept
{
	std::function<NodeID(Edge)> edgeToNode = [this](Edge p) {
		return p.first;
	};
	auto rawNeighbors = adjacencyList[src];
	return map(rawNeighbors.begin(), rawNeighbors.end(), edgeToNode);
}

bool Graph::contains(Node node) const noexcept
{
	return std::find(vertices.begin(), vertices.end(), node) != vertices.end();
}
