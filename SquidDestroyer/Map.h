#pragma once

#include "SquidDestroyer/types.h"

#include <vector>

class Map {
public:
	const int width, height;

	Map(int width, int height);

	void set(ConstHexCellRef cell);
	bool validCoordinate(ConstPosRef pos) const;
	ConstHexCellRef get(ConstPosRef pos) const;
	HexCellList getNeighbors(ConstHexCellRef cell) const;
	HexCellList getAll() const;

	friend class World;

private:
	vector2D<HexCell> cells;
	inline ConstPosRef convertPos(ConstPosRef pos) const noexcept
	{
		return Pos{ pos.q, pos.r + (pos.q - (pos.q & 1)) / 2 };
	}
};
