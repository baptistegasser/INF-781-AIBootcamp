@echo off

set mapname=%1

rem =================================================>
rem Update Configs Here

rem example : 
rem set AIBOOTCAMP2_ENGINE_DIR=D:\AIBootcamp2\AIBootcamp2\Releases
rem set AIBOT_PATH=D:\AIBootcamp2\LesOutardes\build-solution-x64\Debug\lesoutardes.dll
rem set TEAM=LesOutardes

call SetEnv.bat

set AIBOOTCAMP2_ENGINE_DIR="%AIBOOTCAMP2_ENGINE_DIR%"
set AIBOT_PATH="%AIBOT_PATH%"
set TEAM="SquidDestroyer"

rem in s, -1 = default
set ConnectToProcessDelay=10

rem in ms, -1 = default
set InitTime=-1
set TurnTime=-1

rem <=================================================

python PlayLocalMatch.py %mapname% %AIBOOTCAMP2_ENGINE_DIR% %AIBOT_PATH% %TEAM% %ConnectToProcessDelay% %InitTime% %TurnTime%

exit /b