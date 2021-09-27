#include "SquidDestroyer/Map.h"

#include <algorithm>
#include <iterator>

//namespace std {
//	template <>
//	struct hash<HexCell>
//	{
//		std::size_t operator()(const HexCell& cell) const
//		{
//			return (( std::hash<int>()(cell.q)
//				  ^ ( std::hash<int>()(cell.r) << 1)) >> 1);
//		}
//	};
//
//}

ConstHexCellRef Map::get(ConstPosRef pos) const
{
	auto converted = convertPos(pos);
	return cells[converted.q][converted.r];
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

void Map::set(ConstHexCellRef cell)
{
	auto converted = convertPos(cell.pos);
	cells[converted.q][converted.r] = cell;
}

bool Map::validCoordinate(ConstPosRef pos) const
{
	return pos.r >= 0 && pos.r < width && pos.q >= 0 && pos.q < height;
}

HexCellList Map::getNeighbors(ConstHexCellRef cell) const
{
	HexCellList neighbors;

	const static std::vector<std::pair<int, int>> dirs{
		{+0, -1}, {+1, -1}, {+1, 0},
		{+0, +1}, {-1, +1}, {-1, 0}
	};

	for (const auto& dir : dirs) {
		auto pos = convertPos(cell.pos);
		if (validCoordinate(pos)) {
			neighbors.push_back(get(pos));
		}
	}

	return neighbors;
}

HexCellList Map::getAll() const
{
	HexCellList all;
	all.reserve(width*(size_t)height);

	for (const auto& v : cells) {
		copy(v.begin(), v.end(), std::back_inserter(all));
	}

	return all;
}
