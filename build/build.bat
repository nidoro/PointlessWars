
g++ -Wall -std=c++0x -Wno-comment -g ^
-D_USE_MATH_DEFINES -DIMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS -DPW_PACK_RESOURCES ^
../code/AISystem.cpp ^
../code/ActionOutcome.cpp ^
../code/ActionSystem.cpp ^
../code/AnchorSystem.cpp ^
../code/AnimationSystem.cpp ^
../code/AnnouncerSystem.cpp ^
../code/ArmyHUDSystem.cpp ^
../code/ArmySystem.cpp ^
../code/Assets.cpp ^
../code/AutoPilotSystem.cpp ^
../code/ButtonSystem.cpp ^
../code/CMDLineSystem.cpp ^
../code/CameraSystem.cpp ^
../code/CommandListener.cpp ^
../code/Component.cpp ^
../code/Config.cpp ^
../code/CursorSystem.cpp ^
../code/CustomMatchSystem.cpp ^
../code/DebugServices.cpp ^
../code/DisplayerSystem.cpp ^
../code/DragDropSystem.cpp ^
../code/DropListSystem.cpp ^
../code/EditorSystem.cpp ^
../code/ElipsoidalMovementSystem.cpp ^
../code/EntitiesManager.cpp ^
../code/Entity.cpp ^
../code/FadeSystem.cpp ^
../code/FormationEditor.cpp ^
../code/GUIGroupSystem.cpp ^
../code/Game.cpp ^
../code/InGameMenu.cpp ^
../code/InputTextBoxSystem.cpp ^
../code/MainMenuSystem.cpp ^
../code/MasterSystem.cpp ^
../code/MatchConfig.cpp ^
../code/NetworkSystem.cpp ^
../code/OSAppDataDir.cpp ^
../code/OptionBoxSystem.cpp ^
../code/PanelSystem.cpp ^
../code/ParticleSystem.cpp ^
../code/PathSystem.cpp ^
../code/RemotePlayer.cpp ^
../code/RenderSystem.cpp ^
../code/ScenarioSystem.cpp ^
../code/ScreenSystem.cpp ^
../code/ScriptedAnimation.cpp ^
../code/ScrollListSystem.cpp ^
../code/SideUISystem.cpp ^
../code/SliderSystem.cpp ^
../code/SoundSystem.cpp ^
../code/SpinButtonSystem.cpp ^
../code/Standard.cpp ^
../code/System.cpp ^
../code/TextboxSystem.cpp ^
../code/TiltSystem.cpp ^
../code/TimerSystem.cpp ^
../code/TooltipSystem.cpp ^
../code/UnitSystem.cpp ^
../code/War.cpp ^
../code/WarSystem.cpp ^
../code/external/imgui-SFML.cpp ^
../code/external/imgui.cpp ^
../code/external/imgui_draw.cpp ^
../code/external/tinyxml2.cpp ^
../code/main.cpp ^
 ^
-I"C:/Libs/SFML-2.4.0/include" ^
-I"C:/Libs/boost_1_60_0" ^
-I"C:/Users/Davi Doro/PW Local/PointlessWars/code/external" ^
 ^
-L"C:/boost-1.60-mingw-6.2.0/boost/bin.v2/libs/filesystem/build/gcc-mingw-6.2.0/debug" ^
-L"C:/boost-1.60-mingw-6.2.0/boost/bin.v2/libs/system/build/gcc-mingw-6.2.0/debug" ^
-L"C:/Libs/SFML-2.4.0/lib" ^
-lsfml-graphics-d ^
-lsfml-window-d ^
-lsfml-audio-d ^
-lsfml-network-d ^
-lsfml-system ^
-lboost_filesystem-mgw62-d-1_60 ^
-lboost_system-mgw62-d-1_60 ^
-lopengl32 ^
 ^
-o ../bin/windows-debug/pw-debug.exe
../win32/icon-win32.o ^


