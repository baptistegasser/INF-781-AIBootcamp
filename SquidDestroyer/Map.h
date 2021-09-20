#pragma once

#include "Globals.h"

#include <vector>

class HexCell {
	const int q, r;
	EHexCellType type;

public:
	HexCell();
	HexCell(int q, int r, EHexCellType type);
};

class Map {
public:
	const int width, height;

	HexCell& getCell(int q, int r);

private:
	std::vector<std::vector<HexCell>> cells;
};