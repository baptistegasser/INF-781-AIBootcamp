#pragma once

#include "Globals.h"
#include "InitData.h"

#include "Map.h"
#include "Graph.h"

#include <vector>
#include <memory>

class World {
private:
	std::unique_ptr<Map> map;
	std::unique_ptr<Graph> graph;

public:
	void parseMap(const SInitData& initData) noexcept;
	void setMap(std::unique_ptr<Map> map) noexcept;
	void calcGraph() noexcept;

	const Graph& getGraph() const noexcept;

	bool isWallkable(ConstPosRef pos) const noexcept;
	bool isWallkable(ConstHexCellRef cell) const noexcept;

	bool canMove(ConstPosRef cell) const noexcept;
	bool cellIs(ConstPosRef cell, const EHexCellType& type) const noexcept;
	EHexCellDirection getMoveDir(ConstPosRef src, ConstPosRef dest) const noexcept;
};
