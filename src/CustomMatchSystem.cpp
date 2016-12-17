#include "CustomMatchSystem.h"

CustomMatchSystem::CustomMatchSystem(){
    addSubscription(CREATE_SCREEN);
    addSubscription(START_CUSTOM_MATCH);
    onMenu = false;
}

CustomMatchSystem::~CustomMatchSystem(){

}

void CustomMatchSystem::update(){
    if (onMenu){

    }
}

void CustomMatchSystem::onCreateScreen(Entity* e){
    switch(e->get<CScreen>()->id){
        case CScreen::CUSTOM_MATCH: eManager->clearSystem(); createMenu(); notify(NEW_SCREEN); break;
        default: break;
    }
}

void CustomMatchSystem::createMenu(){

    //loadMatchConfig("MostRecent", current);

    onMenu = true;
    Entity* eObj;
    double xAux, yAux;
    double x0, y0;

    double spacing = 100;
    double w = 160;
    double h = 40;

    ///BACKGROUND
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->add(new CTexture("background-desert.png"));
    eObj->add(new CDraw(CDraw::BACKGROUND));

    ///AVAILABLE UNITS
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, 50));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-UNITS"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(100, 20, sf::Color::White, 1));

    w = 50;
    h = 50;
    int nUnits = 17;
    x0 = cxWindow - (double)nUnits*w/2;
    y0 = 50 + h/2 + 20/2 + 8;
    xAux = x0;
    yAux = y0;
    for(int i = 0; i < nUnits; i++){
        eObj = eManager->createEntity();
        eObj->add(new CPosition(xAux, yAux));
        eObj->add(new CUnit(CUnit::Map[(CUnit::ID)i]));
        eObj->add(new CDraw(CDraw::GUI1));
        eObj->add(new CTexture("blank-circle.png"));
        eObj->add(new CCheckBox("blank-circle.png", "unavailable.png", true));
        eObj->add(new CButtonHitbox(w, h));
        eObj->add(new CDimensions(w, h));
        eObj->add(new CButtonState());
        eObj->add(new CButtonTrigger(TOGGLE_CHECK_BOX));
        eObj->add(new CTooltip(CTooltip::UNIT));

        if (!contains(current.unitPool, i)){
            eObj->get<CCheckBox>()->on = false;
            eObj->get<CTexture>()->file = "unavailable.png";
        }

        eUnitPool.push_back(eObj);
        xAux += w;
    }
    ///AVAILABLE HEROES
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, 150));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-HEROES"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(100, 20, sf::Color::White, 1));

    w = 50;
    h = 50;
    int nHeroes = 13;
    x0 = cxWindow - (double)nHeroes*w/2;
    y0 = 150 + h/2 + 20/2 + 8;
    xAux = x0;
    yAux = y0;
    for(int i = 0; i < nHeroes; i++){
        eObj = eManager->createEntity();
        eObj->add(new CPosition(xAux, yAux));
        eObj->add(new CCaptain(CCaptain::Map[(CCaptain::ID)i]));
        eObj->add(new CDraw(CDraw::GUI1));
        eObj->add(new CTexture("blank-circle.png"));
        eObj->add(new CCheckBox("blank-circle.png", "unavailable.png", true));
        eObj->add(new CButtonHitbox(w, h));
        eObj->add(new CDimensions(w, h));
        eObj->add(new CButtonState());
        eObj->add(new CButtonTrigger(TOGGLE_CHECK_BOX));
        eObj->add(new CTooltip(CTooltip::CAPTAIN));

        if (!contains(current.heroPool, i)){
            eObj->get<CCheckBox>()->on = false;
            eObj->get<CTexture>()->file = "unavailable.png";
        }

        eHeroPool.push_back(eObj);
        xAux += w;
    }

    ///PLAYERS
    x0 = 80;
    y0 = 300;
    w = 150;
    h = 20;
    x0 = cxWindow - 3*(w + 10)/2;
    y0 = 300;

    ///LABELS
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 1*(w + 10), y0 - h - 10));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-PLAYER-NAME"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 2*(w + 10), y0 - h - 10));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-INTELLIGENCE"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 3*(w + 10), y0 - h - 10));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-DIFFICULTY"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    ///PLAYER 1
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0, y0));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-PLAYER") + int2str(1), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 1*(w + 10), y0));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));
    //eObj->add(new CInputTextBox(true, CInputTextBox::INACTIVE, 12));
    eObj->add(new CDisplayer(CDisplayer::INPUT_TEXT, eObj));
    eObj->add(new CButtonHitbox(w, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(ACTIVATE_INPUT_TEXT_BOX));
    ePlayer1 = eObj;

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 2*(w + 10), y0));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-INTELLIGENCE-HUMAN"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 3*(w + 10), y0));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-DIFFICULTY-NORMAL"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    ///PLAYER 2
    y0 += h + 10;

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0, y0));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-PLAYER") + int2str(2), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 1*(w + 10), y0));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));
    //eObj->add(new CInputTextBox(true, CInputTextBox::INACTIVE, 12));
    eObj->add(new CDisplayer(CDisplayer::INPUT_TEXT, eObj));
    eObj->add(new CButtonHitbox(w, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(ACTIVATE_INPUT_TEXT_BOX));
    ePlayer2 = eObj;

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 2*(w + 10), y0));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-INTELLIGENCE-HUMAN"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x0 + 3*(w + 10), y0));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-DIFFICULTY-NORMAL"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w, h, sf::Color::White, 1));

    ///MATCH CONFIGURATION
    double w1 = 200;
    double w2 = 50;
    double space = 38;
    double x1 = cxWindow - (w1+space+w2)/2 + w1/2;
    double x2 = cxWindow + (w1+space+w2)/2 - w2/2;
    double dy = 32;
    y0 = 380;

    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, 380));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-MATCH-CONFIGURATION"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w1+w2+space, 20, sf::Color::White, 1));

    ///BROKEN SINCE SPIN_BUTTON CHANGES
    /*
    ///HEROES PER PLAYER
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x1, y0 + 1*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-HEROES-PER-PLAYER"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w1, 20, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x2, y0 + 1*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w2, 20, sf::Color::White, 1));
    eObj->add(new CButtonHitbox(w2, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger());
    eObj->add(new CDisplayer(CDisplayer::SPIN_BUTTON, eObj));
    eObj->add(new CSpinButtonValue(current.nPlayerHeroes, 1, 1, 6));
    eObj->get<CSpinButtonValue>()->style = CSpinButton::STYLE_2;
    notify(CREATE_SPIN_BUTTONS, eObj);
    ePlayerHeroes = eObj;

    ///ARMY SIZE
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x1, y0 + 2*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-ARMY-SIZE"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w1, 20, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x2, y0 + 2*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w2, 20, sf::Color::White, 1));
    eObj->add(new CButtonHitbox(w2, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger());
    eObj->add(new CDisplayer(CDisplayer::SPIN_BUTTON, eObj));
    eObj->add(new CSpinButtonValue(current.armySize, 10, 30, 50));
    eObj->get<CSpinButtonValue>()->style = CSpinButton::STYLE_2;
    notify(CREATE_SPIN_BUTTONS, eObj);
    eArmySize = eObj;

    ///UNIT OPTIONS
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x1, y0 + 3*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-UNIT-OPTIONS"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w1, 20, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x2, y0 + 3*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w2, 20, sf::Color::White, 1));
    eObj->add(new CButtonHitbox(w2, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger());
    eObj->add(new CDisplayer(CDisplayer::SPIN_BUTTON, eObj));
    eObj->add(new CSpinButtonValue(current.nUnitOpt, 1, 1, 17));
    eObj->get<CSpinButtonValue>()->style = CSpinButton::STYLE_2;
    notify(CREATE_SPIN_BUTTONS, eObj);
    eUnitOptions = eObj;

    ///BATTLE DURATION
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x1, y0 + 4*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-BATTLE-DURATION"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w1, 20, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x2, y0 + 4*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2("3", Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w2, 20, sf::Color::White, 1));
    eObj->add(new CButtonHitbox(w2, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger());
    eObj->add(new CDisplayer(CDisplayer::SPIN_BUTTON, eObj));
    eObj->add(new CSpinButtonValue(current.nTurns, 1, 5, 99));
    eObj->get<CSpinButtonValue>()->style = CSpinButton::STYLE_2;
    notify(CREATE_SPIN_BUTTONS, eObj);
    eTurns = eObj;

    ///WAR DURATION
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x1, y0 + 5*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-WAR-DURATION"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w1, 20, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x2, y0 + 5*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2("5", Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w2, 20, sf::Color::White, 1));
    eObj->add(new CButtonHitbox(w2, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger());
    eObj->add(new CDisplayer(CDisplayer::SPIN_BUTTON, eObj));
    eObj->add(new CSpinButtonValue(current.maxBattles, 1, 5, 99));
    eObj->get<CSpinButtonValue>()->style = CSpinButton::STYLE_2;
    notify(CREATE_SPIN_BUTTONS, eObj);
    eBattles = eObj;

    ///STREAK TO WIN
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x1, y0 + 6*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("LABEL-WIN-STREAK-TO-END"), Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w1, 20, sf::Color::White, 1));

    eObj = eManager->createEntity();
    eObj->add(new CPosition(x2, y0 + 6*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2("2", Assets::getFont(Assets::getPrimaryFont()), 16));
    eObj->add(new CRectShape(w2, 20, sf::Color::White, 1));
    eObj->add(new CButtonHitbox(w2, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger());
    eObj->add(new CDisplayer(CDisplayer::SPIN_BUTTON, eObj));
    eObj->add(new CSpinButtonValue(current.streakToWin, 1, 2, 5));
    eObj->get<CSpinButtonValue>()->style = CSpinButton::STYLE_2;
    notify(CREATE_SPIN_BUTTONS, eObj);
    eStreak = eObj;
    */
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, y0 + 8*dy));
    eObj->add(new CDraw());
    eObj->add(new CTextbox2(Assets::getString("START"), Assets::getFont(Assets::getPrimaryFont()), 14));
    eObj->add(new CRectShape(100, 50, sf::Color::White, 1));
    eObj->add(new CButtonHitbox(100, 50));
    eObj->add(new CButtonState());
    eObj->add(new CScreen(CScreen::MATCH, CScreen::FADE_BLACK));
    eObj->add(new CButtonTrigger(START_CUSTOM_MATCH));
}

void CustomMatchSystem::onStartCustomMatch(Entity* e){
    current.armySize = str2int(eArmySize->get<CTextbox2>()->content.getString());
    current.nTurns = str2int(eTurns->get<CTextbox2>()->content.getString());
    current.nUnitOpt = str2int(eUnitOptions->get<CTextbox2>()->content.getString());
    current.streakToWin = str2int(eStreak->get<CTextbox2>()->content.getString());
    current.maxBattles = str2int(eBattles->get<CTextbox2>()->content.getString());
    current.nPlayerHeroes = str2int(ePlayerHeroes->get<CTextbox2>()->content.getString());
    current.heroPool.clear();
    current.unitPool.clear();

    for(EntityListIt i = eUnitPool.begin(); i != eUnitPool.end(); i++){
        Entity* eUnit = *i;
        if (eUnit->get<CCheckBox>()->on){
            current.unitPool.push_back(eUnit->get<CUnit>()->id);
        }
    }

    for(EntityListIt i = eHeroPool.begin(); i != eHeroPool.end(); i++){
        Entity* eHero = *i;
        if (eHero->get<CCheckBox>()->on){
            current.heroPool.push_back(eHero->get<CCaptain>()->id);
        }
    }

    //war.match = current;
    saveConfiguration("MostRecent");

    notify(START_SCREEN_TRANSITION, e);
}

void CustomMatchSystem::saveConfiguration(string name){
    tinyxml2::XMLDocument doc;
    string path = Assets::rscRoot + "match-config.xml";
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR){
        printf("Error!\n");
    }
    tinyxml2::XMLNode* node = doc.FirstChildElement(name.c_str());
    tinyxml2::XMLElement* element;
    if (node == nullptr){
        element = doc.NewElement(name.c_str());
        doc.InsertEndChild(element);
    }else{
        element = node->ToElement();
    }

    element->SetAttribute("Color1", "");
    element->SetAttribute("Color2", "");
    element->SetAttribute("PlayerHeroes", current.nPlayerHeroes);
    element->SetAttribute("MaxBattles", current.maxBattles);
    element->SetAttribute("Turns", current.nTurns);
    element->SetAttribute("ArmySize", current.armySize);
    element->SetAttribute("UnitaryBlockChance", current.uBlockChance);
    element->SetAttribute("OptionsOnRebuild", current.nUnitOpt);
    element->SetAttribute("RecruitGroup", current.recruitGroup);
    element->SetAttribute("StreakToWin", current.streakToWin);

    int nUnits = 17;
    for(int i = 0; i < nUnits; i++){
        string att = "Unit" + int2str(i, 2);
        bool available = contains(current.unitPool, i);
        element->SetAttribute(att.c_str(), available);
    }

    int nHeroes = 13;
    for(int i = 0; i < nHeroes; i++){
        string att = "Hero" + int2str(i, 2);
        bool available = contains(current.heroPool, i);
        element->SetAttribute(att.c_str(), available);
    }

    doc.SaveFile(path.c_str());
}
