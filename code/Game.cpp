#include "Game.h"

void Game::start() {
    helper::initializeAppDataDirectory();

    srand(time(nullptr));
    sf::ContextSettings ctx;
    ctx.antialiasingLevel = 0;

    System::config.loadConfigFile(helper::getAppDataDir() + "/config.xml");
    if (!System::config.getLanguage().empty()) Assets::readStrings(System::config.getLanguage());
    else                                       Assets::readStrings("en");

    Assets::load();
    System::config.setWindowIcon(Assets::getTexture("icon-v3.png"));

    window.create(sf::VideoMode(System::config.getResolution().x, System::config.getResolution().y),
                  "Pointless Wars", System::config.getFullscreen() ? sf::Style::Fullscreen : sf::Style::Default,
                  ctx);

    //window.setIcon(System::config.getWindowIcon().getSize().x, System::config.getWindowIcon().getSize().y, System::config.getWindowIcon().getPixelsPtr());

    window.setVerticalSyncEnabled(true);

#if PW_DEBUG
    DBG_SERVICES.initialize(&window);
#endif
    
#if 0
    ImGui::SFML::Init(window);
#endif

    window.setMouseCursorVisible(false);

    initializeSystem(new NetworkSystem());
    initializeSystem(new TurnTimerSystem());
    initializeSystem(new CursorSystem());
    initializeSystem(new AnimationSystem());
    initializeSystem(new WarSystem());
    initializeSystem(new ScenarioSystem());
    initializeSystem(new CustomMatchSystem());
    initializeSystem(new MainMenuSystem());
    initializeSystem(new AISystem());
    initializeSystem(new CameraSystem());
    initializeSystem(new ArmyHUDSystem());
    initializeSystem(new ScreenSystem());
    initializeSystem(new UnitSystem());
    initializeSystem(new TextboxSystem());
    initializeSystem(new FadeSystem());
    initializeSystem(new ArmySystem());
    initializeSystem(new ButtonSystem());
    initializeSystem(new AnchorSystem());
    initializeSystem(new TimerSystem());
    initializeSystem(new TooltipSystem());
    initializeSystem(new SoundSystem());
    initializeSystem(new SpinButtonSystem());
    initializeSystem(new PanelSystem());
    initializeSystem(new DisplayerSystem());
    initializeSystem(new ActionSystem());
    initializeSystem(new CommandListener());
    initializeSystem(new PathSystem());
    initializeSystem(new ScriptedAnimation());
    initializeSystem(new EditorSystem());
    initializeSystem(new ScrollListSystem());
    initializeSystem(new DragDropSystem());
    initializeSystem(new InputTextBoxSystem());
    initializeSystem(new ParticleSystem());
    initializeSystem(new AutoPilotSystem());
    initializeSystem(new DropListSystem());
    initializeSystem(new FormationEditor());
    initializeSystem(new InGameMenu());
    initializeSystem(new SliderSystem());
    initializeSystem(new AnnouncerSystem());
    initializeSystem(new ElipsoidalMovementSystem());
    initializeSystem(new SideUISystem());
    initializeSystem(new GUIGroupSystem());
    initializeSystem(new TiltSystem());

#if PW_DEBUG
    initializeSystem(new CMDLineSystem());
#endif
    
    initializeSystem(new RenderSystem(), -1);

    System::notify(GAME_STARTED);
    Entity* eInput = eManager.createEntity();
    eInput->add(new CTextInput());
    eInput->add(new CMouseInput());
    eInput->add(new CKeyboardInput());
    eInput->add(new CSystem());

    //bool networkReadyToUpdate = true;
    std::vector<bool> updatedAfterNetwork(systems.size(), false);

    bool processInput = true;
    while(window.isOpen()) {

        sf::Event ev;
        while(window.pollEvent(ev)) {
            //ImGui::SFML::ProcessEvent(ev);

            if (ev.type == sf::Event::Closed) {
                System::notify(WINDOW_CLOSED);
                window.close();
                break;
            } else if (ev.type == sf::Event::LostFocus) {
                System::notify(WINDOW_LOST_FOCUS);
                processInput = false;
            } else if (ev.type == sf::Event::GainedFocus) {
                System::notify(WINDOW_GAINED_FOCUS);
                processInput = true;
            }
            if (window.hasFocus()) processInput = true;
            else                   processInput = false;
            if (processInput) {
                if (ev.type == sf::Event::MouseButtonPressed) {
                    eInput->get<CMouseInput>()->buttonPressed = ev.mouseButton.button;
                    System::notify(MOUSE_BUTTON_PRESSED, eInput);
                } else if (ev.type == sf::Event::MouseButtonReleased) {
                    eInput->get<CMouseInput>()->buttonReleased = ev.mouseButton.button;
                    System::notify(MOUSE_BUTTON_RELEASED, eInput);
                } else if (ev.type == sf::Event::TextEntered) {
                    eInput->get<CTextInput>()->value = (char) ev.text.unicode;
                    System::notify(TEXT_ENTERED, eInput);
                } else if (ev.type == sf::Event::KeyPressed) {
                    eInput->get<CKeyboardInput>()->setInput(ev);
                    System::notify(KEY_PRESSED, eInput);
                } else if (ev.type == sf::Event::KeyReleased) {
                    eInput->get<CKeyboardInput>()->setInput(ev);
                    System::notify(KEY_RELEASED, eInput);
                }
            }

        }
        // @todo: find a better way to loop through system updates
        /*
        int nSys = 0;
        for (auto sys : systems){
            if (nSys > 0){
                bool updated = sys->checkAndUpdate();
                if (updated && !networkReadyToUpdate){
                    updatedAfterNetwork[nSys] = true;
                    networkReadyToUpdate = true;
                    for (unsigned int i = 1; i < updatedAfterNetwork.size(); i++){
                        if (!updatedAfterNetwork[i]){
                            networkReadyToUpdate = false;
                            break;
                        }
                    }
                }
            }else if (networkReadyToUpdate){
                if (sys->checkAndUpdate()){
                    for (unsigned int i = 1; i < updatedAfterNetwork.size(); i++){
                        updatedAfterNetwork[i] = false;
                        networkReadyToUpdate = false;
                    }
                }
            }
            nSys++;
        }
        */

        for (auto& sys : systems) {
            sys->checkAndUpdate();
        }

        if (eManager.updated()) {
            for (auto sys : systems) {
                sys->updateEntities();
            }
            eManager.updateList();
            eManager.clearEvents();
        }
    }
    
    System::notify(APP_ENDING);

    System::shutdownAll();
    Assets::shutdown();

    eManager.clearAll();
    eManager.updateList();
    eManager.clearEvents();

    for (auto sys : systems) {
        delete sys;
    }
    //ImGui::SFML::Shutdown();
}

void Game::initializeSystem(System* sys, double ups) {
    sys->start(&eManager, &window, ups);
    systems.push_back(sys);
}

