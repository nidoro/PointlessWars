set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(PointlessWars)
#=========================
# GLOBAL COMPILER OPTIONS
#=========================
add_compile_options(-Wall -std=c++0x)
add_definitions(-D_USE_MATH_DEFINES -DIMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS)
link_directories(C:/Libs/SFML-2.4.0/lib)
link_directories(C:/boost-1.60-mingw-6.2.0/boost/bin.v2/libs/system/build/gcc-mingw-6.2.0/release)
link_directories(C:/boost-1.60-mingw-6.2.0/boost/bin.v2/libs/filesystem/build/gcc-mingw-6.2.0/release)
link_directories(C:/boost-1.60-mingw-6.2.0/boost/bin.v2/libs/system/build/gcc-mingw-6.2.0/debug)
link_directories(C:/boost-1.60-mingw-6.2.0/boost/bin.v2/libs/filesystem/build/gcc-mingw-6.2.0/debug)
include_directories(${PROJECT_SOURCE_DIR}/../code)
include_directories(${PROJECT_SOURCE_DIR}/../code/external)
include_directories(C:/Libs/SFML-2.4.0/include)
include_directories(C:/Libs/boost_1_60_0)
#=========================
# TARGET SPECIFIC OPTIONS
#=========================
add_executable (pw-release
	../code/random_int_array.cpp
	../code/TiltSystem.cpp
	../code/TurnTimerSystem.cpp
	../code/OSAppDataDir.cpp
	../code/GUIGroupSystem.cpp
    ../code/main.cpp
	../code/AISystem.cpp 
    ../code/ActionOutcome.cpp 
    ../code/ActionSystem.cpp 
    ../code/AnchorSystem.cpp 
    ../code/AnimationSystem.cpp 
    ../code/AnnouncerSystem.cpp 
    ../code/ArmyHUDSystem.cpp 
    ../code/ArmySystem.cpp 
    ../code/Assets.cpp 
    ../code/AutoPilotSystem.cpp 
    ../code/ButtonSystem.cpp 
    ../code/CMDLineSystem.cpp 
    ../code/CameraSystem.cpp 
    ../code/CommandListener.cpp 
    ../code/Component.cpp
    ../code/Config.cpp 
    ../code/CursorSystem.cpp 
    ../code/CustomMatchSystem.cpp 
    ../code/DisplayerSystem.cpp 
    ../code/DragDropSystem.cpp 
    ../code/DropListSystem.cpp 
    ../code/EditorSystem.cpp 
    ../code/ElipsoidalMovementSystem.cpp 
    ../code/EntitiesManager.cpp 
    ../code/Entity.cpp 
    ../code/FadeSystem.cpp 
    ../code/FormationEditor.cpp 
    ../code/Game.cpp 
    ../code/InGameMenu.cpp 
    ../code/InputTextBoxSystem.cpp 
    ../code/MainMenuSystem.cpp 
    ../code/MatchConfig.cpp 
    ../code/NetworkSystem.cpp 
    ../code/PanelSystem.cpp 
    ../code/ParticleSystem.cpp 
    ../code/PathSystem.cpp 
    ../code/RemotePlayer.cpp 
    ../code/RenderSystem.cpp 
    ../code/ScenarioSystem.cpp 
    ../code/ScreenSystem.cpp 
    ../code/ScriptedAnimation.cpp 
    ../code/ScrollListSystem.cpp 
    ../code/SideUISystem.cpp 
    ../code/SliderSystem.cpp 
    ../code/SoundSystem.cpp 
    ../code/SpinButtonSystem.cpp 
    ../code/Standard.cpp
    ../code/System.cpp 
    ../code/TextboxSystem.cpp 
    ../code/TimerSystem.cpp 
    ../code/TooltipSystem.cpp 
    ../code/UnitSystem.cpp 
    ../code/War.cpp 
    ../code/WarSystem.cpp 
    ../code/external/imgui.cpp 
    ../code/external/imgui-SFML.cpp 
    ../code/external/imgui_draw.cpp 
    ../code/external/tinyxml2.cpp 
    ../win32/icon-win32.o
)
add_executable (pw-debug 
	../code/random_int_array.cpp
	../code/TiltSystem.cpp
	../code/TurnTimerSystem.cpp
	../code/OSAppDataDir.cpp
	../code/GUIGroupSystem.cpp
    ../code/main.cpp
    ../code/AISystem.cpp 
    ../code/ActionOutcome.cpp 
    ../code/ActionSystem.cpp 
    ../code/AnchorSystem.cpp 
    ../code/AnimationSystem.cpp 
    ../code/AnnouncerSystem.cpp 
    ../code/ArmyHUDSystem.cpp 
    ../code/ArmySystem.cpp 
    ../code/Assets.cpp 
    ../code/AutoPilotSystem.cpp 
    ../code/ButtonSystem.cpp 
    ../code/CMDLineSystem.cpp 
    ../code/CameraSystem.cpp 
    ../code/CommandListener.cpp 
    ../code/Component.cpp
    ../code/Config.cpp 
    ../code/CursorSystem.cpp 
    ../code/CustomMatchSystem.cpp 
    ../code/DisplayerSystem.cpp 
    ../code/DragDropSystem.cpp 
    ../code/DropListSystem.cpp 
    ../code/EditorSystem.cpp 
    ../code/ElipsoidalMovementSystem.cpp 
    ../code/EntitiesManager.cpp 
    ../code/Entity.cpp 
    ../code/FadeSystem.cpp 
    ../code/FormationEditor.cpp 
    ../code/Game.cpp 
    ../code/InGameMenu.cpp 
    ../code/InputTextBoxSystem.cpp 
    ../code/MainMenuSystem.cpp 
    ../code/MatchConfig.cpp 
    ../code/NetworkSystem.cpp 
    ../code/PanelSystem.cpp 
    ../code/ParticleSystem.cpp 
    ../code/PathSystem.cpp 
    ../code/RemotePlayer.cpp 
    ../code/RenderSystem.cpp 
    ../code/ScenarioSystem.cpp 
    ../code/ScreenSystem.cpp 
    ../code/ScriptedAnimation.cpp 
    ../code/ScrollListSystem.cpp 
    ../code/SideUISystem.cpp 
    ../code/SliderSystem.cpp 
    ../code/SoundSystem.cpp 
    ../code/SpinButtonSystem.cpp 
    ../code/Standard.cpp
    ../code/System.cpp 
    ../code/TextboxSystem.cpp 
    ../code/TimerSystem.cpp 
    ../code/TooltipSystem.cpp
    ../code/UnitSystem.cpp 
    ../code/War.cpp 
    ../code/WarSystem.cpp 
    ../code/external/imgui.cpp 
    ../code/external/imgui-SFML.cpp 
    ../code/external/imgui_draw.cpp 
    ../code/external/tinyxml2.cpp 
    ../win32/icon-win32.o
)
target_link_libraries(pw-release
	sfml-graphics
	sfml-window
	sfml-audio
	sfml-network
	sfml-system
	boost_filesystem-mgw62-1_60
	boost_system-mgw62-1_60
    opengl32
    stdc++fs
)
target_link_libraries(pw-debug
	sfml-graphics-d
	sfml-window-d
	sfml-audio-d
	sfml-network-d
	sfml-system-d
	boost_filesystem-mgw62-d-1_60
	boost_system-mgw62-d-1_60
    opengl32
    stdc++fs
)
target_compile_options(pw-release PRIVATE -O2)
target_compile_options(pw-debug PRIVATE -g)
set_target_properties(pw-release
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin/win32
    OUTPUT_NAME "Pointless Wars"
)
set_target_properties(pw-debug
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin/win32-debug
    OUTPUT_NAME "pw-debug"
)
#=========================
# CPACK
#=========================
#install(DIRECTORY ../rsc/ DESTINATION rsc COMPONENT resources)
get_target_property(ExecutableDir pw-release RUNTIME_OUTPUT_DIRECTORY)
get_target_property(ExecutableName pw-release OUTPUT_NAME)

install(DIRECTORY ../rsc/lan DESTINATION rsc COMPONENT Languages)
install(DIRECTORY ../rsc/sceneries DESTINATION rsc COMPONENT Sceneries)
install(FILES ../rsc/animations.dat DESTINATION rsc COMPONENT Animations)
install(FILES ../rsc/objects.dat DESTINATION rsc COMPONENT Objects)
install(FILES ../rsc/resources DESTINATION rsc COMPONENT Resources)

install(DIRECTORY ../bin/win32 DESTINATION bin COMPONENT Game)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries) 
#=========================
# CPACK CONFIG
#=========================
set(CPACK_PACKAGE_NAME "PW-setup")
set(CPACK_PACKAGE_VENDOR "Davi Doro")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Pointless Wars setup")
#set(CPACK_PACKAGE_VERSION_MAJOR "1")
#set(CPACK_PACKAGE_VERSION_MINOR "0")
#set(CPACK_PACKAGE_VERSION_PATCH "0")
#set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "alpha")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Pointless Wars")

#
# Tell CPack about the components and group the data components together (CPACK_COMPONENT_${COMPONENT_NAME_ALL_CAPS}_GROUP).
#
set(CPACK_COMPONENTS_ALL Game Languages Sceneries Animations Objects Resources)
set(CPACK_COMPONENT_GAME_GROUP "Pointless Wars")
set(CPACK_COMPONENT_LANGUAGES_GROUP "Pointless Wars")
set(CPACK_COMPONENT_SCENERIES_GROUP "Pointless Wars")
set(CPACK_COMPONENT_ANIMATIONS_GROUP "Pointless Wars")
set(CPACK_COMPONENT_OBJECTS_GROUP "Pointless Wars")
set(CPACK_COMPONENT_RESOURCES_GROUP "Pointless Wars")

#
# More descriptive names for each of the components, and component groups
#
#set(CPACK_COMPONENT_BIN_DISPLAY_NAME "Application")
#set(CPACK_COMPONENT_RESOURCES_DISPLAY_NAME "Resources")
#set(CPACK_COMPONENT_GROUP_POINTLESSWARS_DESCRIPTION "Game")

#
# Text from "EULA.txt" is displayed in the installer's license tab
#
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/../EULA.txt")

#
# Set the icon used inside the installer wizard and the icons for the installer and un-installer
#
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/../rsc/images/icons\\\\banner-150-57.bmp")
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/../win32/icon-v3.ico")
#set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/hw.ico")


#
# Set the programs displayed under the Start menu shortcut
# We do not use CPACK_PACKAGE_EXECUTABLES because it expects the binaries to be in a
# directory named "bin" (CMake bug tracker ID: 0007829).
#
#set(CPACK_NSIS_MENU_LINKS "programs\\hello" "Hello"
#                          "programs\\world" "World"
#                          ${CT_DATA_FILE_DIR} "CT Data"
#                          "data\\example.pdf" "documentation")

# Create the desktop link at installation
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "\
  SetOutPath '$INSTDIR\\\\bin\\\\win32'\n\
  CreateShortCut '$INSTDIR\\\\Pointless Wars.lnk' '$INSTDIR\\\\bin\\\\win32\\\\Pointless Wars.exe'\n\
  SetOutPath '$INSTDIR\\\\bin\\\\win32'\n\
  CreateShortCut '$DESKTOP\\\\Pointless Wars.lnk' '$INSTDIR\\\\bin\\\\win32\\\\Pointless Wars.exe'\
")

# Delete the desktop link at uninstallation
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "\
  Delete '$INSTDIR\\\\Pointless Wars.lnk'\n\
  Delete '$DESKTOP\\\\Pointless Wars.lnk'\
")

include(CPack)
















