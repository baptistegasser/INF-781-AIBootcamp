#include "SquidDestroyer/Map.h"
#include <algorithm>
#include <iterator>

inline bool even(int i) noexcept
{
	return !(i & 1);
}


HexCell::HexCell()
	: q{ 0 },
	  r{ 0 },
	  type{ EHexCellType::Forbidden }
{}

HexCell::HexCell(const STileInfo& tileInfo)
	: q{ tileInfo.q },
	  r{ tileInfo.r },
	  type{ tileInfo.type }
{}

inline int adjustR(const int q, const int r) noexcept
{
	return r+(q-(q&1))/2;
}

HexCell& Map::get(int q, int r)
{
	return cells[q][adjustR(q, r)];
}

Map::Map(int _width, int _height)
	: width(_width),
	  height(_height)
{
	cells.resize(height);
	for (auto& row : cells) {
		row.resize(width);
	}
}

void Map::set(HexCell& cell)
{
	cells[cell.q][adjustR(cell.q, cell.r)] = cell;
}

bool Map::validCoordinate(int q, int r)
{
	int r2 = adjustR(q, r);
	return r2 >= 0 && r2 < width && q >= 0 && q < height;
}

Map::HexCellList Map::getNeighbors(const HexCell& cell)
{
	HexCellList neighbors;

	const static std::vector<std::pair<int, int>> dirs{
		{+0, -1}, {+1, -1}, {+1, 0},
		{+0, +1}, {-1, +1}, {-1, 0}
	};

	for (const auto& dir : dirs) {
		int q = cell.q + dir.first,
			r = cell.r + dir.second;
		if (validCoordinate(q, r))
			neighbors.push_back(cells[q][r]);
	}

	return neighbors;
}

Map::HexCellList Map::getAll()
{
	HexCellList all;
	all.reserve(width*(size_t)height);

	for (const auto& v : cells) {
		copy(v.begin(), v.end(), std::back_inserter(all));
	}

	return all;
}
