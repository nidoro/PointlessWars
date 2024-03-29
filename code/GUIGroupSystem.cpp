#include "GUIGroupSystem.h"

GUIGroupSystem::GUIGroupSystem() {
    addSubscription(UPDATE_PLAYER_NICKNAME_WITH_INPUT_TEXTBOX);
    addSubscription(CREATE_GUI_GROUP);
    addSubscription(REMOVE_GUI_GROUP);
    addSubscription(CHANGE_WINDOW_PAGE);
    addSubscription(MOUSE_BUTTON_PRESSED);
    addSubscription(DISCONNECT_FROM_SERVER);
    addSubscription(CONNECTION_LOST);

    addRequirement(Component::GUI_GROUP);
    addRequirement(Component::DRAW);
    addRequirement(Component::UI_LAYER);
}

void GUIGroupSystem::update() {

}

void GUIGroupSystem::onCreateGUIGroup(Entity* e) {
    Entity* eGUI = e->getObservedEntity("create-gui-group");

    if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "single-player") {
        createWindowSinglePlayer(eGUI);
        notify(BRING_UI_LAYER_FORWARD, eGUI);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "game-options") {
        createWindowGameOptions(eGUI);
        notify(BRING_UI_LAYER_FORWARD, eGUI);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "multiplayer") {
        Entity* eWin = createWindowMultiplayer(eGUI);
        showPage(eWin->getObservedEntity("page-connect"));
        notify(BRING_UI_LAYER_FORWARD, eWin);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "credits") {
        Entity* eWin = createCredits(eGUI);
        showPage(eWin->getObservedEntity("page-1"));
        notify(BRING_UI_LAYER_FORWARD, eWin);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "help") {
        Entity* eWin = createHelp(eGUI, true);
        showPage(eWin->getObservedEntity("lesson-1"));
        notify(BRING_UI_LAYER_FORWARD, eWin);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "help-main-menu") {
        Entity* eWin = createHelp(eGUI, false);
        showPage(eWin->getObservedEntity("lesson-1"));
        notify(BRING_UI_LAYER_FORWARD, eWin);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "in-game-menu") {
        Entity* eWin = createWindowInGameMenu(eGUI);
        showPage(eWin->getObservedEntity("page-first"));
        notify(BRING_UI_LAYER_FORWARD, eWin);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "leave-match-confirmation") {
        createLeaveMatchConfirmationWindow(eGUI);
        notify(BRING_UI_LAYER_FORWARD, eGUI);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "opponent-left") {
        createEndMultiplayerMatchWindow(eGUI, Assets::getString("MESSAGE-OPPONENT-LEFT"));
        notify(BRING_UI_LAYER_FORWARD, eGUI);
    } else if (eGUI->get<CGUIGroup>()->groupType == "window-page") {

    }
}

void GUIGroupSystem::createWindowGameOptions(Entity* e) {
    /// e has CGUIGroup, CDraw and CUILayer
    /// make a copy of e
    Entity* eGUI = eManager->createEntity();
    eGUI->add(new CGUIGroup(*e->get<CGUIGroup>()));
    eGUI->add(new CDraw(*e->get<CDraw>()));
    eGUI->add(new CUILayer(*e->get<CUILayer>()));

    eGUI->addObservedEntity("page-game-options", eManager->createEntity());

    eGUI->attachEmployee(eGUI->getObservedEntity("page-game-options"));

    eGUI->getObservedEntity("page-game-options")->addObservedEntity("window", eGUI);

    //Entity* eObj;

    ///================
    /// First page:
    ///================
    //double wButton = 180;
    //double hButton = 40;
    //double spcButton = 50;
    double wPanel = 290;
    double hPanel = 350;
    double xPanel = cxWindow;
    double yPanel = cyWindow + 100;

    //double x0 = xPanel;
    //double y0 = yPanel - hPanel/2 + 30;
    //double x = x0;
    //double y = y0;

    sf::Color darkBlue(15, 30, 60);

    /// Panel
    eGUI->add(new CTexture("9p-logo-frame.png"));
    eGUI->add(new CDimensions(wPanel, hPanel));
    eGUI->add(new CButtonHitbox(wPanel, hPanel));
    eGUI->add(new CButtonState());
    eGUI->add(new CButtonTrigger(EMPTY_MESSAGE));
    eGUI->add(new CPosition(xPanel, yPanel));
    eGUI->add(new CDraw(CDraw::GUI_00));
    eGUI->add(new CUILayer(CUILayer::L1));

    initializeGameOptionsPage(eGUI, true);
}
void GUIGroupSystem::createWindowSinglePlayer(Entity* e) {
    /// e has CGUIGroup, CDraw and CUILayer
    /// make a copy of e
    Entity* eGUI = eManager->createEntity();
    eGUI->add(new CGUIGroup(*e->get<CGUIGroup>()));
    eGUI->add(new CDraw(*e->get<CDraw>()));
    eGUI->add(new CUILayer(*e->get<CUILayer>()));

    eGUI->addObservedEntity("page-first", eManager->createEntity());
    eGUI->addObservedEntity("page-quick-match", eManager->createEntity());

    eGUI->attachEmployee(eGUI->getObservedEntity("page-first"));
    eGUI->attachEmployee(eGUI->getObservedEntity("page-quick-match"));

    eGUI->getObservedEntity("page-first")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("page-quick-match")->addObservedEntity("window", eGUI);

    Entity* eObj;

    ///================
    /// First page:
    ///================
    //double wButton = 180;
    //double hButton = 40;
    double spcButton = 50;
    double wPanel = 290;
    double hPanel = 350;
    double xPanel = cxWindow;
    double yPanel = cyWindow + 100;

    double x0 = xPanel;
    double y0 = yPanel - hPanel/2 + 30;
    double x = x0;
    double y = y0;

    sf::Color darkBlue(15, 30, 60);

    /// Panel
    eGUI->add(new CTexture("9p-logo-frame.png"));
    eGUI->add(new CDimensions(wPanel, hPanel));
    eGUI->add(new CButtonHitbox(wPanel, hPanel));
    eGUI->add(new CButtonState());
    eGUI->add(new CButtonTrigger(EMPTY_MESSAGE));
    eGUI->add(new CPosition(xPanel, yPanel));
    eGUI->add(new CDraw(CDraw::GUI_00));
    eGUI->add(new CUILayer(CUILayer::L1));
    /// Title
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-SINGLE-PLAYER"), Assets::getFont(Assets::getPrimaryFont()), 20, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CPosition(x, y));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    double wTitle = eObj->get<CTextbox2>()->content.getLocalBounds().width;

    /// Underline
    y += 24;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CRectShape(wTitle, 1));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Gem
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CTexture("small-orange-gem.png"));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 2)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Tutorial
    y += spcButton/1.5;
    eObj = createRectButton(Assets::getString("LABEL-TUTORIAL"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer, true);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Quick Match
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-QUICK-MATCH"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    eObj->addObservedEntity("remove-gui-group", eGUI);
    eObj->add(new CStringMessage("Prototype"));
    eObj->add(new CScreen(CScreen::MATCH, CScreen::FADE_BLACK));
    eObj->get<CButtonTrigger>()->msgs = {SET_MATCH_CONFIG, REMOVE_GUI_GROUP, START_SCREEN_TRANSITION};

    /// Campaign
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-CAMPAIGN"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer, true);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Arena
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-ARENA"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer, true);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Custom
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-CUSTOM-GAME"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer, true);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Button Back (hidden)
    y += spcButton;
    eObj = createRectButton("", 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, REMOVE_GUI_GROUP, eGUI->get<CUILayer>()->layer);
    eObj->add(new CButtonHitbox(0,0));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    eObj->addObservedEntity("remove-gui-group", eGUI);
}
Entity* GUIGroupSystem::createWindowInGameMenu(Entity* e) {
    /// e has CGUIGroup, CDraw and CUILayer
    /// make a copy of e
    Entity* eGUI = eManager->createEntity();
    eGUI->add(new CGUIGroup(*e->get<CGUIGroup>()));
    eGUI->add(new CDraw(*e->get<CDraw>()));
    eGUI->add(new CUILayer(*e->get<CUILayer>()));

    eGUI->addObservedEntity("page-first", eManager->createEntity());
    eGUI->addObservedEntity("page-game-options", eManager->createEntity());

    eGUI->attachEmployee(eGUI->getObservedEntity("page-first"));
    eGUI->attachEmployee(eGUI->getObservedEntity("page-game-options"));

    eGUI->getObservedEntity("page-first")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("page-game-options")->addObservedEntity("window", eGUI);

    Entity* eObj;

    //double wButton = 180;
    //double hButton = 40;
    double spcButton = 50;
    double wPanel = 290;
    double hPanel = 350;
    double xPanel = cxWindow;
    double yPanel = cyWindow;

    double x0 = xPanel;
    double y0 = yPanel - hPanel/2 + 30;
    double x = x0;
    double y = y0;

    sf::Color darkBlue(15, 30, 60);

    /// Panel
    eGUI->add(new CTexture("9p-logo-frame.png"));
    eGUI->add(new CDimensions(wPanel, hPanel));
    eGUI->add(new CButtonHitbox(wPanel, hPanel));
    eGUI->add(new CButtonState());
    eGUI->add(new CButtonTrigger(EMPTY_MESSAGE));
    eGUI->add(new CPosition(xPanel, yPanel));
    eGUI->add(new CDraw(CDraw::GUI_00));
    eGUI->add(new CUILayer(CUILayer::L1));

    ///================
    /// First page:
    ///================
    /// Title
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-MENU"), Assets::getFont(Assets::getPrimaryFont()), 20, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CPosition(x, y));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    double wTitle = eObj->get<CTextbox2>()->content.getLocalBounds().width;

    /// Underline
    y += 24;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CRectShape(wTitle, 1));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Gem
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CTexture("small-orange-gem.png"));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 2)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Resume
    y += spcButton/1.5;
    eObj = createRectButton(Assets::getString("LABEL-RESUME"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, REMOVE_GUI_GROUP, eGUI->get<CUILayer>()->layer);
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    eObj->addObservedEntity("remove-gui-group", eGUI);

    /// Game Options
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-OPTIONS"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, CHANGE_WINDOW_PAGE, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("page-game-options"));

    /// Quit
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-QUIT"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, CREATE_GUI_GROUP, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    Entity* eAux = eManager->createEntity();
    eAux->add(new CGUIGroup("window", "leave-match-confirmation"));
    eAux->add(new CUILayer(CUILayer::L3));
    eAux->add(new CDraw(CDraw::GUI_04));
    eAux->attachEmployer(eObj);
    //eObj->attachEmployer(eAux);
    eObj->addObservedEntity("create-gui-group", eAux);
    ///================
    /// Options page:
    ///================
    initializeGameOptionsPage(eGUI, false);

    // @note:
    // Help
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-HELP"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer);
    eObj->get<CButtonTrigger>()->msgs = {REMOVE_GUI_GROUP, CREATE_GUI_GROUP};
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    eAux = eManager->createEntity();
    eAux->add(new CGUIGroup("window", "help"));
    eAux->add(new CUILayer(CUILayer::L3));
    eAux->add(new CDraw(CDraw::GUI_04));
    eAux->attachEmployer(eObj);
    //eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    eObj->addObservedEntity("create-gui-group", eAux);
    eObj->addObservedEntity("remove-gui-group", eGUI);

    return eGUI;

}


void GUIGroupSystem::initializeGameOptionsPage(Entity* eGUI, bool closeOnEsc) {
    /// Assumes that e has a CGUIGroup, a page of name game-options and a CUILayer

    //double wButton = 180;
    //double hButton = 40;
    double spcButton = 50;
    //double wPanel = 290;
    double hPanel = 350;
    double xPanel = cxWindow;
    double yPanel = closeOnEsc ? cyWindow + 100 : cyWindow;

    double x0 = xPanel;
    double y0 = yPanel - hPanel/2 + 30;
    double x = x0;
    double y = y0;

    sf::Color darkBlue(15, 30, 60);

    Entity* eObj;
    /// Title
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-OPTIONS"), Assets::getFont(Assets::getPrimaryFont()), 20, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CPosition(x, y));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));
    double wTitle = eObj->get<CTextbox2>()->content.getLocalBounds().width;

    /// Underline
    y += 24;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CRectShape(wTitle, 1));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// Gem
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CTexture("small-orange-gem.png"));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 2)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    double sliderSize = 125;
    double hText;

    /// SFX LABEL
    y += spcButton;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-SFX"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// SFX SLIDER
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x - sliderSize/2 + (config.getSfxMaxVolume()/100)*sliderSize + sliderSize/2.f - 5, y));
    eObj->add(new CTexture("slider-01.png"));
    eObj->add(new CSlider(x + sliderSize/2.f - 5, y, sliderSize, (config.getMusMaxVolume()/100), CSlider::HORIZONTAL, "sfx-volume"));
    eObj->add(new CDraw(CDraw::GUI_02));
    eObj->add(new CDimensions(19, 30));
    eObj->add(new CButtonHitbox(19, 30));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(START_SLIDING, CButtonTrigger::ON_PRESS));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->add(new CButtonSounds("NONE", "NONE", "sfx-poof-03.ogg"));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// SFX SLIDER BAR
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + sliderSize/2.f - 5, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CDimensions(sliderSize, 2));
    eObj->add(new CRectShape(sliderSize, 2, sf::Color::White));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// MUSIC LABEL
    y += spcButton;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-MUSIC"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// MUSIC SLIDER
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x - sliderSize/2 + (config.getMusMaxVolume()/100)*sliderSize + sliderSize/2.f - 5, y));
    eObj->add(new CTexture("slider-01.png"));
    eObj->add(new CSlider(x + sliderSize/2.f - 5, y, sliderSize, (config.getMusMaxVolume()/100), CSlider::HORIZONTAL, "music-volume"));
    eObj->add(new CDraw(CDraw::GUI_02));
    eObj->add(new CDimensions(19, 30));
    eObj->add(new CButtonHitbox(19, 30));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(START_SLIDING, CButtonTrigger::ON_PRESS));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// MUSIC SLIDER BAR
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + sliderSize/2.f - 5, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CDimensions(sliderSize, 2));
    eObj->add(new CRectShape(sliderSize, 2, sf::Color::White));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// RESOLUTION LABEL
    y += spcButton;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-RESOLUTION"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// RESOLUTION DROP LIST
    list<string> resStrings;
    unsigned int bbp = sf::VideoMode::getFullscreenModes()[0].bitsPerPixel;
    for (unsigned int i = 0; i < sf::VideoMode::getFullscreenModes().size(); i++) {
        if (sf::VideoMode::getFullscreenModes()[i].bitsPerPixel != bbp) continue;
        resStrings.push_back(int2str(sf::VideoMode::getFullscreenModes()[i].width) + " x " + int2str(sf::VideoMode::getFullscreenModes()[i].height));
    }
    string initValue = int2str(config.getResolution().x) + " x " + int2str(config.getResolution().y);
    eObj = eManager->createEntity();
    eObj->add(new CDropList(resStrings, initValue, CDropList::DOWN, UPDATE_RESOLUTION_WITH_DROP_LIST));
    eObj->add(new CPosition(x + sliderSize/2.f - 5, y));
    eObj->add(new CDraw(CDraw::GUI_02));
    eObj->add(new CButtonHitbox(120, 20));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(EXPAND_DROP_LIST));
    eObj->add(new CTextbox2(initValue, Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::CENTRALIZED));
    eObj->add(new CDimensions(120, 20));
    eObj->add(new CRectShape(120, 20, darkBlue, 1, sf::Color::White));
    eObj->add(new CRectButton(sf::RectangleShape(), eObj->get<CRectShape>()->shape, eObj->get<CRectShape>()->shape));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// style
    sf::RectangleShape defShape;
    defShape.setSize(sf::Vector2f(120, 20));
    defShape.setFillColor(sf::Color::Black);
    defShape.setOutlineColor(sf::Color::White);
    defShape.setOutlineThickness(1);
    sf::RectangleShape hovShape = defShape;
    hovShape.setOutlineColor(sf::Color::Red);
    eObj->get<CDropList>()->setStyle(defShape, hovShape, hovShape);

    /// FULLSCREEN
    y += spcButton;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-FULLSCREEN"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// FULLSCREEN TOGGLE BUTTON
    /// style
    defShape.setSize(sf::Vector2f(120, 20));
    defShape.setFillColor(sf::Color::Black);
    defShape.setOutlineColor(sf::Color::White);
    defShape.setOutlineThickness(1);
    hovShape.setOutlineColor(sf::Color::Red);
    list<string> optionList;
    optionList.push_back(Assets::getString("LABEL-ON"));
    optionList.push_back(Assets::getString("LABEL-OFF"));
    initValue = config.getFullscreen() ? Assets::getString("LABEL-ON") : Assets::getString("LABEL-OFF");
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + sliderSize/2.f - 5, y));
    eObj->add(new CDraw(CDraw::GUI_02));
    eObj->add(new CButtonHitbox(120, 20));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(DO_TOGGLE_ACTION));
    eObj->add(new CTextbox2(initValue, Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::CENTRALIZED));
    eObj->add(new CDimensions(120, 20));
    eObj->add(new CRectShape(120, 20, darkBlue, 1, sf::Color::White));
    eObj->add(new CRectButton(sf::RectangleShape(), eObj->get<CRectShape>()->shape, hovShape));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->add(new CStringToggleButton(Assets::getString("LABEL-ON"), Assets::getString("LABEL-OFF"), config.getFullscreen() ? 0:1, UPDATE_FULLSCREEN_WITH_TOGGLE_BUTTON));
    eObj->add(new CDisplayer(CDisplayer::STR_TOGGLE_BUTTON, eObj));
    eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));

    /// Button Back (hidden)
    if (closeOnEsc) {
        y += spcButton;
        eObj = createRectButton("", 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, REMOVE_GUI_GROUP, eGUI->get<CUILayer>()->layer);
        eObj->add(new CButtonHitbox(0,0));
        eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
        eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));
        eObj->addObservedEntity("remove-gui-group", eGUI);
    } else {
        y += spcButton;
        eObj = createRectButton("", 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, CHANGE_WINDOW_PAGE, eGUI->get<CUILayer>()->layer);
        eObj->add(new CButtonHitbox(0,0));
        eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
        eObj->attachEmployer(eGUI->getObservedEntity("page-game-options"));
        eObj->addObservedEntity("change-page", eGUI->getObservedEntity("page-first"));
    }
}

Entity* GUIGroupSystem::createLeaveMatchConfirmationWindow(Entity* e) {
    string panelTexture = "9p-grey-frame-rounded.png";
    double xPanel = cxWindow;
    double yPanel = cyWindow;
    double wPanel = 450;
    double hPanel = 100;
    CUILayer::Layer uiLayer = e->get<CUILayer>()->layer;

    Entity* eInfoWin;
    eInfoWin = eManager->createEntity();
    eInfoWin->add(new CPosition(xPanel, yPanel));
    eInfoWin->add(new CTexture(panelTexture));
    eInfoWin->add(new CDimensions(wPanel, hPanel));
    eInfoWin->add(new CDraw(e->get<CDraw>()->tag));
    eInfoWin->add(new CUILayer(e->get<CUILayer>()->layer));
    eInfoWin->add(new CGUIGroup(*e->get<CGUIGroup>()));

    Entity* eObj;
    double x, y;
    x = xPanel;
    y = yPanel;
    double spacing = 30;
    double hSpacing = 100;

    /// Dark background
    eObj = eManager->createEntity();
    eObj->add(new CRectShape(wWindow, hWindow, sf::Color(0,0,0,100)));
    eObj->add(new CDraw((CDraw::Tag)((int)eInfoWin->get<CDraw>()->tag - 1)));
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->get<CDraw>()->alpha = 150;
    eObj->attachEmployer(eInfoWin);

    /// Message
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y - spacing/2));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)eInfoWin->get<CDraw>()->tag + 1)));
    eObj->add(new CTypingEffect(Assets::getString("MESSAGE-LEAVE-MATCH-CONFIRMATION"), 40));
    eObj->attachEmployer(eInfoWin);
    eInfoWin->addObservedEntity("message", eObj);

    /// Button YES
    eObj = createRectButton(Assets::getString("LABEL-YES"), 14, 30, x - hSpacing/2.f, y + spacing/2,
                            sf::Color::White, sf::Color::Black, sf::Color::White, 2, EMPTY_MESSAGE, uiLayer);
    eObj->get<CDraw>()->tag = (CDraw::Tag)((int)eInfoWin->get<CDraw>()->tag + 1);
    eObj->attachEmployer(eInfoWin);
    eObj->add(new CScreen(CScreen::MAIN_MENU_NO_ANIMATION, CScreen::FADE_BLACK));
    eObj->addObservedEntity("remove-gui-group", eInfoWin);
    eObj->get<CButtonTrigger>()->msgs = {REMOVE_GUI_GROUP, END_MATCH, START_SCREEN_TRANSITION};

    /// Button NO
    eObj = createRectButton(Assets::getString("LABEL-NO"), 14, 30, x + hSpacing/2.f, y + spacing/2,
                            sf::Color::White, sf::Color::Black, sf::Color::White, 2, EMPTY_MESSAGE, uiLayer);
    eObj->get<CDraw>()->tag = (CDraw::Tag)((int)eInfoWin->get<CDraw>()->tag + 1);
    eObj->attachEmployer(eInfoWin);
    eObj->addObservedEntity("remove-gui-group", eInfoWin);
    eObj->get<CButtonTrigger>()->msgs = {REMOVE_GUI_GROUP};

    return eInfoWin;
}

Entity* GUIGroupSystem::createEndMultiplayerMatchWindow(Entity* e, std::string message) {
    string panelTexture = "9p-grey-frame-rounded.png";
    double xPanel = cxWindow;
    double yPanel = cyWindow;
    double wPanel = 450;
    double hPanel = 100;
    CUILayer::Layer uiLayer = e->get<CUILayer>()->layer;

    Entity* eInfoWin;
    eInfoWin = eManager->createEntity();
    eInfoWin->add(new CPosition(xPanel, yPanel));
    eInfoWin->add(new CTexture(panelTexture));
    eInfoWin->add(new CDimensions(wPanel, hPanel));
    eInfoWin->add(new CDraw(e->get<CDraw>()->tag));
    eInfoWin->add(new CUILayer(e->get<CUILayer>()->layer));
    eInfoWin->add(new CGUIGroup(*e->get<CGUIGroup>()));

    Entity* eObj;
    double x, y;
    x = xPanel;
    y = yPanel;
    double spacing = 30;
    double hSpacing = 100;

    /// Dark background
    eObj = eManager->createEntity();
    eObj->add(new CRectShape(wWindow, hWindow, sf::Color(0,0,0,100)));
    eObj->add(new CDraw((CDraw::Tag)((int)eInfoWin->get<CDraw>()->tag - 1)));
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->get<CDraw>()->alpha = 150;
    eObj->attachEmployer(eInfoWin);

    /// Message
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y - spacing/2));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)eInfoWin->get<CDraw>()->tag + 1)));
    eObj->add(new CTypingEffect(message, 40));
    eObj->attachEmployer(eInfoWin);
    eInfoWin->addObservedEntity("message", eObj);

    /// Button OK
    eObj = createRectButton(Assets::getString("LABEL-OK"), 14, 30, x, y + spacing/2,
                            sf::Color::White, sf::Color::Black, sf::Color::White, 2, EMPTY_MESSAGE, uiLayer);
    eObj->get<CDraw>()->tag = (CDraw::Tag)((int)eInfoWin->get<CDraw>()->tag + 1);
    eObj->attachEmployer(eInfoWin);
    eObj->add(new CScreen(CScreen::MAIN_MENU_NO_ANIMATION, CScreen::FADE_BLACK));
    eObj->addObservedEntity("remove-gui-group", eInfoWin);
    eObj->get<CButtonTrigger>()->msgs = {REMOVE_GUI_GROUP, END_MATCH, START_SCREEN_TRANSITION};

    return eInfoWin;
}

Entity* GUIGroupSystem::createWindowMultiplayer(Entity* e) {
    /// e has CGUIGroup, CDraw and CUILayer
    /// make a copy of e
    Entity* eGUI = eManager->createEntity();
    eGUI->add(new CGUIGroup(*e->get<CGUIGroup>()));
    eGUI->add(new CDraw(*e->get<CDraw>()));
    eGUI->add(new CUILayer(*e->get<CUILayer>()));

    eGUI->addObservedEntity("page-connect", eManager->createEntity());
    eGUI->addObservedEntity("page-quick-match", eManager->createEntity());

    eGUI->attachEmployee(eGUI->getObservedEntity("page-connect"));
    eGUI->attachEmployee(eGUI->getObservedEntity("page-quick-match"));

    eGUI->getObservedEntity("page-connect")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("page-quick-match")->addObservedEntity("window", eGUI);

    Entity* eObj;

    ///================
    /// First page:
    ///================
    //double wButton = 180;
    //double hButton = 40;
    double spcButton = 50;
    double wPanel = 290;
    double hPanel = 350;
    double xPanel = cxWindow;
    double yPanel = cyWindow + 100;
    double hText;

    double x0 = xPanel;
    double y0 = yPanel - hPanel/2 + 30;
    double x = x0;
    double y = y0;
    double wTitle;

    sf::Color darkBlue(15, 30, 60);

    /// Panel
    eGUI->add(new CTexture("9p-logo-frame.png"));
    eGUI->add(new CDimensions(wPanel, hPanel));
    eGUI->add(new CButtonHitbox(wPanel, hPanel));
    eGUI->add(new CButtonState());
    eGUI->add(new CButtonTrigger(EMPTY_MESSAGE));
    eGUI->add(new CPosition(xPanel, yPanel));
    eGUI->add(new CDraw(CDraw::GUI_00));
    eGUI->add(new CUILayer(CUILayer::L1));

    /// Title
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-MULTIPLAYER"), Assets::getFont(Assets::getPrimaryFont()), 20, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CPosition(x, y));
    eObj->attachEmployer(eGUI->getObservedEntity("page-connect"));
    wTitle = eObj->get<CTextbox2>()->content.getLocalBounds().width;

    /// Underline
    y += 24;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CRectShape(wTitle, 1));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-connect"));

    /// Gem
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CTexture("small-orange-gem.png"));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 2)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-connect"));

    /// Nickname Label
    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-NICKNAME"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-connect"));

    /// Input Textbox Nickname
    y += spcButton;
    double wInputTB = 200;
    double hInputTB = 35;
    sf::RectangleShape rectInputTB;
    rectInputTB.setOutlineColor(sf::Color::White);
    rectInputTB.setOutlineThickness(2);
    rectInputTB.setPosition(x, y);
    rectInputTB.setSize(sf::Vector2f(wInputTB, hInputTB));
    rectInputTB.setFillColor(darkBlue);
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->add(new CInputTextBox(true, UPDATE_PLAYER_NICKNAME_WITH_INPUT_TEXTBOX, CInputTextBox::INACTIVE, 12, false));
    eObj->add(new CButtonHitbox(wInputTB, hInputTB));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(ACTIVATE_INPUT_TEXT_BOX));
    eObj->add(new CRectButton(rectInputTB, rectInputTB, rectInputTB));
    eObj->add(new CRectShape(sf::RectangleShape()));
    eObj->add(new CDisplayer(CDisplayer::INPUT_TEXT, eObj));
    eObj->attachEmployer(eGUI->getObservedEntity("page-connect"));
    eObj->get<CInputTextBox>()->blinkColor = sf::Color::White;
    eObj->get<CInputTextBox>()->setContent(config.getLastNickname());

    /// Quick Match
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-QUICK-MATCH"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, CHANGE_WINDOW_PAGE, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-connect"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("page-quick-match"));
    eObj->get<CButtonTrigger>()->msgs.push_back(FIND_MATCH);

    /// Button Back (hidden)
    y += spcButton;
    eObj = createRectButton("", 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, REMOVE_GUI_GROUP, eGUI->get<CUILayer>()->layer);
    eObj->add(new CButtonHitbox(0,0));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    eObj->attachEmployer(eGUI->getObservedEntity("page-connect"));
    eObj->addObservedEntity("remove-gui-group", eGUI);

    ///================
    /// Quick Match page:
    ///================
    x0 = xPanel;
    y0 = yPanel - hPanel/2 + 30;
    x = x0;
    y = y0;

    /// Title
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-QUICK-MATCH"), Assets::getFont(Assets::getPrimaryFont()), 20, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CPosition(x, y));
    eObj->attachEmployer(eGUI->getObservedEntity("page-quick-match"));
    wTitle = eObj->get<CTextbox2>()->content.getLocalBounds().width;

    /// Underline
    y += 24;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CRectShape(wTitle, 1));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-quick-match"));

    /// Gem
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CTexture("small-orange-gem.png"));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 2)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-quick-match"));

    /// Quick Match server Message
    y += 2*spcButton;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y - spcButton/2));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::White));
    eObj->add(new CDraw(CDraw::GUI_05));
    //eObj->add(new CTypingEffect(Assets::getString("MESSAGE-CONNECTION-LOST"), 40));
    eObj->attachEmployer(eGUI->getObservedEntity("page-quick-match"));
    notify(SET_SERVER_MESSAGE_DISPLAYER_QUICK_MATCH, eObj);

    /// Button Back
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-BACK"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, CHANGE_WINDOW_PAGE, eGUI->get<CUILayer>()->layer);
    //eObj->add(new CButtonHitbox(0,0));
    eObj->get<CButtonTrigger>()->msgs.push_back(CANCEL_QUICK_MATCH_SEARCH);
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    eObj->attachEmployer(eGUI->getObservedEntity("page-quick-match"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("page-connect"));

    return eGUI;
}

Entity* GUIGroupSystem::createCredits(Entity* e) {
    /// e has CGUIGroup, CDraw and CUILayer
    /// make a copy of e
    Entity* eGUI = eManager->createEntity();
    eGUI->add(new CGUIGroup(*e->get<CGUIGroup>()));
    eGUI->add(new CDraw(*e->get<CDraw>()));
    eGUI->add(new CUILayer(*e->get<CUILayer>()));

    eGUI->addObservedEntity("page-1", eManager->createEntity());
    eGUI->addObservedEntity("page-2", eManager->createEntity());
    eGUI->addObservedEntity("page-3", eManager->createEntity());

    eGUI->attachEmployee(eGUI->getObservedEntity("page-1"));
    eGUI->attachEmployee(eGUI->getObservedEntity("page-2"));
    eGUI->attachEmployee(eGUI->getObservedEntity("page-3"));

    eGUI->getObservedEntity("page-1")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("page-2")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("page-3")->addObservedEntity("window", eGUI);

    Entity* eObj;

    ///================
    /// First page:
    ///================
    //double wButton = 180;
    //double hButton = 40;
    double spcButton = 50;
    double wPanel = 290;
    double hPanel = 350;
    double xPanel = cxWindow;
    double yPanel = cyWindow + 100;
    double hText;

    double x0 = xPanel;
    double y0 = yPanel - hPanel/2 + 30;
    double x = x0;
    double y = y0;
    double wTitle;

    sf::Color darkBlue(15, 30, 60);

    /// Panel
    eGUI->add(new CTexture("9p-scroll.png"));
    eGUI->add(new CDimensions(wPanel, hPanel));
    eGUI->add(new CButtonHitbox(wPanel, hPanel));
    eGUI->add(new CButtonState());
    eGUI->add(new CButtonTrigger(EMPTY_MESSAGE));
    eGUI->add(new CPosition(xPanel, yPanel));
    eGUI->add(new CDraw(CDraw::GUI_00));
    eGUI->add(new CUILayer(CUILayer::L1));
    eGUI->add(new CElipsoidalMovement(xPanel, yPanel, 0, 4, 180));

    // ==============
    // @note: Page 1
    // ==============
    x = x0;
    y = y0;

    /// Title
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-CREDITS"), Assets::getFont(Assets::getPrimaryFont()), 20, sf::Color::Black));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CPosition(x, y));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));
    wTitle = eObj->get<CTextbox2>()->content.getLocalBounds().width;

    /// Underline
    y += 24;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CRectShape(wTitle, 1, sf::Color::Black));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 1)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    /// Gem
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + 2, y));
    eObj->add(new CTexture("small-orange-gem.png"));
    eObj->add(new CDraw((CDraw::Tag)((int)eGUI->get<CDraw>()->tag + 2)));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    /// Credit 1
    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-DESIGN-AND-PROGRAMMING") + ":", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    y += hText;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2("Davi Doro", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    /// Credit 2
    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-ART") + ":", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    y += hText;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2("Gabriel Nogueira", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    /// Credit 3
    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-MUSIC") + ":", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    y += hText;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2("Jeff Penny", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    /// Credit 4
    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-LOGO") + ":", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    y += hText;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2("Jonathas Dewojno", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));

    /// Button page down
    y += spcButton;
    std::string buttonDefaultTexture = "down-arrow-01-default.png";
    float wPageButton = Assets::getTexture(buttonDefaultTexture)->getSize().x;
    float hPageButton = Assets::getTexture(buttonDefaultTexture)->getSize().y;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->add(new CTexture(buttonDefaultTexture));
    eObj->add(new CButtonTextures("down-arrow-01-default.png", "down-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wPageButton, hPageButton));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->attachEmployer(eGUI->getObservedEntity("page-1"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("page-2"));

    // ==============
    // @note: Page 2
    // ==============
    x = x0;
    y = y0;

    // @note: up arrow
    y += 15;
    buttonDefaultTexture = "up-arrow-01-default.png";
    wPageButton = Assets::getTexture(buttonDefaultTexture)->getSize().x;
    hPageButton = Assets::getTexture(buttonDefaultTexture)->getSize().y;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->add(new CTexture(buttonDefaultTexture));
    eObj->add(new CButtonTextures("up-arrow-01-default.png", "up-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wPageButton, hPageButton));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->attachEmployer(eGUI->getObservedEntity("page-2"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("page-1"));

    // @note: credit 2.1
    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-WEBSITE-AND-PR") + ":", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-2"));

    y += hText;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2("Thiago Doro", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-2"));

    // @note: credit 2.2: Additional art
    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-SPECIAL-THANKS") + ":", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-2"));

    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2("SFML developers", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-2"));

    y += hText;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2("SFML community", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-2"));

    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-OPENGAMEART-USERS") + ":", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-2"));

    std::vector<std::string> creditNames1 = {
        "daneeklu",
        "JPhilipp",
        "Leonard pabin"
    };

    for (auto nm : creditNames1) {
        //y += hText;
        y += 16.75;
        eObj = eManager->createEntity();
        eObj->add(new CTextbox2(nm, Assets::getFont(Assets::getPrimaryFont()),
                                14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
        hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
        hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
        eObj->add(new CPosition(x, y));
        eObj->add(new CDraw(CDraw::GUI_01));
        eObj->attachEmployer(eGUI->getObservedEntity("page-2"));
    }

    /// Button page down
    y += spcButton;
    buttonDefaultTexture = "down-arrow-01-default.png";
    wPageButton = Assets::getTexture(buttonDefaultTexture)->getSize().x;
    hPageButton = Assets::getTexture(buttonDefaultTexture)->getSize().y;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->add(new CTexture(buttonDefaultTexture));
    eObj->add(new CButtonTextures("down-arrow-01-default.png", "down-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wPageButton, hPageButton));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->attachEmployer(eGUI->getObservedEntity("page-2"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("page-3"));

    // ==============
    // @note: Page 3
    // ==============
    x = x0;
    y = y0;

    // @note: up arrow
    y += 15;
    buttonDefaultTexture = "up-arrow-01-default.png";
    wPageButton = Assets::getTexture(buttonDefaultTexture)->getSize().x;
    hPageButton = Assets::getTexture(buttonDefaultTexture)->getSize().y;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->add(new CTexture(buttonDefaultTexture));
    eObj->add(new CButtonTextures("up-arrow-01-default.png", "up-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wPageButton, hPageButton));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->attachEmployer(eGUI->getObservedEntity("page-3"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("page-2"));

    std::vector<std::string> creditNames2 = {
        "7Soul1",
        "Akachuki",
        "BananazGorilla",
        "geogab7",
        "gameflasher",
        "HauntingEchoes",
        "kevrhodes",
        "manicsfan",
        "SonchuWx33",
        "tenpoundpixel",
        "thelunacy-fringe",
        "shadee"
    };

    y += spcButton/1.5;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-DEVIANTART-USERS") + ":", Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-3"));

    for (auto nm : creditNames2) {
        //y += hText;
        y += 16.75;
        eObj = eManager->createEntity();
        eObj->add(new CTextbox2(nm, Assets::getFont(Assets::getPrimaryFont()),
                                14, sf::Color::Black, 0, 0, CTextbox2::CENTRALIZED));
        hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
        hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
        eObj->add(new CPosition(x, y));
        eObj->add(new CDraw(CDraw::GUI_01));
        eObj->attachEmployer(eGUI->getObservedEntity("page-3"));
    }

    anchorize(eGUI, eGUI->getObservedEntity("page-1")->getEmployees());
    anchorize(eGUI, eGUI->getObservedEntity("page-2")->getEmployees());
    anchorize(eGUI, eGUI->getObservedEntity("page-3")->getEmployees());
    return eGUI;
}

Entity* GUIGroupSystem::createHelp(Entity* e, bool centralized) {
    /// e has CGUIGroup, CDraw and CUILayer
    /// make a copy of e
    Entity* eGUI = eManager->createEntity();
    eGUI->add(new CGUIGroup(*e->get<CGUIGroup>()));
    eGUI->add(new CDraw(*e->get<CDraw>()));
    eGUI->add(new CUILayer(*e->get<CUILayer>()));

    eGUI->addObservedEntity("lesson-1", eManager->createEntity());
    eGUI->addObservedEntity("lesson-2", eManager->createEntity());
    eGUI->addObservedEntity("lesson-3", eManager->createEntity());
    eGUI->addObservedEntity("lesson-4", eManager->createEntity());
    eGUI->addObservedEntity("lesson-5", eManager->createEntity());
    eGUI->addObservedEntity("lesson-6", eManager->createEntity());

    eGUI->attachEmployee(eGUI->getObservedEntity("lesson-1"));
    eGUI->attachEmployee(eGUI->getObservedEntity("lesson-2"));
    eGUI->attachEmployee(eGUI->getObservedEntity("lesson-3"));
    eGUI->attachEmployee(eGUI->getObservedEntity("lesson-4"));
    eGUI->attachEmployee(eGUI->getObservedEntity("lesson-5"));
    eGUI->attachEmployee(eGUI->getObservedEntity("lesson-6"));

    eGUI->getObservedEntity("lesson-1")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("lesson-2")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("lesson-3")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("lesson-4")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("lesson-5")->addObservedEntity("window", eGUI);
    eGUI->getObservedEntity("lesson-6")->addObservedEntity("window", eGUI);

    Entity* eObj;

    ///================
    /// First page:
    ///================
    //double wButton = 180;
    //double hButton = 40;
    double spcButton = 50;
    double wPanel = 376;
    double hPanel = 291;
    double xPanel = cxWindow;
    double yPanel = cyWindow;
    if (!centralized) {
        xPanel = cxWindow;
        yPanel += 100;
    }
    double hText;

    double x0 = xPanel;
    double y0 = yPanel - hPanel/2 + 30;
    double x = x0;
    double y = y0;
    double wTitle;

    sf::Color darkBlue(15, 30, 60);

    /// Panel
    eGUI->add(new CDimensions(wPanel, hPanel));
    eGUI->add(new CButtonHitbox(wPanel, hPanel));
    eGUI->add(new CButtonState());
    eGUI->add(new CButtonTrigger(EMPTY_MESSAGE));
    eGUI->add(new CPosition(xPanel, yPanel));
    eGUI->add(new CUILayer(*e->get<CUILayer>()));

    float xOffArrow = wPanel/2.f - 40;
    float yArrow = yPanel - hPanel/2.f + 27;
    float wArrow = Assets::getTexture("right-arrow-01-default.png")->getSize().x;
    float hArrow = Assets::getTexture("right-arrow-01-default.png")->getSize().y;
    // ================
    // @note: Lesson 1
    // ================

    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel, yPanel));
    eObj->add(new CTexture("lesson-1.png"));
    eObj->add(new CDraw(CDraw::GUI_00));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-1"));

    // @note: right arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel + xOffArrow, yArrow));
    eObj->add(new CTexture("right-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("right-arrow-01-default.png", "right-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-1"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-2"));

    // ================
    // @note: Lesson 2
    // ================

    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel, yPanel));
    eObj->add(new CTexture("lesson-2.png"));
    eObj->add(new CDraw(CDraw::GUI_00));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-2"));

    // @note: right arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel + xOffArrow, yArrow));
    eObj->add(new CTexture("right-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("right-arrow-01-default.png", "right-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-2"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-3"));

    // @note: left arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel - xOffArrow, yArrow));
    eObj->add(new CTexture("left-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("left-arrow-01-default.png", "left-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-2"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-1"));

    // ================
    // @note: Lesson 3
    // ================

    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel, yPanel));
    eObj->add(new CTexture("lesson-3.png"));
    eObj->add(new CDraw(CDraw::GUI_00));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-3"));

    // @note: right arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel + xOffArrow, yArrow));
    eObj->add(new CTexture("right-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("right-arrow-01-default.png", "right-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-3"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-4"));

    // @note: left arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel - xOffArrow, yArrow));
    eObj->add(new CTexture("left-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("left-arrow-01-default.png", "left-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-3"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-2"));

    // ================
    // @note: Lesson 4
    // ================

    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel, yPanel));
    eObj->add(new CTexture("lesson-4.png"));
    eObj->add(new CDraw(CDraw::GUI_00));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-4"));

    // @note: right arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel + xOffArrow, yArrow));
    eObj->add(new CTexture("right-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("right-arrow-01-default.png", "right-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-4"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-5"));

    // @note: left arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel - xOffArrow, yArrow));
    eObj->add(new CTexture("left-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("left-arrow-01-default.png", "left-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-4"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-3"));

    // ================
    // @note: Lesson 5
    // ================

    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel, yPanel));
    eObj->add(new CTexture("lesson-5.png"));
    eObj->add(new CDraw(CDraw::GUI_00));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-5"));

    // @note: right arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel + xOffArrow, yArrow));
    eObj->add(new CTexture("right-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("right-arrow-01-default.png", "right-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-5"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-6"));

    // @note: left arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel - xOffArrow, yArrow));
    eObj->add(new CTexture("left-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("left-arrow-01-default.png", "left-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-5"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-4"));

    // ================
    // @note: Lesson 6
    // ================

    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel, yPanel));
    eObj->add(new CTexture("lesson-6.png"));
    eObj->add(new CDraw(CDraw::GUI_00));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-6"));

    // @note: left arrow
    eObj = eManager->createEntity();
    eObj->add(new CPosition(xPanel - xOffArrow, yArrow));
    eObj->add(new CTexture("left-arrow-01-default.png"));
    eObj->add(new CDraw((CDraw::Tag) ((int) eGUI->get<CDraw>()->tag + 1)));
    eObj->add(new CButtonTextures("left-arrow-01-default.png", "left-arrow-01-highlighted.png"));
    eObj->add(new CButtonHitbox(wArrow, hArrow));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CHANGE_WINDOW_PAGE));
    eObj->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
    eObj->add(new CUILayer(eGUI->get<CUILayer>()->layer));
    eObj->attachEmployer(eGUI->getObservedEntity("lesson-6"));
    eObj->addObservedEntity("change-page", eGUI->getObservedEntity("lesson-5"));

    return eGUI;
}

void GUIGroupSystem::onMouseButtonPressed(Entity* e) {
    for (Entity* eGUI : entities) {
        //if (eGUI->has(Component::BUTTON_HITBOX) && eGUI->has(Component::BUTTON_STATE) && eGUI->get<CUILayer>()->layer == topUILayer){
        if (eGUI->has(Component::BUTTON_HITBOX) && eGUI->has(Component::BUTTON_STATE) && eGUI->get<CUILayer>()->layer == uiLayers.top()) {
            if (eGUI->get<CButtonState>()->state == CButtonState::NON_ACTIVE && eGUI->get<CButtonState>()->state != CButtonState::LOCKED) {
                notify(REMOVE_GUI_GROUP, eGUI);
            }
        }
    }
}

void GUIGroupSystem::onRemoveGUIGroup(Entity* e) {
    Entity* eGUI = nullptr;
    if (e->has(Component::GUI_GROUP)) {
        eGUI = e;
    } else {
        eGUI = e->getObservedEntity("remove-gui-group");
    }
    if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "multiplayer") {
        notify(SET_SERVER_MESSAGE_DISPLAYER_QUICK_MATCH);
        notify(CANCEL_QUICK_MATCH_SEARCH);
    }
    eManager->removeEntity(eGUI);
    notify(BRING_UI_LAYER_FORWARD);
}

void GUIGroupSystem::onChangeWindowPage(Entity* e) {
    Entity* ePage = e->getObservedEntity("change-page");
    showPage(ePage);
}

void GUIGroupSystem::hidePage(Entity* e) {
    EntityList eObjects = e->getEmployees();
    for (Entity* eObj : eObjects) {
        if (eObj->has(Component::DRAW)) {
            eObj->get<CDraw>()->isHidden = true;
        }
        if (eObj->has(Component::BUTTON_STATE)) {
            eObj->get<CButtonState>()->state = CButtonState::LOCKED;
        }
    }
}

void GUIGroupSystem::showPage(Entity* e) {
    Entity* eWindow = e->getObservedEntity("window");
    EntityList ePages = eWindow->getEmployees();
    for (Entity* ePage : ePages) {
        if (ePage != e) {
            hidePage(ePage);
        }
    }

    EntityList eObjects = e->getEmployees();
    for (Entity* eObj : eObjects) {
        if (eObj->has(Component::DRAW)) {
            eObj->get<CDraw>()->isHidden = false;
        }
        if (eObj->has(Component::BUTTON_STATE)) {
            eObj->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
        }
    }
}


Entity* GUIGroupSystem::createRectButton(string label, double fontSize, double h, double x, double y, sf::Color textColor, sf::Color fillColor, sf::Color outColor, double outThickness, Message m, CUILayer::Layer UILayer, bool unavailable) {
    sf::RectangleShape rect;
    double w = 0;
    double sideSpacing = 10;
    // @hack: unavailable options
    if (unavailable) {
        sf::Color grey(155, 155, 155);
        Entity* e = eManager->createEntity();
        e->add(new CTextbox2(label, Assets::getFont(Assets::getPrimaryFont()), fontSize, grey));
        w = e->get<CTextbox2>()->content.getLocalBounds().width + 2*sideSpacing;
        e->add(new CPosition(x, y));
        e->add(new CDimensions(w, h));
        e->add(new CDraw(CDraw::GUI_01));
        return e;
    } else {
        Entity* e = eManager->createEntity();
        e->add(new CTextbox2(label, Assets::getFont(Assets::getPrimaryFont()), fontSize, textColor));
        w = e->get<CTextbox2>()->content.getLocalBounds().width + 2*sideSpacing;
        rect.setOutlineColor(outColor);
        rect.setOutlineThickness(outThickness);
        rect.setPosition(x, y);
        rect.setSize(sf::Vector2f(w, h));
        rect.setFillColor(fillColor);
        e->add(new CRectShape(sf::RectangleShape()));
        e->add(new CPosition(x, y));
        e->add(new CDimensions(w, h));
        e->add(new CButtonHitbox(w, h));
        e->add(new CButtonState());
        e->add(new CButtonTrigger(m));
        e->add(new CRectButton(sf::RectangleShape(), rect, rect));
        e->add(new CDraw(CDraw::GUI_01));
        e->add(new CButtonSounds("click2.ogg", "rollover2.ogg"));
        e->add(new CUILayer(UILayer));
        return e;
    }
}

void GUIGroupSystem::onDisconnectFromServer(Entity* e) {
    for (auto& i : entities) {
        if (i->get<CGUIGroup>()->groupType == "window" && i->get<CGUIGroup>()->groupID == "multiplayer") {
            eManager->removeEntity(i);
        }
    }
}

void GUIGroupSystem::onUpdatePlayerNicknameWithInputTextbox(Entity* e) {
    config.setLastNickname(e->get<CInputTextBox>()->content);
    config.saveConfigFile(helper::getAppDataDir() + "/config.xml");
}

void GUIGroupSystem::anchorize(Entity* eAnchor, EntityList eList) {
    float xAnchor = eAnchor->get<CPosition>()->x;
    float yAnchor = eAnchor->get<CPosition>()->y;
    for (Entity* e : eList) {
        float xOff = e->get<CPosition>()->x - xAnchor;
        float yOff = e->get<CPosition>()->y - yAnchor;
        e->add(new CAnchor(xOff, yOff));
        e->addObservedEntity("Anchor", eAnchor);
    }
}



