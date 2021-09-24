#pragma once

#include "Globals.h"

#include <vector>
#include "Graph.h"

struct HexCell {
	int q, r;
	EHexCellType type;

	HexCell();
	HexCell(const int q, const int r, const EHexCellType type);
	HexCell(const STileInfo& tileInfo);
	bool operator==(const HexCell& other) const
	{
		return q == other.q && r == other.r;
	}
	bool operator!=(const HexCell& other) const
	{
		return !(*this == other);
	}
};

class Map {
public:
	using HexCellList = std::vector<HexCell>;
	const int width, height;

	Map(int width, int height);

	void set(HexCell& cell);
	bool validCoordinate(int q, int r) const;
	HexCell get(int q, int r) const;
	HexCellList getNeighbors(const HexCell& cell) const;
	HexCellList getAll() const;

	friend Graph<HexCell> mapToGraph(const Map& map);

private:
	std::vector<HexCellList> cells;
};
