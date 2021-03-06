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

	std::vector<std::shared_ptr<NPC>> npcs;

	std::for_each(_initData.npcInfoArray, _initData.npcInfoArray + _initData.nbNPCs, [this, &world_ptr, &npcs](const SNPCInfo& npcInfo) {
		auto npc = std::make_shared<NPC>(npcInfo.uid, Pos{ npcInfo.q, npcInfo.r });
		npc->setWorld(world_ptr);
		world.addObject(npc);
		npcs.push_back(npc);
	});

	auto goals = world.getMap().getAllOfType(EHexCellType::Goal);

	using namespace AStar;
	for (int i = 0; i < npcs.size(); ++i) {
		auto goal = goals[i];
		Heuristic heuristic{ goal.pos };
		auto path = searchPathAStar(world.getGraph(), npcs[i]->pos(), goal.pos, heuristic);

		// TODO: delete
		BOT_LOGIC_LOGF(mLogger, "Bot %d path is : ", npcs[i]->uid);
		for (auto pos : path) {
			BOT_LOGIC_LOGF(mLogger, "{%d, %d}, ", pos.q, pos.r);
		}
		BOT_LOGIC_LOG(mLogger, " ", true);

		npcs[i]->setPath(path);
	}
}

void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	// TODO : Update World with turn data ?

	for (const auto& o : world.getObjects(GameObject::Type::NPC)) {
		auto npc = dynamic_cast<NPC*>(o.get());

		BOT_LOGIC_LOGF(mLogger, "Bot %d moving from {%d, %d}", npc->uid, npc->pos().q, npc->pos().r);
		auto order = npc->playTurn();
		_orders.push_back(order);
		BOT_LOGIC_LOGF(mLogger, " to {%d, %d} with order %d\n", npc->pos().q, npc->pos().r, order.direction);
	}
}