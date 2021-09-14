from datetime import datetime
from subprocess import call
import ctypes
import sys
from enum import Enum

class MatchResult(Enum):
    SUCCESS = 0
    FAILURE = 1
    INVALID_MAP = 2
    UNKNOWN = 3


class MatchConfig():
    mapName = ""
    AIBOOTCAMP2_ENGINE_DIR = ""
    AIBOT_PATH = ""
    TEAM = ""
    connectToProcessDelay = ""
    initTime = ""
    turnTime = ""
    replayFile = ""
    stopOnFail = True


def RepairReplayLog(replayLog):
    isValid = False
    data = []
    dataSize = len(data)
    with open(replayLog) as f:
        for line in f:
            data.append(line)
        if "resultData" in line:
    	    isValid = True
    if not isValid:
	    data.append("], \"resultData\":{\"hasWin\":false,\"resultPayload\":\"AIBot Crashed Detected!\"}}")

    with open(replayLog, "w+") as f:
        f.truncate(0)
        for line in data:
            f.write(line)


def parseConfigFromArgv(argv):
    if len(argv) != 8:
        print("Invalid arguments count")
        print(argv)
        sys.exit(0)

    config = MatchConfig();
    config.mapName = argv[1];
    config.AIBOOTCAMP2_ENGINE_DIR = argv[2];
    config.AIBOT_PATH = argv[3];
    config.TEAM = argv[4];
    config.connectToProcessDelay = argv[5];
    config.initTime = argv[6];
    config.turnTime = argv[7];
    config.replayFile = datetime.now().strftime("%Y%m%d_%H%M%S");
    config.stopOnFail = config.mapName != "ALL_FAIL";

    return config;


def displayPrePlayMessage(config):
    print("")
    print("You are about to play a local match with the following parameters:")
    print("")
    print("   mapname                = [" + config.mapName + "]")
    print("   AIBOOTCAMP2_ENGINE_DIR = [" + config.AIBOOTCAMP2_ENGINE_DIR + "]")
    print("   AIBOT_PATH             = [" + config.AIBOT_PATH + "]")
    print("   TEAM                   = [" + config.TEAM + "]")
    print("   ConnectToProcessDelay  = [" + config.connectToProcessDelay + "]")
    print("   InitTime               = [" + config.initTime + "]")
    print("   TurnTime               = [" + config.turnTime + "]")
    print("   replayfile             = [" + config.replayFile + "]")
    print("")
    input("Press Enter to continue...")
    print("")


def playMatch(config):
    result = call([
        config.AIBOOTCAMP2_ENGINE_DIR+"/AIBootCamp2.exe",
        "-dllpath", config.AIBOT_PATH,
        "-mode", "match",
        "-scene", config.mapName,
        "-team", config.TEAM,
        "-replayfile", config.replayFile,
        "-connecttoprocessdelay", config.connectToProcessDelay,
        "-initdelay", config.initTime,
        "-turndelay", config.turnTime,
        "-quit", "-batchmode"]);

    exitCode = ctypes.c_int32(result).value;

    return {
         0: MatchResult.SUCCESS,
        -1: MatchResult.FAILURE,
         3: MatchResult.FAILURE,
        -2: MatchResult.INVALID_MAP,
    }.get(exitCode, MatchResult.UNKNOWN);


def playSingleMatch(config):
    matchResult = playMatch(config);

    if matchResult == MatchResult.SUCCESS:
        print("Match Completed : Victory!");

    elif matchResult == MatchResult.FAILURE:
        print("Match Completed : Failure");
        RepairReplayLog(".\\Replays\\" + config.replayFile + "\\" + config.mapName + "_" + config.replayFile + ".replay");

    elif matchResult == MatchResult.INVALID_MAP:
        print("Invalid Map Name [" + config.mapName + "]");


def playAllMatches(config):
    LEVELS = ["L_000","L_001","L_002","L_003","L_004","L_005","L_010","L_011","L_012","L_013"];

    for config.mapName in LEVELS:
        matchResult = playMatch(config);

        success = matchResult == MatchResult.SUCCESS;
        print("[ %s ] : %s" % (config.mapName, ('Failure', 'Victory')[success]));

        if not success:
            RepairReplayLog(".\\Replays\\" + config.replayFile + "\\" + config.mapName + "_" + config.replayFile + ".replay");

            if config.stopOnFail:
                print("");
                print("A map failed, stopping tests.");
                break;



def main(argv):
    config = parseConfigFromArgv(argv);

    displayPrePlayMessage(config);

    if config.mapName in ["ALL", "ALL_FAIL"]:
        playAllMatches(config);
    else:
        playSingleMatch(config);


if __name__ == "__main__":
    main(sys.argv);
