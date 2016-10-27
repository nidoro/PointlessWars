#ifndef GAME_H
#define GAME_H
#include <list>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundRecorder.hpp>
#include "Entity.h"
#include "System.h"
#include "RenderSystem.h"
#include "EntitiesManager.h"
#include "ButtonSystem.h"
#include "AnchorSystem.h"
#include "OptionBoxSystem.h"
#include "AnimationSystem.h"
#include "CursorSystem.h"
#include "UnitSystem.h"
#include "TimerSystem.h"
#include "ArmySystem.h"
#include "TooltipSystem.h"
#include "TextboxSystem.h"
#include "FadeSystem.h"
#include "SoundSystem.h"
#include "SpinButtonSystem.h"
#include "PanelSystem.h"
#include "DisplayerSystem.h"
#include "WarSystem.h"
#include "ArmySystem.h"
#include "ActionSystem.h"
#include "CommandListener.h"
#include "PathSystem.h"
#include "ScriptedAnimation.h"
#include "ScenarioSystem.h"
#include "EditorSystem.h"
#include "ScrollListSystem.h"
#include "DragDropSystem.h"
#include "InputTextBoxSystem.h"
#include "NetworkSystem.h"
#include "ScreenSystem.h"
#include "ParticleSystem.h"
#include "AutoPilotSystem.h"
#include "ArmyHUDSystem.h"
#include "CameraSystem.h"
#include "DropListSystem.h"
#include "CustomMatchSystem.h"
#include "MainMenuSystem.h"
#include "AISystem.h"
#include "FormationEditor.h"
#include "InGameMenu.h"
#include "SliderSystem.h"
#include "imgui.h"
#include "imgui_SFML.h"
#include "CMDLineSystem.h"
#include "AnnouncerSystem.h"
#include "ElipsoidalMovementSystem.h"
#include "SideUISystem.h"
#include "GUIGroupSystem.h"

class Game{
    public:
        Game();
        ~Game();

        void start();
    private:
        EntitiesManager eManager;
        sf::RenderWindow window;
        void initializeSystem(System* sys, double ups = 60);

        sf::Clock updateClock;
        sf::Clock renderClock;

        std::list<System*> systems;
};

#endif // GAME_H
