#pragma once

#include "types.h"

#include <vector>

class GameObject {
public:
	virtual const Pos pos() const noexcept
	{
		return { 0, 0 };
	}
	enum class Type {
		Unknown,
		NPC
	};
	virtual const Type type() const noexcept
	{
		return Type::Unknown;
	}
};

using GameObjectList = std::vector<GameObject>;
