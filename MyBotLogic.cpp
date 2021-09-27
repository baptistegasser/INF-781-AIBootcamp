#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

#include "SquidDestroyer/Map.h"
#include "SquidDestroyer/Graph.h"
#include "SquidDestroyer/PathFinding.h"

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

	Map map { _initData.colCount, _initData.rowCount };

	world.setMap(std::make_unique<Map>(_initData.colCount, _initData.rowCount));
	world.calcGraph();
	world.initNPCPaths();

	std::vector<HexCell> goals;

	std::for_each(_initData.tileInfoArray, _initData.tileInfoArray + _initData.tileInfoArraySize, [&map, &goals](auto tileInfo) {
		HexCell cell{ tileInfo };
		map.set(cell);

		if (cell.type == EHexCellType::Goal) goals.push_back(cell);
	});

	auto graph = mapToGraph(map);

	std::vector<int> uids;

	std::unordered_map<int, std::vector<std::vector<HexCell>>> potentialPaths;
	for (int i = 0; i < _initData.nbNPCs; ++i) {
		SNPCInfo& npcInfo = _initData.npcInfoArray[i];
		uids.push_back(npcInfo.uid);
		// bot is on the first step
		pathsStep[npcInfo.uid] = 0;
		for (const auto& goal : goals) {
			HexCell start{ npcInfo.q, npcInfo.r, EHexCellType::Default };
			potentialPaths[npcInfo.uid].push_back(AStar(graph, start, goal, [](const HexCell& start, const HexCell& end) -> double {
				return (abs(start.q - end.q)
					+ abs(start.q + start.r - end.q - end.r)
					+ abs(start.r - end.r)) / 2;
			}));
		}
	}

	for (int i = 0; i < goals.size(); ++i) {
		int minUid = 0;
		int minCost = INT64_MAX;
		for (int& uid : uids) {
			if (potentialPaths[uid].size() < minCost) {
				minCost = potentialPaths[uid].size();
				minUid = uid;
			}
		}

		paths[minUid] = potentialPaths[minUid][i];
	}

	for (int& uid : uids) {
		int i = 1;
		for (const auto& n : paths[uid]) {
			BOT_LOGIC_LOGF(mLogger, "Step %d : go to { %d, %d }\n", i, n.q, n.r);
			++i;
		}
	}
}

void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	// TODO : Update World with turn data ?

	auto& npcs = world.getNPCs();
	for (NPC& npc : npcs) {
		_orders.push_back(npc.playTurn());
	}
}