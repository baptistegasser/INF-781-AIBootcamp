#pragma once

#include "Globals.h"

#include <vector>

struct HexCell {
	int q, r;
	EHexCellType type;

	HexCell();
	HexCell(const STileInfo& tileInfo);
	bool operator==(const HexCell& other) const
	{
		return q == other.q && r == other.r;
	}
};

class Map {
public:
	using HexCellList = std::vector<HexCell>;
	const int width, height;

	Map(int width, int height);

	void set(HexCell& cell);
	bool validCoordinate(int q, int r);
	HexCell& get(int q, int r);
	HexCellList getNeighbors(const HexCell& cell);
	HexCellList getAll();

private:
	std::vector<HexCellList> cells;
};