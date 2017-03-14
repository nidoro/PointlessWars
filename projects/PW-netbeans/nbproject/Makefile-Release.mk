#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/f1ffb32f/AISystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ActionOutcome.o \
	${OBJECTDIR}/_ext/f1ffb32f/ActionSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/AnchorSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/AnimationSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/AnnouncerSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ArmyHUDSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ArmySystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/Assets.o \
	${OBJECTDIR}/_ext/f1ffb32f/AutoPilotSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ButtonSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/CMDLineSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/CameraSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/CommandListener.o \
	${OBJECTDIR}/_ext/f1ffb32f/Component.o \
	${OBJECTDIR}/_ext/f1ffb32f/Config.o \
	${OBJECTDIR}/_ext/f1ffb32f/CursorSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/CustomMatchSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/DebugServices.o \
	${OBJECTDIR}/_ext/f1ffb32f/DisplayerSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/DragDropSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/DropListSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/EditorSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ElipsoidalMovementSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/EntitiesManager.o \
	${OBJECTDIR}/_ext/f1ffb32f/Entity.o \
	${OBJECTDIR}/_ext/f1ffb32f/EventListenerSysrem.o \
	${OBJECTDIR}/_ext/f1ffb32f/FadeSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/FormationEditor.o \
	${OBJECTDIR}/_ext/f1ffb32f/GUIGroupSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/Game.o \
	${OBJECTDIR}/_ext/f1ffb32f/InGameMenu.o \
	${OBJECTDIR}/_ext/f1ffb32f/InputTextBoxSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/MainMenuSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/MatchConfig.o \
	${OBJECTDIR}/_ext/f1ffb32f/NetworkSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/OSAppDataDir.o \
	${OBJECTDIR}/_ext/f1ffb32f/PanelSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ParticleSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/PathSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/PlayerSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/RemotePlayer.o \
	${OBJECTDIR}/_ext/f1ffb32f/RenderSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ScenarioSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ScreenSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/ScriptedAnimation.o \
	${OBJECTDIR}/_ext/f1ffb32f/ScrollListSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/SideUISystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/SliderSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/SoundSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/SpinButtonSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/Standard.o \
	${OBJECTDIR}/_ext/f1ffb32f/System.o \
	${OBJECTDIR}/_ext/f1ffb32f/TextboxSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/TiltSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/TimerSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/TooltipSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/TurnTimerSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/UnitSystem.o \
	${OBJECTDIR}/_ext/f1ffb32f/War.o \
	${OBJECTDIR}/_ext/f1ffb32f/WarSystem.o \
	${OBJECTDIR}/_ext/dc7f36ab/imgui-SFML.o \
	${OBJECTDIR}/_ext/dc7f36ab/imgui.o \
	${OBJECTDIR}/_ext/dc7f36ab/imgui_draw.o \
	${OBJECTDIR}/_ext/dc7f36ab/tinyxml2.o \
	${OBJECTDIR}/_ext/f1ffb32f/main.o \
	${OBJECTDIR}/_ext/f1ffb32f/random_int_array.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pw-netbeans.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pw-netbeans.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pw-netbeans ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/f1ffb32f/AISystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AISystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/AISystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AISystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ActionOutcome.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ActionOutcome.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ActionOutcome.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ActionOutcome.cpp

${OBJECTDIR}/_ext/f1ffb32f/ActionSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ActionSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ActionSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ActionSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/AnchorSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AnchorSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/AnchorSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AnchorSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/AnimationSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AnimationSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/AnimationSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AnimationSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/AnnouncerSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AnnouncerSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/AnnouncerSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AnnouncerSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ArmyHUDSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ArmyHUDSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ArmyHUDSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ArmyHUDSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ArmySystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ArmySystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ArmySystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ArmySystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/Assets.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Assets.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/Assets.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Assets.cpp

${OBJECTDIR}/_ext/f1ffb32f/AutoPilotSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AutoPilotSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/AutoPilotSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/AutoPilotSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ButtonSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ButtonSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ButtonSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ButtonSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/CMDLineSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CMDLineSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/CMDLineSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CMDLineSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/CameraSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CameraSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/CameraSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CameraSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/CommandListener.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CommandListener.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/CommandListener.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CommandListener.cpp

${OBJECTDIR}/_ext/f1ffb32f/Component.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Component.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/Component.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Component.cpp

${OBJECTDIR}/_ext/f1ffb32f/Config.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Config.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/Config.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Config.cpp

${OBJECTDIR}/_ext/f1ffb32f/CursorSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CursorSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/CursorSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CursorSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/CustomMatchSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CustomMatchSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/CustomMatchSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/CustomMatchSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/DebugServices.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/DebugServices.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/DebugServices.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/DebugServices.cpp

${OBJECTDIR}/_ext/f1ffb32f/DisplayerSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/DisplayerSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/DisplayerSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/DisplayerSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/DragDropSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/DragDropSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/DragDropSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/DragDropSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/DropListSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/DropListSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/DropListSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/DropListSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/EditorSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/EditorSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/EditorSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/EditorSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ElipsoidalMovementSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ElipsoidalMovementSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ElipsoidalMovementSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ElipsoidalMovementSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/EntitiesManager.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/EntitiesManager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/EntitiesManager.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/EntitiesManager.cpp

${OBJECTDIR}/_ext/f1ffb32f/Entity.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Entity.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/Entity.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Entity.cpp

${OBJECTDIR}/_ext/f1ffb32f/EventListenerSysrem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/EventListenerSysrem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/EventListenerSysrem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/EventListenerSysrem.cpp

${OBJECTDIR}/_ext/f1ffb32f/FadeSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/FadeSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/FadeSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/FadeSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/FormationEditor.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/FormationEditor.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/FormationEditor.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/FormationEditor.cpp

${OBJECTDIR}/_ext/f1ffb32f/GUIGroupSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/GUIGroupSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/GUIGroupSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/GUIGroupSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/Game.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Game.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/Game.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Game.cpp

${OBJECTDIR}/_ext/f1ffb32f/InGameMenu.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/InGameMenu.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/InGameMenu.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/InGameMenu.cpp

${OBJECTDIR}/_ext/f1ffb32f/InputTextBoxSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/InputTextBoxSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/InputTextBoxSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/InputTextBoxSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/MainMenuSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/MainMenuSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/MainMenuSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/MainMenuSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/MatchConfig.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/MatchConfig.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/MatchConfig.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/MatchConfig.cpp

${OBJECTDIR}/_ext/f1ffb32f/NetworkSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/NetworkSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/NetworkSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/NetworkSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/OSAppDataDir.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/OSAppDataDir.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/OSAppDataDir.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/OSAppDataDir.cpp

${OBJECTDIR}/_ext/f1ffb32f/PanelSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/PanelSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/PanelSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/PanelSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ParticleSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ParticleSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ParticleSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ParticleSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/PathSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/PathSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/PathSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/PathSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/PlayerSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/PlayerSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/PlayerSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/PlayerSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/RemotePlayer.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/RemotePlayer.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/RemotePlayer.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/RemotePlayer.cpp

${OBJECTDIR}/_ext/f1ffb32f/RenderSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/RenderSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/RenderSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/RenderSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ScenarioSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ScenarioSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ScenarioSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ScenarioSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ScreenSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ScreenSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ScreenSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ScreenSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/ScriptedAnimation.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ScriptedAnimation.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ScriptedAnimation.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ScriptedAnimation.cpp

${OBJECTDIR}/_ext/f1ffb32f/ScrollListSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ScrollListSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/ScrollListSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/ScrollListSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/SideUISystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/SideUISystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/SideUISystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/SideUISystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/SliderSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/SliderSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/SliderSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/SliderSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/SoundSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/SoundSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/SoundSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/SoundSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/SpinButtonSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/SpinButtonSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/SpinButtonSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/SpinButtonSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/Standard.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Standard.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/Standard.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/Standard.cpp

${OBJECTDIR}/_ext/f1ffb32f/System.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/System.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/System.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/System.cpp

${OBJECTDIR}/_ext/f1ffb32f/TextboxSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TextboxSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/TextboxSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TextboxSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/TiltSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TiltSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/TiltSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TiltSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/TimerSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TimerSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/TimerSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TimerSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/TooltipSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TooltipSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/TooltipSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TooltipSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/TurnTimerSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TurnTimerSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/TurnTimerSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/TurnTimerSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/UnitSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/UnitSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/UnitSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/UnitSystem.cpp

${OBJECTDIR}/_ext/f1ffb32f/War.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/War.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/War.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/War.cpp

${OBJECTDIR}/_ext/f1ffb32f/WarSystem.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/WarSystem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/WarSystem.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/WarSystem.cpp

${OBJECTDIR}/_ext/dc7f36ab/imgui-SFML.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/external/imgui-SFML.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/dc7f36ab
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/dc7f36ab/imgui-SFML.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/external/imgui-SFML.cpp

${OBJECTDIR}/_ext/dc7f36ab/imgui.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/external/imgui.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/dc7f36ab
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/dc7f36ab/imgui.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/external/imgui.cpp

${OBJECTDIR}/_ext/dc7f36ab/imgui_draw.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/external/imgui_draw.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/dc7f36ab
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/dc7f36ab/imgui_draw.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/external/imgui_draw.cpp

${OBJECTDIR}/_ext/dc7f36ab/tinyxml2.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/external/tinyxml2.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/dc7f36ab
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/dc7f36ab/tinyxml2.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/external/tinyxml2.cpp

${OBJECTDIR}/_ext/f1ffb32f/main.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/main.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/main.cpp

${OBJECTDIR}/_ext/f1ffb32f/random_int_array.o: /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/random_int_array.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f1ffb32f
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f1ffb32f/random_int_array.o /C/Users/Davi\ Doro/PW\ Local/PointlessWars/code/random_int_array.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
