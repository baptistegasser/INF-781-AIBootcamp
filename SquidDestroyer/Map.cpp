#include "SquidDestroyer/Map.h"
#include <algorithm>
#include <iterator>

HexCell::HexCell()
	: HexCell{ 0, 0, EHexCellType::Forbidden }
{}

HexCell::HexCell(const int q, const int r, const EHexCellType type)
	: q{ q },
	  r{ r },
	  type{ type }
{}

HexCell::HexCell(const STileInfo& tileInfo)
	: q{ tileInfo.q },
	  r{ tileInfo.r },
	  type{ tileInfo.type }
{}

namespace std {
	template <>
	struct hash<HexCell>
	{
		std::size_t operator()(const HexCell& cell) const
		{
			return (( std::hash<int>()(cell.q)
				  ^ ( std::hash<int>()(cell.r) << 1)) >> 1);
		}
	};

}

inline int adjustR(const int q, const int r) noexcept
{
	return r+(q-(q&1))/2;
}

HexCell Map::get(int q, int r) const
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

bool Map::validCoordinate(int q, int r) const
{
	return r >= 0 && r < width && q >= 0 && q < height;
}

Map::HexCellList Map::getNeighbors(const HexCell& cell) const
{
	HexCellList neighbors;

	const static std::vector<std::pair<int, int>> dirs{
		{+0, -1}, {+1, -1}, {+1, 0},
		{+0, +1}, {-1, +1}, {-1, 0}
	};

	for (const auto& dir : dirs) {
		int q = cell.q + dir.first,
			r = adjustR(q, cell.r + dir.second);
		if (validCoordinate(q, r)) {
			neighbors.push_back(cells[q][r]);
		}
	}

	return neighbors;
}

Map::HexCellList Map::getAll() const
{
	HexCellList all;
	all.reserve(width*(size_t)height);

	for (const auto& v : cells) {
		copy(v.begin(), v.end(), std::back_inserter(all));
	}

	return all;
}

Graph<HexCell> mapToGraph(const Map& map)
{
	int size = 0;
	for (const auto& row : map.cells) {
		size += std::count_if(row.begin(), row.end(), [](auto& cell) {
			return cell.type != EHexCellType::Forbidden;
		});
	}

	Graph<HexCell> graph{ size };

	for (const auto& row : map.cells) {
		for (const auto& cell : row) {
			if (cell.type != EHexCellType::Forbidden) {
				auto id = graph.addNode(cell);

				for (auto neighbor : map.getNeighbors(cell)) {
					if (neighbor.type != EHexCellType::Forbidden) {
						auto neighborID = graph.addNode(neighbor);
						graph.addEdge(id, neighborID, 1.0);
					}
				}
			}
		}
	}

	return graph;
}
