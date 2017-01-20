#include "ScreenSystem.h"

ScreenSystem::ScreenSystem(){
    addSubscription(GAME_STARTED);
    addSubscription(START_SCREEN_TRANSITION);
    addSubscription(CREATE_SCREEN);
    addSubscription(BT_NEW_GAME);
    addSubscription(NEW_COMMAND_LINE);
    addSubscription(KEY_PRESSED);
    addSubscription(UPDATE_RESOLUTION_WITH_DROP_LIST);
    addSubscription(UPDATE_FULLSCREEN_WITH_TOGGLE_BUTTON);
    addSubscription(CHOOSE_LANGUAGE);
    //addSubscription(OPEN_MENU);
    //addSubscription(RESUME_GAME);
}

ScreenSystem::~ScreenSystem(){

}

void ScreenSystem::update(){
    string windowName;
    windowName = "Pointless Wars";
    window->setTitle(windowName);
}

void ScreenSystem::onCreateScreen(Entity* e){
    switch(e->get<CScreen>()->id){
        case CScreen::SPLASH_1: eManager->clearSystem(); createSplash1(e); notify(NEW_SCREEN); break;
        case CScreen::SPLASH_2: eManager->clearSystem(); createSplash2(e); notify(NEW_SCREEN); break;
        case CScreen::MATCH: eManager->clearSystem(); createMatch(e); notify(NEW_SCREEN); break;
        case CScreen::LANGUAGE_MENU: eManager->clearSystem(); createLanguageMenu(e); notify(NEW_SCREEN); break;
        default: break;
    }
}

void ScreenSystem::createCustomMatch(Entity* e){
}

void ScreenSystem::createMainMenu(Entity* e){
}

void ScreenSystem::createSplash1(Entity* e){
    Entity* eObj;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->add(new CTexture("sfml-logo-small.png"));
    eObj->add(new CDraw(CDraw::GUI1));
    eObj->add(new CElipsoidalMovement(cxWindow, cyWindow, 0.f, 5.f, 180));

    eObj = eManager->createEntity();
    eObj->add(new CScreen(CScreen::SPLASH_2, CScreen::FADE_BLACK));
    eObj->add(new CTimer(1.5, START_SCREEN_TRANSITION));

    eObj = eManager->createEntity();
    eObj->add(new CMusic("main-theme.ogg"));
    notify(PLAY_MUSIC, eObj);
}

void ScreenSystem::createSplash2(Entity* e){
    Entity* eObj;

    double lineSpacing = 24;
    int fontSize = 20;

    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, cyWindow - lineSpacing/2));
    eObj->add(new CTextbox2(Assets::getString("MESSAGE-SPLASH-SCREEN-01"),
                            Assets::getFont(Assets::getPrimaryFont()), fontSize, sf::Color::White, 0, 0, CTextbox2::CENTRALIZED));
    eObj->add(new CDraw(CDraw::GUI1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, cyWindow + lineSpacing/2));
    eObj->add(new CTextbox2(Assets::getString("MESSAGE-SPLASH-SCREEN-02"),
                            Assets::getFont(Assets::getPrimaryFont()), fontSize, sf::Color::White, 0, 0, CTextbox2::CENTRALIZED));
    eObj->add(new CDraw(CDraw::GUI1));

    eObj = eManager->createEntity();
    eObj->add(new CScreen(CScreen::MAIN_MENU, CScreen::FADE_BLACK));
    eObj->add(new CTimer(5, START_SCREEN_TRANSITION));

    ///THANKS
    double walkSpeed = 90;
    double xStart = -100;
    double xTarget = 100;
    double yStart = cyWindow + 50;

    eObj = eManager->createEntity();
    eObj->add(new CPosition(xStart, yStart));
    eObj->add(new CAnimation(false, "hero-12-walk.png"));
    eObj->add(new CActor());
    eObj->add(new CDraw());
    eObj->add(new CVelocity());
    eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, "hero-12-walk.png"));
    eObj->get<CActor>()->addNode(new AMove(0.0, xTarget, yStart, walkSpeed));
    eObj->get<CActor>()->addNode(new ASpriteAnimation(getTravelTime(xStart, yStart, xTarget, yStart, walkSpeed), "hero-12-idle.png"));
    eObj->get<CActor>()->addNode(new ASpeak(0.0, Assets::getString("MESSAGE-SPLASH-SCREEN-03"), 5));
}

void ScreenSystem::createMatch(Entity* e){
    notify(INITIALIZE_WAR, e);
}

void ScreenSystem::onStartScreenTransition(Entity* e){
    ///===========
    /// Blindfold
    ///===========
    Entity* eFold = eManager->createEntity();
    eFold->add(new CScreen(*e->get<CScreen>()));
    eFold->add(new CDraw(CDraw::BLINDFOLD, 0));
    eFold->add(new CRectShape(3*wWindow, 3*hWindow, sf::Color::Black));
    eFold->add(new CPosition(cxWindow, cyWindow));
    eFold->add(new CSystem());
    eFold->add(new CActor());
    eFold->get<CActor>()->timeline.push_back(new AFade(0.0, 255, 255));
    eFold->get<CActor>()->timeline.push_back(new AFade(1.0, -255, 0));
    eFold->get<CActor>()->timeline.push_back(new ADestroy(1.0));
    eFold->add(new CTimer(1.0, CREATE_SCREEN));
    notify(LOCK_ALL_BUTTONS);
    //eManager->removeEntity(e);
}

void ScreenSystem::onBtNewGame(Entity* e){
    Entity* eScreen = eManager->createEntity();
    eScreen->add(new CScreen(CScreen::MATCH, CScreen::FADE_BLACK));
    notify(START_SCREEN_TRANSITION, eScreen);
}

void ScreenSystem::onGameStarted(Entity* e){
    //war.setMultiplayer(false);
    //onBtNewGame(e);
/*
    Entity* eScreen = eManager->createEntity();
    eScreen->add(new CScreen(CScreen::MATCH, CScreen::FADE_BLACK));
    notify(START_SCREEN_TRANSITION, eScreen);
*/
    Entity* eScreen = eManager->createEntity();
    string quickStart = config.getQuickStart();
    if (config.getLanguage().empty()){
        eScreen->add(new CScreen(CScreen::LANGUAGE_MENU, CScreen::FADE_BLACK));
    }else if (!quickStart.empty()){
        notify(SET_MATCH_CONFIG, quickStart);
        eScreen->add(new CScreen(CScreen::MATCH, CScreen::FADE_BLACK));
    }else if (!config.getSkipIntro()){
        eScreen->add(new CScreen(CScreen::SPLASH_1, CScreen::FADE_BLACK));
    }else{
        eScreen->add(new CScreen(CScreen::MAIN_MENU_NO_ANIMATION, CScreen::FADE_BLACK));
    }
    eScreen->add(new CTimer(0.0, START_SCREEN_TRANSITION));
/*
*/

}

void ScreenSystem::onOpenMenu(Entity* e){

}

void ScreenSystem::onResumeGame(Entity* e){

}

void ScreenSystem::onNewCommandLine(Entity* e){
    if ("new-game" == e->get<CCommandLine>()->command){
        war.setMultiplayer(false);
        onBtNewGame(e);
    }
}

void ScreenSystem::onKeyPressed(Entity* e){
    if (e->get<CKeyboardInput>()->alt && e->get<CKeyboardInput>()->code == sf::Keyboard::Return){
        config.setFullscreen(opposite(config.getFullscreen()));
        window->create(sf::VideoMode(config.getResolution().x, config.getResolution().y),
                      "Pointless Wars",
                      config.getFullscreen() ? sf::Style::Fullscreen : sf::Style::Default);
        window->setVerticalSyncEnabled(true);
        window->setMouseCursorVisible(false);
        config.saveConfigFile(helper::getAppDataDir() + "/config.xml");
    }
}

void ScreenSystem::onUpdateFullscreenWithToggleButton(Entity* e){
    config.setFullscreen(e->get<CStringToggleButton>()->valueIndex == 0 ? true:false);
    window->create(sf::VideoMode(config.getResolution().x, config.getResolution().y),
                   "Pointless Wars",
                    config.getFullscreen() ? sf::Style::Fullscreen : sf::Style::Default);
    window->setVerticalSyncEnabled(true);
    window->setMouseCursorVisible(false);

    config.saveConfigFile(helper::getAppDataDir() + "/config.xml");
}
void ScreenSystem::onUpdateResolutionWithDropList(Entity* e){
    stringstream ss(e->get<CDropList>()->value);
    string strWRes, strAux, strHRes;
    ss >> strWRes;
    ss >> strAux;
    ss >> strHRes;
    config.setResolution(str2int(strWRes), str2int(strHRes));
    window->create(sf::VideoMode(config.getResolution().x, config.getResolution().y),
                   "Pointless Wars",
                    config.getFullscreen() ? sf::Style::Fullscreen : sf::Style::Default);
    window->setVerticalSyncEnabled(true);
    window->setMouseCursorVisible(false);
    config.saveConfigFile(helper::getAppDataDir() + "/config.xml");
}

void ScreenSystem::createLanguageMenu(Entity* e){
    std::list<std::string> supportedLanguages = config.getSupportedLanguages();
    std::map<std::string, std::string> flags;
    for (auto& i : supportedLanguages){
        if (i == "en"){
            flags[i] = "us-flag";
        }else if (i == "pt"){
            flags[i] = "br-flag";
        }else if (i == "es"){
            flags[i] = "es-flag";
        }
    }

    double spacing = 70;
    double x = cxWindow - (flags.size()-1)*spacing/2.f;
    double y = cyWindow;
    for (auto& i : flags){
        //Entity* eObj = createFlagButton(i.second, i.first, x, y);
        createFlagButton(i.second, i.first, x, y);
        x += spacing;
    }
}

Entity* ScreenSystem::createFlagButton(std::string flag, std::string lan, double x, double y){
    Entity* e = eManager->createEntity();
    double w = Assets::getTexture(flag + "-default.png")->getSize().x;
    double h = Assets::getTexture(flag + "-default.png")->getSize().y;
    e->add(new CPosition(x, y));
    e->add(new CTexture());
    e->add(new CDimensions(w, h));
    e->add(new CButtonHitbox(w, h));
    e->add(new CButtonTextures(flag+"-default.png", flag+"-highlit.png", flag+"-highlit.png"));
    e->add(new CButtonState());
    e->add(new CButtonTrigger(CHOOSE_LANGUAGE));
    e->add(new CDraw(CDraw::GUI_01));
    e->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    e->add(new CStringMessage(lan));
    e->add(new CScreen(CScreen::SPLASH_1, CScreen::FADE_BLACK));
    e->get<CButtonTrigger>()->msgs.push_back(START_SCREEN_TRANSITION);

    return e;
}

void ScreenSystem::onChooseLanguage(Entity* e){
    config.setLanguage(e->get<CStringMessage>()->value);
    config.saveConfigFile(helper::getAppDataDir() + "/config.xml");
    Assets::readStrings(e->get<CStringMessage>()->value);
}

























