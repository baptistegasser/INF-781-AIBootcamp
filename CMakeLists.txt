cmake_minimum_required( VERSION 3.0 )
project( SquidDestroyer C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )

SET( SD_DIR SquidDestroyer )

SET ( SD_HEADERS
	${SD_DIR}/types.h
	${SD_DIR}/Map.h
	${SD_DIR}/Graph.h
	${SD_DIR}/AStar.h
	${SD_DIR}/NPC.h
	${SD_DIR}/World.h
)

SET ( SD_SRC
	${SD_DIR}/Map.cpp
	${SD_DIR}/Graph.cpp
	${SD_DIR}/NPC.cpp
	${SD_DIR}/World.cpp
)

SET( AIBOT_BUILDFILES 
	
	CleanAll.bat
	CMakeLists.txt
	Filters.cmake
	GenSubmit.bat
	GenSubmit.py
	PlayLocalMatch.bat
	PlayLocalMatch.py
	rebuild_vis_sln_x64.bat
)

SET( AIBOT_HEADERS 

	#Framework
	AIBot.h
	BotLogicIF.h
	ConfigData.h
	Globals.h
	InitData.h
	Logger.h
	MyBotLogic.h	
	TurnData.h
	
	#DLL Stuff
	Interop_API.h	
	stdafx.h
	targetver.h

	# SquidDestroyer headers
	${SD_HEADERS}
)

SET( AIBOT_SRC 

	#Framework
	AIBot.cpp
	Logger.cpp
	MyBotLogic.cpp

	#DLL Stuff
	dllmain.cpp
	Interop_API.cpp
	stdafx.cpp

	# SquidDestroyer sources
	${SD_SRC}
)

SET(jobfiles "${AIBOT_HEADERS};${AIBOT_SRC};${AIBOT_BUILDFILES}")
file(WRITE jobfiles.txt "${jobfiles}")

add_library(SquidDestroyer SHARED ${AIBOT_SRC} ${AIBOT_HEADERS})

CREATE_VISUAL_STUDIO_FILTERS(AIBOT)

