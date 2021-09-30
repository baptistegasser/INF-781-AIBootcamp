#include "World.h"

#include <algorithm>

void World::parseMap(const SInitData& initData) noexcept
{
	map = std::make_unique<Map>(initData.colCount, initData.rowCount);

	std::for_each(initData.tileInfoArray, initData.tileInfoArray + initData.tileInfoArraySize, [this](auto tileInfo) {
		HexCell cell{ {tileInfo.q, tileInfo.r}, tileInfo.type };
		map->set(cell);
	});
}

void World::setMap(std::unique_ptr<Map> _map) noexcept
{
	this->map = std::move(_map);
}

void World::calcGraph() noexcept
{
	auto isWallkableCallback = [this](ConstHexCellRef cell) {
		return isWallkable(cell);
	};

	// To save memory, calculate the needed size
	std::size_t size = 0;
	for (const auto& row : map->cells) {
		size += std::count_if(row.begin(), row.end(), isWallkableCallback);
	}

	graph = std::make_unique<Graph>(size);

	for (const auto& row : map->cells) {
		for (ConstHexCellRef cell : row) {
			if (!isWallkable(cell)) continue;

			graph->addNode(cell.pos);
			auto neighbors = map->getNeighbors(cell);
			for (ConstHexCellRef neighbor : neighbors) {
				if (!isWallkable(neighbor)) continue;
				graph->addNode(neighbor.pos);
				graph->addEdge(cell.pos, neighbor.pos, 1.0);
			}
		}
	}
}

const Map& World::getMap() const noexcept
{
	return *map;
}

const Graph& World::getGraph() const noexcept
{
	return *graph;
}

bool World::isWallkable(ConstPosRef pos) const noexcept {
	return isWallkable(map->get(pos));
}

bool World::isWallkable(ConstHexCellRef cell) const noexcept {
	return cell.type != EHexCellType::Forbidden;
}

bool World::canMove(ConstPosRef pos) const noexcept
{
	// cell is not forbidden
	if (!isWallkable(pos))
		return false;

	// Cant move if npc on their
	// TODO
	for (auto npc : getObjects(GameObject::Type::NPC)) {
		if (npc->pos() == pos)
			return false;
	}

	return true;
}

bool World::cellIs(ConstPosRef cell, const EHexCellType& type) const noexcept
{
	return map->get(cell).type == type;
}

EHexCellDirection World::getMoveDir(ConstPosRef src, ConstPosRef dest) const noexcept
{
	if (src == dest)
		return EHexCellDirection::CENTER;

	if (src.q == dest.q) {
		if (dest.r < src.r)
			return EHexCellDirection::W;
		else
			return EHexCellDirection::E;
	}
	else if (src.q < dest.q) {
		if (dest.r == src.r)
			return EHexCellDirection::SE;
		else
			return EHexCellDirection::SW;
	}
	else {
		if (dest.r == src.r)
			return EHexCellDirection::NW;
		else
			return EHexCellDirection::NE;
	}
}

void World::addObject(std::shared_ptr<GameObject> object_ptr) noexcept
{
	gameObjects.push_back(object_ptr);
}

std::vector<std::shared_ptr<GameObject>> World::getObjects(GameObject::Type type) const noexcept
{
	std::vector<std::shared_ptr<GameObject>> objects;
	for (const auto& o : gameObjects) {
		if (o->type() == type) {
			objects.push_back(o);
		}
	}
	return objects;
}
