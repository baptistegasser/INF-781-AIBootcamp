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

	const static std::vector<Pos> dirs{
		{+0, -1}, {+1, -1}, {+1, 0},
		{+0, +1}, {-1, +1}, {-1, 0}
	};

	for (const auto& dir : dirs) {
		Pos pos = convertPos(cell.pos + dir);
		if (validCoordinate(pos)) {
			neighbors.push_back(cells[pos.q][pos.r]);
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


HexCellList Map::getAllOfType(EHexCellType type) const
{
	HexCellList result;

	auto isOfType = [&type](const HexCell& cell) {
		return cell.type == type;
	};

	auto addToResult = [&result](const HexCell& cell) {
		result.push_back(cell);
	};

	for (const auto& v : cells) {
		for_each_if(v.begin(), v.end(), isOfType, addToResult);
	}

	return result;
}
