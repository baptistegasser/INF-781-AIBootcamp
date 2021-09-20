#include "SquidDestroyer/Map.h"

HexCell::HexCell()
	: HexCell(0, 0, EHexCellType::Invalid)
{}

HexCell::HexCell(int q, int r, EHexCellType type)
	: q(q),
	  r(r),
	  type(type)
{}
