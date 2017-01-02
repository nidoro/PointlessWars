#include "InGameMenu.h"

InGameMenu::InGameMenu(){
    addSubscription(OPEN_MENU);
    addSubscription(RESUME_GAME);
    addSubscription(CHANGE_MENU_PAGE);
    addSubscription(HAS_BEEN_TOGGLED);
    addSubscription(HAS_CHANGED_VALUE);

    ePanel = nullptr;
    UILayer = CUILayer::L1;
}

InGameMenu::~InGameMenu(){

}

void InGameMenu::onOpenMenu(Entity* e){
    //notify(LOCK_ALL_BUTTONS);
    Entity* eLayer = eManager->createEntity();
    eLayer->add(new CUILayer(UILayer));
    notify(BRING_UI_LAYER_FORWARD, eLayer);
    //eManager->removeEntity(eLayer);

    create();
    showPage(CInGameMenuPage::FIRST);
}

void InGameMenu::create(){
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
    ePanel->add(new CDraw(CDraw::GUI_00));
    ePanel->add(new CPanel());

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

    /// BUTTON OPTIONS
    y += spacing;
    eObj = createRectButton(Assets::getString("BUTTON-OPTIONS"), 18, 40, x, y, sf::Color::White, sf::Color::White, 1, CHANGE_MENU_PAGE);
    eObj->add(new CInGameMenuPage(CInGameMenuPage::OPTIONS));
    addToPage(eObj, CInGameMenuPage::FIRST);
    ///=====================
    /// OPTIONS
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;
    double sliderSize = 125;
    double hText;
    /// MUSIC LABEL
    y += spacing;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-MUSIC"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    addToPage(eObj, CInGameMenuPage::OPTIONS);

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
    eObj->add(new CUILayer(UILayer));
    addToPage(eObj, CInGameMenuPage::OPTIONS);

    /// MUSIC SLIDER BAR
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x + sliderSize/2.f - 5, y));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CDimensions(sliderSize, 2));
    eObj->add(new CRectShape(sliderSize, 2, sf::Color::White));
    addToPage(eObj, CInGameMenuPage::OPTIONS);

    /// RESOLUTION LABEL
    y += spacing;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-RESOLUTION"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    addToPage(eObj, CInGameMenuPage::OPTIONS);

    /// RESOLUTION DROP LIST
    list<string> resStrings;
    unsigned int bbp = sf::VideoMode::getFullscreenModes()[0].bitsPerPixel;
    for (int i = 0; i < sf::VideoMode::getFullscreenModes().size(); i++){
        if (sf::VideoMode::getFullscreenModes()[i].bitsPerPixel != bbp) continue;
        resStrings.push_back(int2str(sf::VideoMode::getFullscreenModes()[i].width) + " x " + int2str(sf::VideoMode::getFullscreenModes()[i].height));
    }
    string initValue = int2str(config.getResolution().x) + " x " + int2str(config.getResolution().y);
    eObj = eManager->createEntity();
    eObj->add(new CDropList(resStrings, initValue));
    eObj->add(new CPosition(x + sliderSize/2.f - 5, y));
    eObj->add(new CDraw(CDraw::GUI_02));
    eObj->add(new CButtonHitbox(120, 20));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(EXPAND_DROP_LIST));
    eObj->add(new CTextbox2(initValue, Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::CENTRALIZED));
    eObj->add(new CDimensions(120, 20));
    eObj->add(new CRectShape(120, 20, sf::Color::Black, 1, sf::Color::White));
    eObj->add(new CRectButton(sf::RectangleShape(), eObj->get<CRectShape>()->shape, eObj->get<CRectShape>()->shape));
    eObj->add(new CUILayer(UILayer));
    ePanel->addObservedEntity("OptionResolution", eObj);
    addToPage(eObj, CInGameMenuPage::OPTIONS);
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
    y += spacing;
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2(Assets::getString("LABEL-FULLSCREEN"), Assets::getFont(Assets::getPrimaryFont()),
                            14, sf::Color::White, 0, 0, CTextbox2::NORMAL));
    hText = eObj->get<CTextbox2>()->content.getLocalBounds().height;
    hText += eObj->get<CTextbox2>()->content.getFont()->getLineSpacing(eObj->get<CTextbox2>()->content.getCharacterSize())/4;
    eObj->add(new CPosition(x - 125, y - hText/2.f));
    eObj->add(new CDraw(CDraw::GUI_01));
    addToPage(eObj, CInGameMenuPage::OPTIONS);

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
    eObj->add(new CRectShape(120, 20, sf::Color::Black, 1, sf::Color::White));
    eObj->add(new CRectButton(sf::RectangleShape(), eObj->get<CRectShape>()->shape, hovShape));
    eObj->add(new CUILayer(UILayer));
    eObj->add(new CStringToggleButton(Assets::getString("LABEL-ON"), Assets::getString("LABEL-OFF"), config.getFullscreen() ? 0:1));
    eObj->add(new CDisplayer(CDisplayer::STR_TOGGLE_BUTTON, eObj));
    ePanel->addObservedEntity("OptionFullscreen", eObj);
    addToPage(eObj, CInGameMenuPage::OPTIONS);

    /// BUTTON BACK
    y += spacing;
    eObj = createRectButton(Assets::getString("BUTTON-BACK"), 18, 40, x, y, sf::Color::White, sf::Color::White, 1, CHANGE_MENU_PAGE);
    eObj->add(new CInGameMenuPage(CInGameMenuPage::FIRST));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CInGameMenuPage::OPTIONS);
}

void InGameMenu::onChangeMenuPage(Entity* e){
    if (e->has(Component::IN_GAME_MENU_PAGE)){
        showPage(e->get<CInGameMenuPage>()->id);
    }

    if (getPage(e) == CInGameMenuPage::OPTIONS){
        config.saveConfigFile("config.xml");
    }
}

CInGameMenuPage::ID InGameMenu::getPage(Entity* e){
    for (int i = 0; i < pages.size(); i++){
        for (Entity* eObj : pages[i]){
            if (eObj == e) return (CInGameMenuPage::ID) i;
        }
    }
    return (CInGameMenuPage::ID) 0;
}

void InGameMenu::addToPage(Entity* e, CInGameMenuPage::ID id){
    pages[id].push_back(e);
    ePanel->get<CPanel>()->objects.push_back(e);
    ePanel->attachEmployee(e);
}

void InGameMenu::showPage(CInGameMenuPage::ID id){
    for(unsigned i = 0; i < pages.size(); i++){
        for(EntityListIt it = pages[i].begin(); it != pages[i].end(); it++){
            Entity* eObj = *it;
            if (eObj->has(Component::BUTTON_STATE)){
                eObj->get<CButtonState>()->state = CButtonState::LOCKED;
            }
            eObj->get<CDraw>()->isHidden = true;

        }
    }

    for(EntityListIt it = pages[id].begin(); it != pages[id].end(); it++){
        Entity* eObj = *it;
        if (eObj->has(Component::BUTTON_STATE)){
            eObj->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
        }
        eObj->get<CDraw>()->isHidden = false;
    }
}

void InGameMenu::clearPage(CInGameMenuPage::ID id){
    for(EntityListIt i = pages[id].begin(); i != pages[id].end(); i++){
        Entity* e = *i;
        eManager->removeEntity(e);
    }
    pages[id].clear();
}

Entity* InGameMenu::createButton(string label, double w, double h, double x, double y, Message m){
    Entity* e = eManager->createEntity();
    e->add(new CTexture());
    e->add(new CPosition(x, y));
    e->add(new CDimensions(w, h));
    e->add(new CButtonHitbox(w, h));
    e->add(new CButtonState());
    e->add(new CButtonTrigger(m));
    e->add(new CDefaultTexture("tooltip-02.png"));
    e->add(new CHoverTexture("tooltip-02.png"));
    e->add(new CActiveTexture("tooltip-02.png"));
    e->add(new CDraw(CDraw::GUI_01));
    e->add(new CTextbox2(label, Assets::getFont(Assets::getPrimaryFont()), 18, sf::Color::White));
    e->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    e->add(new CUILayer(UILayer));
    return e;
}

Entity* InGameMenu::createRectButton(string label, double fontSize, double h, double x, double y, sf::Color textColor, sf::Color outColor, double outThickness, Message m){
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
    rect.setFillColor(sf::Color::Black);
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

void InGameMenu::onResumeGame(Entity* e){
    notify(REMOVE_PANEL, ePanel);
    notify(BRING_UI_LAYER_FORWARD);
    //notify(UNLOCK_ALL_BUTTONS);
    ePanel = nullptr;
}

void InGameMenu::onHasBeenToggled(Entity* e){
    if (ePanel){
        if (e == ePanel->getObservedEntity("OptionFullscreen")){
            config.setFullscreen(e->get<CStringToggleButton>()->valueIndex == 0 ? true:false);
            window->create(sf::VideoMode(config.getResolution().x, config.getResolution().y),
                          "Pointless Wars",
                          config.getFullscreen() ? sf::Style::Fullscreen : sf::Style::Default);
            window->setVerticalSyncEnabled(true);
            window->setMouseCursorVisible(false);
            config.saveConfigFile("config.xml");
        }
    }
}

void InGameMenu::onHasChangedValue(Entity* e){
    if (ePanel){
        if (e == ePanel->getObservedEntity("OptionResolution")){
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

            config.saveConfigFile("config.xml");
        }
    }
}

