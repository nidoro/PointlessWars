#include "GUIGroupSystem.h"

GUIGroupSystem::GUIGroupSystem(){
    addSubscription(CREATE_GUI_GROUP);
    addSubscription(REMOVE_GUI_GROUP);
    addSubscription(CHANGE_WINDOW_PAGE);
    addSubscription(MOUSE_BUTTON_RELEASED);

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
    }else if (eGUI->get<CGUIGroup>()->groupType == "window-page"){

    }
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

void GUIGroupSystem::onMouseButtonReleased(Entity* e){
    for (Entity* eGUI : entities){
        if (eGUI->has(Component::BUTTON_HITBOX)){
            sf::FloatRect hitbox;
            hitbox.left = eGUI->get<CPosition>()->x - eGUI->get<CButtonHitbox>()->width/2;
            hitbox.top = eGUI->get<CPosition>()->y - eGUI->get<CButtonHitbox>()->height/2;
            hitbox.width = eGUI->get<CButtonHitbox>()->width;
            hitbox.height = eGUI->get<CButtonHitbox>()->height;
            sf::Vector2i mousePosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
            sf::Vector2f mp = window->mapPixelToCoords(mousePosition);
            if (!hitbox.contains(mp)){
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
    eManager->removeEntity(eGUI);
    notify(BRING_UI_LAYER_FORWARD);
}

void GUIGroupSystem::onChangeWindowPage(Entity* e){
    Entity* ePage = e->getObservedEntity("page");
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







