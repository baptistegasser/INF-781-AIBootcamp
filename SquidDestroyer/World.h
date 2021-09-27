#pragma once

#include "Globals.h"

#include "Map.h"
#include "Graph.h"
#include "NPC.h"

#include <vector>
#include <memory>

class World {
private:
	std::unique_ptr<Map> map;
	std::unique_ptr<Graph> graph;
	std::vector<NPC> npcs;
	std::shared_ptr<World> this_ptr;

public:
	World();
	void addNPC(NPC& npc) noexcept;
	void setMap(std::unique_ptr<Map> map) noexcept;
	void calcGraph() noexcept;

	std::vector<NPC>& getNPCs() noexcept;
	const Graph& getGraph() const noexcept;

	bool isWallkable(ConstHexCellRef cell) const noexcept;
	bool isWallkable(ConstPosRef pos) const noexcept;


	bool canMove(ConstPosRef cell) const noexcept;
	bool cellIs(ConstPosRef cell, const EHexCellType& type) const noexcept;
	EHexCellDirection getMoveDir(ConstPosRef src, ConstPosRef dest) const noexcept;
};
