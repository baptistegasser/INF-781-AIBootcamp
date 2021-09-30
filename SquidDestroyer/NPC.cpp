#include "NPC.h"

#include <stdexcept>

NPC::NPC(int _uid, Pos _startPos)
	: uid{ _uid },
	  startPos{ _startPos },
	  state{ State::Default },
	  pathPos{ -1 },
	  DONT_MOVE_ORDER{ EOrderType::Move, _uid, EHexCellDirection::CENTER }
{}

void NPC::setWorld(std::shared_ptr<World> _world) noexcept
{
	this->world = _world;
}

void NPC::setPath(const PosList& _path)
{
	if (pathPos != -1 && _path[pathPos] != pos()) {
		throw std::runtime_error("Path does not start at the NPC position.");
	}

	pathPos = 0;
	path = _path;
}

SOrder NPC::playTurn() noexcept
{
	switch (state) {
	case State::Default:
	case State::Moving:
		return handleMove();
	case State::Blocked:
		return handleBlocked();
	case State::Arrived:
		return DONT_MOVE_ORDER;
	}
}

void NPC::setState(const State& _state) noexcept
{
	this->state = _state;
}

SOrder NPC::handleMove() noexcept
{
	if (!world->canMove(nextPos())) {
		setState(State::Blocked);
		return DONT_MOVE_ORDER;
	}

	if (world->cellIs(nextPos(), EHexCellType::Goal)) {
		setState(State::Arrived);
	}

	SOrder order { EOrderType::Move, uid, world->getMoveDir(pos(), nextPos()) };
	++pathPos;
	return order;
}

SOrder NPC::handleBlocked() noexcept
{
	if (world->canMove(nextPos())) {
		setState(State::Moving);
		return handleMove();
	}

	return DONT_MOVE_ORDER;
}

const Pos NPC::pos() const noexcept
{
	return pathPos == -1 ? startPos : path[pathPos];
}

ConstPosRef NPC::nextPos() const noexcept
{
	return path[pathPos+(Graph::NodeID)1];
}
