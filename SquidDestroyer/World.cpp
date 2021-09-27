#include "World.h"

#include <algorithm>

World::World()
{
	this->this_ptr = std::make_shared<World>(this);
}

void World::addNPC(NPC& npc) noexcept
{
	npcs.push_back(npc);
	npc.setWorld(this_ptr);
}

void World::setMap(std::unique_ptr<Map> _map) noexcept
{
	this->map = std::move(_map);
}

void World::calcGraph() noexcept
{
	// To save memory, calculate the needed size
	std::size_t size = 0;
	for (const auto& row : map->cells) {
		size += std::count_if(row.begin(), row.end(), this->isWallkable);
	}

	graph = std::make_unique<Graph>(size);

	map->cells.for_each([this](ConstHexCellRef cell) {
		graph->addNode(cell.pos);
		auto neighbor = map->getNeighbors(cell);

		auto addEdge = [this, &cell](ConstHexCellRef dest) {
			graph->addEdge(cell.pos, dest.pos, 1.0);
		};

		for_each_if(neighbor.begin(), neighbor.end(), &isWallkable, addEdge);
	});
}

std::vector<NPC>& World::getNPCs() noexcept
{
	return npcs;
}

const Graph& World::getGraph() const noexcept
{
	return *graph;
}

bool World::isWallkable(ConstHexCellRef cell) const noexcept {
	return cell.type != EHexCellType::Forbidden;
}

bool World::isWallkable(ConstPosRef pos) const noexcept {
	return isWallkable(map->get(pos));
}

bool World::canMove(ConstPosRef pos) const noexcept
{
	// cell is not forbidden
	if (!isWallkable(pos))
		return false;

	// Cant move if npc on their
	for (const NPC& npc : npcs) {
		if (npc.pos() == pos)
			return false;
	}

	return true;
}

bool World::cellIs(ConstPosRef cell, const EHexCellType& type) const noexcept
{
	
}

EHexCellDirection World::getMoveDir(ConstPosRef src, ConstPosRef dest) const noexcept
{
	
}

/*

Graph<HexCell> mapToGraph(const Map& map)
{
	int size = 0;
	for (const auto& row : map.cells) {
		size += std::count_if(row.begin(), row.end(), [](auto& cell) {
			return cell.type != EHexCellType::Forbidden;
		});
	}

	Graph<HexCell> graph{ size };

	for (const auto& row : map.cells) {
		for (const auto& cell : row) {
			if (cell.type != EHexCellType::Forbidden) {
				auto id = graph.addNode(cell);

				for (auto neighbor : map.getNeighbors(cell)) {
					if (neighbor.type != EHexCellType::Forbidden) {
						auto neighborID = graph.addNode(neighbor);
						graph.addEdge(id, neighborID, 1.0);
					}
				}
			}
		}
	}

	return graph;
}
*/