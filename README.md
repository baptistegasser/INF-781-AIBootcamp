# Developement and installation guide

This guide will help you install the AIBootcamp DevKit and explain how to use it.
The source of the framework was cloned from [AINomade's AIBootCamp2-MyFirstAIBot repository](https://github.com/AINomade/AIBootCamp2-MyFirstAIBot).

## Table of content
- **[Developement and installation guide](#Developement-and-installation-guide)**
	- [Table of content](#Table-of-content)
	- [Requirements](#Requirements)
	- [Installation](#Installation)
	- [Building and executing your AIBot](#Building-and-executing-your-AIBot)
	- [Managing the _MyFirstAIBot.sln_ solution](#Managing-the-MyFirstAIBotsln-solution)
    
&nbsp;
## Requirements

-   **[Visual Studio 2019](https://visualstudio.microsoft.com/vs/)** 
	- Windows PATH environment variable must contain the MSbuild folder containing its main executable (msbuild.exe).
	- Make sure to install v142 Platform Toolset

Ex.: C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe

- **[CMake](https://cmake.org/)**
	- 3.15.0 have been tested. More recent versions should also work but haven't been tested yet
	- Windows PATH environment variable must contain the CMake folder containing its main executable (cmake.exe).

- **[Python](https://www.python.org/)**
	- 3.7.4 have been tested. More recent versions should also work but haven't been tested yet
	- Windows PATH environment variable must contain the Python folder containing its main executable (python.exe).

- **[Unity](https://unity.com/)** (2020.1.4.f1)

- **AIBootcamp2 DevKit ([Sharepoint](https://usherbrooke.sharepoint.com/:f:/r/sites/msteams_6542e2/Documents%20partages/General/AIBootcamp?csf=1&web=1&e=ka8aq8))**
	- AIBootCamp2-2021.X.XX.zip
	- AIBootCamp2-MyFirstAIBot-2021.X.XX.zip

&nbsp;
## Installation
1.  **_Install required softwares and update windows PATH environment variable._**

2.  **_Check that your programs are installed properly_**
    Open a Command Prompt and execute the following commands:
    > cmake --version
	> python --version
	> msbuild --version

	If properly installed, you should see outputs specifying version numbers for every software

3.  **_Unzip AIBootcamp2 DevKit zips in separate folders_**
    It is highly recommended to avoid paths including spaces and special characters like accented letters (ex.: é)
    
4.  **_Associate .replay file to AIBootcamp2.exe_**
    After having executed your first AIBot (see next section), a **_.replay_** file will be generated. In Windows Explorer, navigate to that file and follow these steps :
    
    -Right-click on that file and select **Open with** -> **Choose default program** from the pop-up menu. -Select AIbootcamp.exe and click the **Always use the selected program to open this kind of file** check box.
    
    Important: When upgrading to newer version of AIBootcamp2.exe, it might be necessary to redo these steps to avoid opening old versions

&nbsp;
## Building and executing your AIBot
1.  **_Generating MyFirstAIBot Visual Studio solution_**
    
    In the **_AIBootCamp2-MyFirstAIBot_** folder, execute the ***rebuild_vis_sln_x64.bat*** command in a Command Prompt. This will generate the **_build-solution-x64_** folder, where you can find the **_MyFirstAIBot.sln_** visual studio solution file.
    
2.  **_Opening and managing the solution_**
    
    Open the ***MyFirstAIBot.sln*** solution in Visual Studio. You'll see the 3 following projects : **_myfirstaibot_****, ALL_BUILD and ZERO_CHECK**. Make sure your set ***myfirstaibot*** project as the startup project by right clicking the ***myfirstaibot*** solution and choosing the **_"Set as Startup Project"_** option.
    
    To create your custom decision-making system, you should implement your own BotLogic implementation. In the provided project, an empty class implementation is provided named MyBotLogic.
    
    The **_MyFirstAIBot_** framework will be explained in detail in class.
    
3.  **_Building Solution_**
    
    Choose the proper Build Target **_(Release|Debug)_** and simply build the selected project **_(F7)_**. This step should generate **_.dll file_** that will be used by the game launcher.
    
4.  **_Configure Game Launcher_**
    
    Edit **_PlayLocalMatch.bat_** found in the **_AIBootCamp2-MyFirstAIBot_** folder and modify the following lines :
	
	```batch
	set AIBOOTCAMP2_ENGINE_DIR=
	set AIBOT_PATH=
	set TEAM=
	```
    Please check examples on how to set those variables in comments in the **PlayLocalMatch.bat** file.

5.  **Executing Solution**

    To execute, launch **_PlayLocalMatch.bat_** command with the following parameters:
	> PlayLocalMatch.bat [LEVEL_ID]

    The list of `LEVEL_ID` will be provided in class.
    There is two special `LEVEL_ID` as follow:
    1. `ALL`: run all levels and stop in case of a level failure.
    1. `ALL_FAIL`: run all levels and ignore a level failure.
    
6.  **_Watching Game Replay_**
    
    Double-click on the genererated **_.replay file_** in the **_Replays_** folder and enjoy!
    
    **_Important:_** Step 4 of Installation section must be completed for this to work properly.

&nbsp;
## Managing the _MyFirstAIBot.sln_ solution

1.  **_Adding/removing files to the solution_**

    To add/remove files to the solution, you need to edit the **_CMakeLists.txt_** file in the **_AIBootCamp2-MyFirstAIBot_** folder . This files contains all the info for CMake to generate and build VisualStudio solutions.

    Look for the **_AIBOT_HEADERS_** and **_AIBOT_SRC_** sections and add/remove files in the list, and regenerate the solution using the rebuild_vis_sln_x64.bat command.

    IMPORTANT : It is highly recommanded to add your custom AIBot development files in **_custom subfolders_** to ease to process of migrating your code with the future versions of the DevKit.
    
2.  **_Cleaning the project_**

    To clean the project, execute the CleanAll.bat command in the **_AIBootCamp2-MyFirstAIBot_** folder. This will remove the build-solution-x64 folder and all temporary files. All other files will be left as is.