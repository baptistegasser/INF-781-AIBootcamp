#pragma once

#include "Globals.h"

#include "SquidDestroyer/types.h"
#include "SquidDestroyer/World.h"

#include <memory>

class World; // forward declaration to avoir cycle between World<-->NPC

enum class State {
	Default,
	Moving,
	Blocked,
	Arrived
};

class NPC
{
public:
	const int uid;
	
private:
	State state;
	PosList path;
	Pos startPos;
	int pathPos;
	std::shared_ptr<World> world;

	const SOrder DONT_MOVE_ORDER;

public:
	NPC(int uid, Pos startPos);

	void setWorld(std::shared_ptr<World> world) noexcept;
	void setPath(const PosList& path);

	inline ConstPosRef pos() const noexcept;

	SOrder playTurn() noexcept;

private:
	void setState(const State &state) noexcept;
	SOrder handleMove() noexcept;
	SOrder handleBlocked() noexcept;

	inline ConstPosRef nextPos() const noexcept;
};
