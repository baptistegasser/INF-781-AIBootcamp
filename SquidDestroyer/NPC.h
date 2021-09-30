#pragma once

#include "Globals.h"

#include "SquidDestroyer/types.h"
#include "SquidDestroyer/GameObject.h"
#include "SquidDestroyer/World.h"

#include <memory>

enum class State {
	Default,
	Moving,
	Blocked,
	Arrived
};

class NPC : public GameObject
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
	const Type type() const noexcept { return Type::NPC; }

	void setWorld(std::shared_ptr<World> world) noexcept;
	void setPath(const PosList& path);

	const Pos pos() const noexcept;

	SOrder playTurn() noexcept;

private:
	void setState(const State &state) noexcept;
	SOrder handleMove() noexcept;
	SOrder handleBlocked() noexcept;

	ConstPosRef nextPos() const noexcept;
};
