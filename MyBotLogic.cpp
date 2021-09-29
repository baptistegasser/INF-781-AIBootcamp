#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

#include "SquidDestroyer/Map.h"
#include "SquidDestroyer/Graph.h"
#include "SquidDestroyer/AStar.h"

#include <algorithm>

MyBotLogic::MyBotLogic()
{
	//Write Code Here
}

MyBotLogic::~MyBotLogic()
{
	//Write Code Here
}

void MyBotLogic::Configure(const SConfigData& _configData)
{
#ifdef BOT_LOGIC_DEBUG
	// Find the level's name
	std::string levelName = "Unknown_Level";
	for (int i = 0; i < _configData.cmdlineStrArraySize - 1; ++i) {
		if (strcmp(_configData.cmdlineStrArray[i], "-scene") == 0) {
			levelName = _configData.cmdlineStrArray[i + 1];
			break;
		}
	}

	// Init logger with appropriate file name for the level
	mLogger.Init(_configData.logpath, levelName + "_MyBotLogic.log");
#endif

	BOT_LOGIC_LOG(mLogger, "Configure", true);
}

void MyBotLogic::Init(const SInitData& _initData)
{
	BOT_LOGIC_LOG(mLogger, "Init", true);

	// npcs

	world.parseMap(_initData);
	world.calcGraph();

	world.getGraph().print(mLogger);

	auto world_ptr = std::shared_ptr<World>(&world);

	std::for_each(_initData.npcInfoArray, _initData.npcInfoArray + _initData.nbNPCs, [this, &world_ptr](const SNPCInfo& npcInfo) {
		NPC n{ npcInfo.uid, { npcInfo.q, npcInfo.r } };
		n.setWorld(world_ptr);
		npcs.push_back(n);
	});

	auto goals = world.getMap().getAllOfType(EHexCellType::Goal);

	for (int i = 0; i < npcs.size(); ++i) {
		auto goal = goals[i];
		Heuristic heuristic{ goal.pos };
		auto path = searchPathAStar(world.getGraph(), npcs[i].pos(), goal.pos, heuristic);
		npcs[i].setPath(path);
	}

	npcs[0].pos();
}

void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	// TODO : Update World with turn data ?

	for (NPC& npc : npcs) {
		_orders.push_back(npc.playTurn());
	}
}