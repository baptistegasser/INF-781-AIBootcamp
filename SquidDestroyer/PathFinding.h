#pragma once

#include "SquidDestroyer/Graph.h"
#include <vector>
#include <queue>
#include <unordered_map>

template <typename Node, typename HeuristicFunc>
std::vector<Node> AStar(const Graph<Node>& graph, const Node& start, const Node& goal, const HeuristicFunc& heuristic)
{
    // Priority queue that associate each node with it's cost and sort them by std::greater instead of default std::less
    using PQElement = std::pair<double, Node>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

    std::unordered_map<Node, Node> previous;
    std::unordered_map<Node, double> currentCost;

    elements.emplace(0, start);
    currentCost[start] = 0;

    while (!elements.empty()) {
        Node current = elements.top().second;
        elements.pop();

        if (current == goal) break;

        for (const Node &neighbor : graph.getNeighbors(current)) {
            double neighborCost = currentCost[current] + graph.getCost(current, neighbor);
            // First time exploring the node or coming back with a better score
            if (currentCost.find(neighbor) == currentCost.end() || neighborCost < currentCost[neighbor]) {
                currentCost[neighbor] = neighborCost;
                double priority = neighborCost + heuristic(current, neighbor);
                elements.emplace(priority, neighbor);
                previous[neighbor] = current;
            }
        }
    }

    std::vector<Node> path;
    Node current = goal;
    while (current != start) {
        path.push_back(current);
        current = previous[current];
    }
    path.push_back(start);

    std::reverse(path.begin(), path.end());

    return path;
}
