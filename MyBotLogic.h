#pragma once

#include "BotLogicIF.h"
#include "Logger.h"
#include "SquidDestroyer/World.h"

#ifdef _DEBUG
#define BOT_LOGIC_DEBUG
#endif

#ifdef BOT_LOGIC_DEBUG
#define BOT_LOGIC_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#define BOT_LOGIC_LOGF(logger, format, ...) logger.Logf(format, __VA_ARGS__)
#else
#define BOT_LOGIC_LOG(logger, text, autoEndLine) 0
#define BOT_LOGIC_LOGF(logger, format, ...) 0
#endif

struct SConfigData;
struct STurnData;

//Custom BotLogic where the AIBot decision making algorithms should be implemented.
//This class must be instantiated in main.cpp.
class MyBotLogic : public virtual BotLogicIF
{
public:
	MyBotLogic();
	virtual ~MyBotLogic();

	virtual void Configure(const SConfigData& _configData);
	virtual void Init(const SInitData& _initData);
	virtual void GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders);

protected:
	Logger mLogger;

	World world;
};