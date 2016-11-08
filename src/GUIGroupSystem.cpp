#include "GUIGroupSystem.h"

GUIGroupSystem::GUIGroupSystem(){
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

void GUIGroupSystem::update(){

}

void GUIGroupSystem::onCreateGUIGroup(Entity* e){
    Entity* eGUI = e->getObservedEntity("create-gui-group");

    if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "single-player"){
        createWindowSinglePlayer(eGUI);
        notify(BRING_UI_LAYER_FORWARD, eGUI);
    }else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "options"){
        createWindowOptions(eGUI);
        notify(BRING_UI_LAYER_FORWARD, eGUI);
    }else if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "multiplayer"){
        Entity* eWin = createWindowMultiplayer(eGUI);
        showPage(eWin->getObservedEntity("page-connect"));
        notify(BRING_UI_LAYER_FORWARD, eWin);
    }else if (eGUI->get<CGUIGroup>()->groupType == "window-page"){

    }
}

void GUIGroupSystem::createWindowOptions(Entity* e){
    /// e has CGUIGroup, CDraw and CUILayer
    /// make a copy of e
    Entity* eGUI = eManager->createEntity();
    eGUI->add(new CGUIGroup(*e->get<CGUIGroup>()));
    eGUI->add(new CDraw(*e->get<CDraw>()));
    eGUI->add(new CUILayer(*e->get<CUILayer>()));

    eGUI->addObservedEntity("page-first", eManager->createEntity());

    eGUI->attachEmployee(eGUI->getObservedEntity("page-first"));

    eGUI->getObservedEntity("page-first")->addObservedEntity("window", eGUI);

    Entity* eObj;

    ///================
    /// First page:
    ///================
    double wButton = 180;
    double hButton = 40;
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
    eObj->add(new CTextbox2(Assets::getString("LABEL-OPTIONS"), Assets::getFont(Assets::getPrimaryFont()), 20, sf::Color::White));
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

    double sliderSize = 125;
    double hText;
    /// MUSIC LABEL
    y += spcButton;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-MUSIC"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

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
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// MUSIC SLIDER BAR
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + sliderSize/2.f - 5, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CDimensions(sliderSize, 2));
    eObj->add(new CRectShape(sliderSize, 2, sf::Color::White));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// RESOLUTION LABEL
    y += spcButton;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-RESOLUTION"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// RESOLUTION DROP LIST
    list<string> resStrings;
    unsigned int bbp = sf::VideoMode::getFullscreenModes()[0].bitsPerPixel;
    for (int i = 0; i < sf::VideoMode::getFullscreenModes().size(); i++){
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
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

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
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

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
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Button Back (hidden)
    y += spcButton;
    eObj = createRectButton("", 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, REMOVE_GUI_GROUP, eGUI->get<CUILayer>()->layer);
    eObj->add(new CButtonHitbox(0,0));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    eObj->addObservedEntity("remove-gui-group", eGUI);
}
void GUIGroupSystem::createWindowSinglePlayer(Entity* e){
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
    double wButton = 180;
    double hButton = 40;
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
    eObj = createRectButton(Assets::getString("LABEL-TUTORIAL"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Quick Match
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-QUICK-MATCH"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Campaign
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-CAMPAIGN"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Arena
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-ARENA"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Custom
    y += spcButton;
    eObj = createRectButton(Assets::getString("LABEL-CUSTOM-GAME"), 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, EMPTY_MESSAGE, eGUI->get<CUILayer>()->layer);
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));

    /// Button Back (hidden)
    y += spcButton;
    eObj = createRectButton("", 18, 40, x, y, sf::Color::White, darkBlue, sf::Color::White, 2, REMOVE_GUI_GROUP, eGUI->get<CUILayer>()->layer);
    eObj->add(new CButtonHitbox(0,0));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    eObj->attachEmployer(eGUI->getObservedEntity("page-first"));
    eObj->addObservedEntity("remove-gui-group", eGUI);
    /*
    pages.clear();
    pages.resize(CInGameMenuPage::N_PAGES);


    Entity* eObj;

    double w = 180;
    double h = 40;
    double spacing = 50;
    double x0 = cxWindow;
    double y0 = cyWindow - spacing/2;
    double x = x0;
    double y = y0;

    ePanel = eManager->createEntity();
    ePanel->add(new CTexture("9p-grey-frame.png"));
    ePanel->add(new CDimensions(290, 300));
    ePanel->add(new CPosition(cxWindow, cyWindow));
    ePanel->add(new CDraw(CDraw::GUI_00)); ePanel->add(new CPanel());
    ///=====================
    /// FIRST PAGE
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON RESUME
    y += spacing;
    eObj = createRectButton(Assets::getString("BUTTON-RESUME"), 18, 40, x, y, sf::Color::White, sf::Color::White, 1, RESUME_GAME);
    eObj->add(new CInGameMenuPage(CInGameMenuPage::FIRST));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CInGameMenuPage::FIRST);
    */
}
Entity* GUIGroupSystem::createWindowMultiplayer(Entity* e){
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
    double wButton = 180;
    double hButton = 40;
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

void GUIGroupSystem::onMouseButtonPressed(Entity* e){
    for (Entity* eGUI : entities){
        if (eGUI->has(Component::BUTTON_HITBOX) && eGUI->has(Component::BUTTON_STATE) && eGUI->get<CUILayer>()->layer == topUILayer){
            if (eGUI->get<CButtonState>()->state == CButtonState::NON_ACTIVE && eGUI->get<CButtonState>()->state != CButtonState::LOCKED){
                notify(REMOVE_GUI_GROUP, eGUI);
            }
        }
    }
}

void GUIGroupSystem::onRemoveGUIGroup(Entity* e){
    Entity* eGUI = nullptr;
    if (e->has(Component::GUI_GROUP)){
        eGUI = e;
    }else{
        eGUI = e->getObservedEntity("remove-gui-group");
    }
    if (eGUI->get<CGUIGroup>()->groupType == "window" && eGUI->get<CGUIGroup>()->groupID == "multiplayer"){
        notify(SET_SERVER_MESSAGE_DISPLAYER_QUICK_MATCH);
        notify(CANCEL_QUICK_MATCH_SEARCH);
    }
    eManager->removeEntity(eGUI);
    notify(BRING_UI_LAYER_FORWARD);
}

void GUIGroupSystem::onChangeWindowPage(Entity* e){
    Entity* ePage = e->getObservedEntity("change-page");
    showPage(ePage);
}

void GUIGroupSystem::hidePage(Entity* e){
    EntityList eObjects = e->getEmployees();
    for (Entity* eObj : eObjects){
        if (eObj->has(Component::DRAW)){
            eObj->get<CDraw>()->isHidden = true;
        }
        if (eObj->has(Component::BUTTON_STATE)){
            eObj->get<CButtonState>()->state = CButtonState::LOCKED;
        }
    }
}

void GUIGroupSystem::showPage(Entity* e){
    Entity* eWindow = e->getObservedEntity("window");
    EntityList ePages = eWindow->getEmployees();
    for (Entity* ePage : ePages){
        if (ePage != e){
            hidePage(ePage);
        }
    }

    EntityList eObjects = e->getEmployees();
    for (Entity* eObj : eObjects){
        if (eObj->has(Component::DRAW)){
            eObj->get<CDraw>()->isHidden = false;
        }
        if (eObj->has(Component::BUTTON_STATE)){
            eObj->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
        }
    }
}


Entity* GUIGroupSystem::createRectButton(string label, double fontSize, double h, double x, double y, sf::Color textColor, sf::Color fillColor, sf::Color outColor, double outThickness, Message m, CUILayer::Layer UILayer){
    sf::RectangleShape rect;
    double w = 0;
    double sideSpacing = 10;
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
    e->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    e->add(new CUILayer(UILayer));
    return e;
}

void GUIGroupSystem::onDisconnectFromServer(Entity* e){
    for (auto& i : entities){
        if (i->get<CGUIGroup>()->groupType == "window" && i->get<CGUIGroup>()->groupID == "multiplayer"){
            eManager->removeEntity(i);
        }
    }
}





