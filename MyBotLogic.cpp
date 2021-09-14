#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

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

	for (int i = 0; i < _initData.tileInfoArraySize; ++i) {
		auto tileInfo = _initData.tileInfoArray[i];
		BOT_LOGIC_LOGF(mLogger, "{%d, %d}: %d\n", tileInfo.q, tileInfo.r, tileInfo.type);
	}
}

void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	BOT_LOGIC_LOG(mLogger, "GetTurnOrders", true);

	for (int i = 0; i < _turnData.npcInfoArraySize; ++i) {
		auto npcInfo = _turnData.npcInfoArray[i];
		_orders.push_back({ Move, npcInfo.uid, SE });
	}
}