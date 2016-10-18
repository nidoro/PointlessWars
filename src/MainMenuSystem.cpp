#include "MainMenuSystem.h"

MainMenuSystem::MainMenuSystem(){
    addSubscription(CREATE_SCREEN);
    addSubscription(CHANGE_MENU_PAGE);
    addSubscription(CREATE_PLAYER_EDITOR);
    addSubscription(CREATE_PROFILE);
    addSubscription(SELECT_CHARACTER);

    addSubscription(CREATE_PROFILE_MENU);
    addSubscription(EDIT_PROFILE);
    addSubscription(DELETE_PROFILE);
    addSubscription(SELECT_PROFILE);
}

MainMenuSystem::~MainMenuSystem(){
    //dtor
}

void MainMenuSystem::update(){
    //updatePlayers();
}

void MainMenuSystem::onCreateScreen(Entity* e){
    switch(e->get<CScreen>()->id){
        case CScreen::MAIN_MENU: eManager->clearSystem(); createMenu(); notify(NEW_SCREEN); break;
        default: break;
    }
}

void MainMenuSystem::create(){
    pages.clear();
    pages.resize(CMainMenuPage::N_PAGES);

    Entity* eObj;

    double w = 180;
    double h = 40;
    double spacing = 50;
    double x0 = cxWindow;
    double y0 = 300;
    double x = x0;
    double y = y0;

    ///BACKGROUND
    Entity* eBack = eManager->createEntity();
    eBack->add(new CPosition(cxWindow, cyWindow));
    eBack->add(new CTexture("background-woods-01.png"));
    eBack->add(new CDraw(CDraw::BACKGROUND));

    /// TITLE
    Entity* eTitle = eManager->createEntity();
    eTitle->add(new CPosition(cxWindow, 200));
    eTitle->add(new CDraw(CDraw::GUI1));
    eTitle->add(new CTexture("title-00.png"));
    /*
    Entity* eTitle = eManager->createEntity();
    eTitle->add(new CPosition(cxWindow - 200, 200));
    eTitle->add(new CDraw(CDraw::GUI1));
    eTitle->add(new CTexture("P1.png"));
    eTitle = eManager->createEntity();
    eTitle->add(new CPosition(cxWindow - 200, 200));
    eTitle->add(new CDraw(CDraw::GUI3));
    eTitle->add(new CTexture("P2.png"));
    ///UNIT

    eObj = eManager->createEntity();
    eObj->add(new CPosition(wWindow - 50, cyWindow));
    eObj->add(new CAnimation(true, CUnit::Map[1].aIdle));
    eObj->add(new CDraw(CDraw::WORLD));
    eObj->add(new CActor());
    eObj->get<CActor>()->timeline.push_back(new ASpriteAnimation(3, CUnit::Map[1].aAction01));
    eObj->get<CActor>()->timeline.push_back(new AShoot(Assets::getAnimation(CUnit::Map[1].aAction01).duration, cxWindow - 200, 200, AShoot::ERIDU_BLADE));
    eObj->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, CUnit::Map[1].aIdle));

    ///SWORD
    eObj = eManager->createEntity();
    eObj->add(new CPosition(-1000, -1000));
    eObj->add(new CDraw(CDraw::GUI1));
    eObj->add(new CTexture("eridu-blade.png"));
    eObj->add(new CActor());

    eObj->get<CActor>()->timeline.push_back(new ATeleport(3 + Assets::getAnimation(CUnit::Map[1].aAction01).duration + 3, cxWindow - 200, 200));
    */

    ///=====================
    /// MAIN SCREEN PAGE
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON FIGHT
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-FIGHT"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::FIGHT));
    addToPage(eObj, CMainMenuPage::MAIN_SCREEN);

    /// BUTTON OPTIONS
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-OPTIONS"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::OPTIONS));
    addToPage(eObj, CMainMenuPage::MAIN_SCREEN);

    /// BUTTON CUSTOM FORMATIONS
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-FORMATIONS"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::FORMATIONS));
    addToPage(eObj, CMainMenuPage::MAIN_SCREEN);

    /// BUTTON BACK
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-BACK"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::SELECT_PLAYER));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CMainMenuPage::MAIN_SCREEN);
    ///=====================
    /// CUSTOM FORMATIONS
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;

    /// FORMATION EDITOR
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-FORMATION-EDITOR"), w, h, x, y, START_SCREEN_TRANSITION);
    eObj->add(new CScreen(CScreen::FORMATION_EDITOR, CScreen::FADE_BLACK));
    addToPage(eObj, CMainMenuPage::FORMATIONS);
    /// BUTTON BACK
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-BACK"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::MAIN_SCREEN));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CMainMenuPage::FORMATIONS);

    ///=====================
    /// SELECT PLAYER
    ///=====================
    createPlayerSelect();
    ///=====================
    /// OPTIONS PAGE
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON BACK
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-BACK"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::MAIN_SCREEN));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CMainMenuPage::OPTIONS);
    ///=====================
    /// FIGHT PAGE
    ///=====================
    x0 = cxWindow - w/2 - 10/2;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON CAMPAIGN
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-CAMPAIGN"), w, h, x, y);
    addToPage(eObj, CMainMenuPage::FIGHT);

    /// BUTTON QUICK GAME
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-QUICK-GAME"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::QUICK_GAME));
    addToPage(eObj, CMainMenuPage::FIGHT);

    /// BUTTON LAN
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-LAN"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::LAN));
    addToPage(eObj, CMainMenuPage::FIGHT);

    x0 = cxWindow + w/2 + 10/2;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON ONLINE
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-ONLINE"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::ONLINE));
    addToPage(eObj, CMainMenuPage::FIGHT);

    /// BUTTON ARENA
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-ARENA"), w, h, x, y);
    addToPage(eObj, CMainMenuPage::FIGHT);

    /// BUTTON CUSTOM GAME
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-CUSTOM-GAME"), w, h, x, y, START_SCREEN_TRANSITION);
    eObj->add(new CScreen(CScreen::CUSTOM_MATCH, CScreen::FADE_BLACK));
    addToPage(eObj, CMainMenuPage::FIGHT);

    /// BUTTON BACK
    x = cxWindow;
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-BACK"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::MAIN_SCREEN));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CMainMenuPage::FIGHT);

    ///=====================
    /// QUICK-GAME PAGE
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON BACK
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-BACK"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::FIGHT));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CMainMenuPage::QUICK_GAME);

    ///=====================
    /// LAN PAGE
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON BACK
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-BACK"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::FIGHT));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CMainMenuPage::LAN);

    ///=====================
    /// ONLINE PAGE
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON BACK
    y += spacing;
    eObj = createButton(Assets::getString("BUTTON-BACK"), w, h, x, y, CHANGE_MENU_PAGE);
    eObj->add(new CMainMenuPage(CMainMenuPage::FIGHT));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CMainMenuPage::ONLINE);


    showPage(CMainMenuPage::MAIN_SCREEN);
}

void MainMenuSystem::createMenu(){
    Entity* eObj;

    ///BACKGROUND 1
    /*
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->add(new CTexture("background-woods-01.png"));
    eObj->add(new CDraw(CDraw::BACKGROUND));

    ///BACKGROUND 2
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow + wWindow, cyWindow + hWindow));
    eObj->add(new CTexture("background-woods-01.png"));
    eObj->add(new CDraw(CDraw::BACKGROUND));
    */

    Entity* eScenario = eManager->createEntity();
    eScenario->add(new CScenario("bosque.sce", -wWindow, 0));
    notify(LOAD_SCENARIO, eScenario);

    eScenario->add(new CScenario("bosque.sce"));
    notify(LOAD_SCENARIO, eScenario);

    ///CAMERA MAN
    eObj = eManager->createEntity();
    eObj->add(new CPosition(-wWindow/4, cyWindow));
    eObj->add(new CActor());
    eObj->add(new CVelocity());
    eObj->add(new CCameraMan());
    eObj->get<CActor>()->addNode(new AMove(0.0, cxWindow, cyWindow, 150));
    eObj->get<CActor>()->addNode(new ADestroy(getTravelTime(-wWindow/4, cyWindow, cxWindow, cyWindow, 150)));

    ///TITLE
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, 80));
    eObj->add(new CDraw(CDraw::GUI1, 0.0));
    eObj->add(new CTexture("pw-logo.png"));
    eObj->add(new CElipsoidalMovement(cxWindow, 130, 0, 10, 180));
    eObj->add(new CActor());
    eObj->get<CActor>()->addNode(new AFade(getTravelTime(-wWindow/4, cyWindow, cxWindow, cyWindow, 150)-0.5, 255, 255));

}

Entity* MainMenuSystem::createButton(string label, double w, double h, double x, double y, Message m){
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
    e->add(new CDraw(CDraw::GUI1));
    e->add(new CTextbox2(label, Assets::getFont(Assets::getPrimaryFont()), 18, sf::Color::White));
    e->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    e->add(new CSound());
    return e;
}

void MainMenuSystem::addToPage(Entity* e, CMainMenuPage::ID id){
    pages[id].push_back(e);
}

void MainMenuSystem::showPage(CMainMenuPage::ID id){
    for(unsigned i = 0; i < pages.size(); i++){
        for(EntityListIt it = pages[i].begin(); it != pages[i].end(); it++){
            Entity* eObj = *it;
            eObj->get<CButtonState>()->state = CButtonState::LOCKED;
            eObj->get<CDraw>()->isHidden = true;
        }
    }

    for(EntityListIt it = pages[id].begin(); it != pages[id].end(); it++){
        Entity* eObj = *it;
        eObj->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
        eObj->get<CDraw>()->isHidden = false;
    }
}

void MainMenuSystem::onChangeMenuPage(Entity* e){
    if (e->has(Component::MAIN_MENU_PAGE)){
        showPage(e->get<CMainMenuPage>()->id);
    }
}

void MainMenuSystem::updatePlayers(){
    XMLDocument doc;
    string path = "../rsc-0.1/profiles.xml";
    if (doc.LoadFile(path.c_str()) != XML_NO_ERROR){
        printf("Could not open profiles.xml!\n");
    }
    XMLElement* element = doc.FirstChildElement();

    CProfile profile;
    for (element; element != nullptr; element = element->NextSiblingElement()){
        profile.name = element->Value();
    }
}

void MainMenuSystem::clearPage(CMainMenuPage::ID id){
    for(EntityListIt i = pages[id].begin(); i != pages[id].end(); i++){
        Entity* e = *i;
        eManager->removeEntity(e);
    }
    pages[id].clear();
}

void MainMenuSystem::createPlayerSelect(){
    loadProfiles();
    clearPage(CMainMenuPage::SELECT_PLAYER);

    Entity* eProfileList = eManager->createEntity();
    eProfileList->add(new CPanel());
    ///=====================
    /// SELECT PLAYER
    ///=====================
    Entity* eObj;
    int nOptions = profiles.size();
    double spacing = 60;
    double x0 = cxWindow - (nOptions)*spacing/2;
    double y0 = 250;
    double x = x0;
    double y = y0;
    double w = 60;
    double h = 60;

    for(list<CProfile>::iterator i = profiles.begin(); i != profiles.end(); i++){
        CProfile profile = *i;
        eObj = eManager->createEntity();
        eObj->add(new CPosition(x, y));
        eObj->add(new CDraw(CDraw::GUI2));
        eObj->add(new CDimensions(60, 60));
        eObj->add(new CChar(CChar::Map.begin()->second));
        eObj->add(new CAnimation(false, CChar::Map.begin()->second.aIdle));
        eObj->add(new CButtonState());
        eObj->add(new CButtonTrigger(CREATE_PROFILE_MENU));
        eObj->add(new CButtonHitbox(60, 60));
        eObj->add(new CPanel());
        eObj->add(new CParentPanel(eProfileList));
        eObj->add(new CProfile(profile));
        eObj->get<CButtonState>()->lostFocusMessage = SWITCH_CHAR_ANIMATION;
        eObj->get<CButtonState>()->gainedFocusMessage = SWITCH_CHAR_ANIMATION;
        eProfileList->get<CPanel>()->objects.push_back(eObj);
        eProfileList->attachEmployee(eObj);
        addToPage(eObj, CMainMenuPage::SELECT_PLAYER);
        x += spacing;
    }

    ///NEW CHAR
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y));
    eObj->add(new CDraw(CDraw::GUI2));
    eObj->add(new CDimensions(60, 60));
    eObj->add(new CAnimation(false, "new-char-idle.png"));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(CREATE_PLAYER_EDITOR));
    eObj->add(new CButtonHitbox(60, 60));
    eObj->add(new CPanel());
    eObj->add(new CParentPanel(eProfileList));
    eProfileList->get<CPanel>()->objects.push_back(eObj);
    eProfileList->attachEmployee(eObj);
    addToPage(eObj, CMainMenuPage::SELECT_PLAYER);
}

void MainMenuSystem::loadProfiles(){
    profiles.clear();

    XMLDocument doc;
    string path = "../rsc-0.1/profiles.xml";
    if (doc.LoadFile(path.c_str()) != XML_NO_ERROR){
        printf("Could not open profiles.xml!\n");
    }
    XMLElement* element = doc.FirstChildElement("Profile");

    for (element; element != nullptr; element = element->NextSiblingElement("Profile")){
        CProfile profile;
        profile.name = string(element->FirstChildElement("Name")->GetText());
        profile.charName = element->FirstChildElement("Char")->GetText();
        profiles.push_back(profile);
    }
}

void MainMenuSystem::onCreatePlayerEditor(Entity* e){
    notify(LOCK_ALL_BUTTONS);
    if (!e->has(Component::PROFILE)) notify(REMOVE_NEPHEWS, e);

    Entity* ePanel = eManager->createEntity();
    ePanel->add(new CPosition(cxWindow, cyWindow));
    ePanel->add(new CTexture("back-player-editor.png"));
    ePanel->add(new CDraw(CDraw::GUI2));
    ePanel->add(new CPanel());

    double x = cxWindow;
    double y = cyWindow - 50;
    double w,h;

    Entity* eObj;
    Entity* eInput;
    Entity* eChar;

    CProfile profile;
    if (e->has(Component::PROFILE)){
        profile = *e->get<CProfile>();
    }else{
        profile.name = "";
        profile.charName = CChar::Map.begin()->second.name;
    }
    /// INPUT BOX
    eObj = eManager->createEntity();
    eInput = eObj;
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->add(new CInputTextBox(true, CInputTextBox::INACTIVE, 10));
    eObj->add(new CRectShape(100, 20, sf::Color::White, 1, sf::Color::Black));
    eObj->add(new CDisplayer(CDisplayer::INPUT_TEXT, eObj));
    eObj->add(new CDraw(CDraw::GUI3));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(ACTIVATE_INPUT_TEXT_BOX));
    eObj->add(new CButtonHitbox(100, 20));
    eObj->add(new CParentPanel(ePanel));
    eObj->get<CInputTextBox>()->content = profile.name;
    eObj->get<CInputTextBox>()->charCount = profile.name.size();

    double spacing = 60;
    int nChars = CChar::Map.size();
    x = cxWindow - double(nChars*spacing)/2.f + spacing/2;
    y = cyWindow - spacing;

    /// CHARACTER
    eObj = eManager->createEntity();
    eChar = eObj;
    eObj->add(new CPosition(cxWindow - 60/2 - 100/2, cyWindow));
    eObj->add(new CDraw(CDraw::GUI3));
    eObj->add(new CDimensions(60, 60));
    eObj->add(new CChar(CChar::Map.begin()->second));
    eObj->add(new CAnimation(false, CChar::Map.begin()->second.aIdle));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger());
    eObj->add(new CButtonHitbox(60, 60));
    eObj->add(new CParentPanel(ePanel));
    eObj->get<CButtonState>()->lostFocusMessage = SWITCH_CHAR_ANIMATION;
    eObj->get<CButtonState>()->gainedFocusMessage = SWITCH_CHAR_ANIMATION;

    /// CHARACTER OPTIONS
    for (map<string, CChar>::iterator i = CChar::Map.begin(); i != CChar::Map.end(); i++){
        CChar c = i->second;
        eObj = eManager->createEntity();
        eObj->add(new CPosition(x, y));
        eObj->add(new CChar(c));
        eObj->add(new CAnimation(false, c.aIdle));
        eObj->add(new CDraw(CDraw::GUI3));
        eObj->add(new CDimensions(60, 60));
        eObj->add(new COwner(eChar));
        eObj->add(new CParentPanel(ePanel));
        eObj->add(new CButtonState());
        eObj->add(new CButtonTrigger(SELECT_CHARACTER));
        eObj->add(new CButtonHitbox(60, 60));
        eObj->get<CButtonState>()->lostFocusMessage = SWITCH_CHAR_ANIMATION;
        eObj->get<CButtonState>()->gainedFocusMessage = SWITCH_CHAR_ANIMATION;
        x += spacing;
    }

    /// BACK
    w = 50;
    h = 50;
    spacing = 10;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow + w/2 + spacing/2, cyWindow + h/2 + 20/2));
    eObj->add(new CTexture("cancel-button.png"));
    eObj->add(new CButtonTextures("cancel-button.png"));
    eObj->add(new CButtonState());
    eObj->add(new CButtonHitbox(w, h));
    eObj->add(new CButtonTrigger(CLOSE_PANEL));
    eObj->add(new CParentPanel(ePanel));
    eObj->add(new CDraw(CDraw::GUI3));

    /// ACCEPT
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow - w/2 - spacing/2, cyWindow + h/2 + 20/2));
    eObj->add(new CTexture("ok-button.png"));
    eObj->add(new CButtonTextures("ok-button.png"));
    eObj->add(new CButtonState());
    eObj->add(new CButtonHitbox(w, h));
    eObj->add(new CButtonTrigger(CREATE_PROFILE));
    eObj->add(new CParentPanel(ePanel));
    eObj->add(new CDraw(CDraw::GUI3));
    eObj->add(new CProfileEditor(eInput, eChar));
    if (e->has(Component::PROFILE)){
        eObj->add(new CProfile(*e->get<CProfile>()));
    }

}

void MainMenuSystem::onCreateProfile(Entity* e){
    saveProfile(e);

    notify(CLOSE_PANEL, e);

    createPlayerSelect();
}

void MainMenuSystem::saveProfile(Entity* e){
    Entity* eName = e->get<CProfileEditor>()->eName;
    Entity* eChar = e->get<CProfileEditor>()->eChar;
    string prvName;
    if (e->has(Component::PROFILE)) prvName = e->get<CProfile>()->name;
    string name = eName->get<CInputTextBox>()->content;

    XMLDocument doc;
    string path = "../rsc-0.1/profiles.xml";
    if (doc.LoadFile(path.c_str()) != XML_NO_ERROR){
        printf("Error!\n");
    }
    XMLNode* node = doc.FirstChildElement("Profile");
    for(node; node != nullptr; node = node->NextSibling()){
        XMLElement* elm = node->ToElement();
        if (string(elm->FirstChildElement("Name")->GetText()) == prvName) break;
    }
    XMLElement* element;
    XMLElement* elChar;
    XMLElement* elName;
    if (node == nullptr){
        element = doc.NewElement(name.c_str());
        elChar = doc.NewElement("Char");
        elName = doc.NewElement("Name");
        doc.InsertEndChild(element);
        element->InsertEndChild(elChar);
        element->InsertEndChild(elName);
    }else{
        element = node->ToElement();
        elChar = element->FirstChildElement("Char");
        elName = element->FirstChildElement("Name");
    }

    element->SetName("Profile");
    elName->SetText(name.c_str());
    elChar->SetText(eChar->get<CChar>()->name.c_str());

    doc.SaveFile(path.c_str());
}

void MainMenuSystem::onSelectCharacter(Entity* e){
    Entity* eChar = e->get<COwner>()->e;
    *eChar->get<CChar>() = *e->get<CChar>();
    *eChar->get<CAnimation>() = CAnimation(false, e->get<CChar>()->aIdle);
    eChar->get<CAnimation>()->sprite = Assets::getAnimation(e->get<CChar>()->aIdle).frames[0];
}

void MainMenuSystem::onCreateProfileMenu(Entity* e){
    notify(REMOVE_NEPHEWS, e);
    notify(REMOVE_CHILDREN, e);

    Entity* eObj;
    double x0 = cxWindow;
    double y0 = cyWindow;
    double x = x0;
    double y = y0;
    double w = 180;
    double h = 40;
    double spacing = h + 10;

    eObj = createButton(Assets::getString("LABEL-SELECT-PROFILE"), w, h, x, y, SELECT_PROFILE);
    eObj->add(new CParentPanel(e));
    eObj->add(new CProfile(*e->get<CProfile>()));
    e->get<CPanel>()->objects.push_back(eObj);
    e->attachEmployee(eObj);

    y += spacing;
    eObj = createButton(Assets::getString("LABEL-EDIT-PROFILE"), w, h, x, y, EDIT_PROFILE);
    eObj->add(new CProfile(*e->get<CProfile>()));
    eObj->add(new CParentPanel(e));
    e->get<CPanel>()->objects.push_back(eObj);
    e->attachEmployee(eObj);
    y += spacing;
    eObj = createButton(Assets::getString("LABEL-DELETE-PROFILE"), w, h, x, y, DELETE_PROFILE);
    eObj->add(new CProfile(*e->get<CProfile>()));
    eObj->add(new CParentPanel(e));    e->get<CPanel>()->objects.push_back(eObj);
    e->attachEmployee(eObj);
}
void MainMenuSystem::onEditProfile(Entity* e){
    notify(CREATE_PLAYER_EDITOR, e);
}
void MainMenuSystem::onDeleteProfile(Entity* e){
    deleteProfile(e);
    createPlayerSelect();
}
void MainMenuSystem::onSelectProfile(Entity* e){
    System::profile = *e->get<CProfile>();
    Entity* ePage = eManager->createEntity();
    ePage->add(new CMainMenuPage(CMainMenuPage::MAIN_SCREEN));
    notify(CHANGE_MENU_PAGE, ePage);
    notify(REMOVE_CHILDREN, e->get<CParentPanel>()->e);
}

void MainMenuSystem::deleteProfile(Entity* e){
    string name = e->get<CProfile>()->name;

    XMLDocument doc;
    string path = "../rsc-0.1/profiles.xml";
    if (doc.LoadFile(path.c_str()) != XML_NO_ERROR){
        printf("Error!\n");
    }
    XMLNode* node = doc.FirstChildElement("Profile");
    for(node; node != nullptr; node = node->NextSibling()){
        XMLElement* elm = node->ToElement();
        if (string(elm->FirstChildElement("Name")->GetText()) == name) break;
    }
    XMLElement* element;
    XMLElement* elChar;
    XMLElement* elName;
    if (node == nullptr){
        element = doc.NewElement(name.c_str());
        elChar = doc.NewElement("Char");
        elName = doc.NewElement("Name");
        doc.InsertEndChild(element);
        element->InsertEndChild(elChar);
        element->InsertEndChild(elName);
    }else{
        element = node->ToElement();
        elChar = element->FirstChildElement("Char");
        elName = element->FirstChildElement("Name");
    }

    doc.DeleteNode(element);
    doc.SaveFile(path.c_str());
}
