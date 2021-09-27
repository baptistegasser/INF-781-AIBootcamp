#pragma once

enum class EHexCellType
{
	Goal,
	Forbidden,
	Default,
	JumpPowerup
};

enum class EHexCellDirection
{
	W, NW, NE, E, SE, SW, CENTER
};

enum class EObjectType
{
	Wall,
	Window,
	Door,
	PressurePlate
};

enum class EObjectState
{
	Opened,
	Closed
};

enum class EOrderType
{
	Move,
	Interact,
	Jump
};

enum class EInteractionType
{
	OpenDoor,
	CloseDoor,
	SearchHiddenDoor,
};

struct SOrder
{
	EOrderType orderType;
	int npcUID;
	EHexCellDirection direction;
	int objectUID;
	EInteractionType interactionType;
};

struct STileInfo
{
	int q;
	int r;
	EHexCellType type;
};

struct SObjectInfo
{
	int uid;
	int q;
	int r;
	EHexCellDirection cellPosition;

	int* types;
	int typesSize;

	int* states;
	int statesSize;

	int* connectedTo;
	int connectedToSize;
};

struct SNPCInfo
{
	int uid;
	int q;
	int r;
	int visionRange;
};
