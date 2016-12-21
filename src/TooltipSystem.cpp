#include "TooltipSystem.h"

TooltipSystem::TooltipSystem(){
    addRequirement(Component::TOOLTIP);

    addSubscription(BUTTON_GAINED_FOCUS);
    addSubscription(BUTTON_LOST_FOCUS);
    addSubscription(UPDATE_TOOLTIP);
}

TooltipSystem::~TooltipSystem(){
    //dtor
}

void TooltipSystem::update(){
    for (EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        /*
        if  (  (e->get<CButtonState>()->state == CButtonState::HOVERED || e->get<CButtonState>()->state == CButtonState::ACTIVE)
            && !e->get<CTooltip>()->isShowing
            &&  e->get<CTooltip>()->timeOnHover.getElapsedTime().asSeconds() > e->get<CTooltip>()->showAfter
            && !eManager->isDead(e)){
            createTooltip(e);
            e->get<CTooltip>()->isShowing = true;
        }
        */
        if  (  (e->get<CButtonState>()->state == CButtonState::HOVERED || e->get<CButtonState>()->state == CButtonState::ACTIVE)
            && !e->get<CTooltip>()->isShowing
            &&  e->get<CTooltip>()->timeOnHover.getElapsedTime().asSeconds() >= e->get<CTooltip>()->showAfter
            && !eManager->isDead(e)){
            createTooltip(e);
            e->get<CTooltip>()->isShowing = true;
        }
    }
}

void TooltipSystem::onButtonGainedFocus(Entity* e){
    if (e->has(Component::TOOLTIP)){
        e->get<CTooltip>()->timeOnHover.restart();
    }
}

void TooltipSystem::onButtonLostFocus(Entity* e){
    if (e->has(Component::TOOLTIP)){
        if (e->get<CTooltip>()->isShowing){
            Entity* eTip = e->getObservedEntity("Tooltip");

            eManager->removeEntity(eTip);

            e->removeObservedEntity("Tooltip");
            e->get<CTooltip>()->isShowing = false;
        }
    }
}

void TooltipSystem::createTooltip(Entity* e){
    switch(e->get<CTooltip>()->info){
        case CTooltip::SECTOR: createSectorTooltip(e); break;
        case CTooltip::UNIT_GROUP: createUnitGroupTooltip(e); break;
        case CTooltip::UNIT: createUnitTooltip(e); break;
        case CTooltip::ACTION: createActionTooltip(e); break;
        case CTooltip::EFFECT: createEffectTooltip(e); break;
        case CTooltip::CAPTAIN: createCaptainTooltip2(e); break;
        case CTooltip::TEXT_1: createTextTooltip1(e); break;
        default: break;
    }
}

void TooltipSystem::createSectorTooltip(Entity* e){
    Entity* eTip = eManager->createEntity();
    CSector sector = *e->get<CSector>();

    double w = 200;
    double h = 100;

    char text[300];
    sprintf(text, "%s\nStrength: %d\nDescription: ", sector.unit.actionDescription.c_str(), sector.strength);

    sf::Text txt;
    txt.setFont(Assets::getFont(Assets::getPrimaryFont()));
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color::Black);
    txt.setString(text);

    eTip->add(new CTexture("tooltip-01.png"));
    eTip->add(new CDimensions(w, h));
    eTip->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y + 200));
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CTextbox2(txt, 0, 0, CTextbox2::CENTRALIZED));
    eTip->add(new CParentPanel(e));

    //e->get<CTooltip>()->eTooltip = eTip;
}

void TooltipSystem::createUnitGroupTooltip(Entity* e){
    /*
    Entity* eTip = eManager->createEntity();
    CUnitGroup sector = *e->get<CUnitGroup>();

    double w = 200;
    double h = 100;

    char text[300];
    sprintf(text, "%s\nStrength: %d\nDescription: ", sector.unit.actionDescription.c_str(), sector.count);

    sf::Text txt;
    txt.setFont(Assets::getFont(Assets::getPrimaryFont()));
    txt.setCharacterSize(14);
    txt.setColor(sf::Color::Black);
    txt.setString(text);

    eTip->add(new CTexture("tooltip-01.png"));
    eTip->add(new CDimensions(w, h));
    eTip->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y + 200));
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CTextbox2(txt, 0, 0, CTextbox2::CENTRALIZED));
    eTip->add(new CParentPanel(e));

    e->get<CTooltip>()->eTooltip = eTip;
    */
}

void TooltipSystem::createUnitTooltip(Entity* e){
    Entity* eTip = eManager->createEntity();
    Entity* eObj;
    CUnit cunit = CUnit::Map[e->get<CUnit>()->id];

    double xAux, yAux;
    double wAux = 0;
    double hAux = 0;
    double w = 150;
    double h = 166;
    double yOff = 54;
    if (e->has(Component::SPIN_BUTTON_VALUE)) yOff = 60;

    double x = e->get<CPosition>()->x;
    double y = e->get<CPosition>()->y + h/2 + yOff;

    eTip->add(new CPanel());
    eTip->add(new CTexture("9p-scroll.png"));
    eTip->add(new CDimensions(w, h));
    eTip->add(new CPosition(x, y));
    eTip->add(new CDraw(CDraw::GUI4));
    ///==================
    /// NAME
    ///==================
    eObj = eManager->createEntity();
    xAux = x;
    yAux = y - h/2 + 26;
    eObj->add(new CPosition(xAux - w/2 + 30 + 10, yAux - 12/2 + 10));
    eObj->add(new CDraw(CDraw::GUI5));
    eObj->add(new CTextbox2(Assets::getString(cunit.strName), Assets::getFont(Assets::getPrimaryFont()), 12, sf::Color::Black, 0, 0, CTextbox2::NORMAL));

    eTip->get<CPanel>()->add(eObj);
    eTip->attachEmployee(eObj);
    wAux += eObj->get<CTextbox2>()->content.getLocalBounds().width;
    ///==================
    /// DAMAGE TYPE ICON
    ///==================
    wAux += 30 + 20;
    w = wAux + 10;
    xAux = x - w/2 + 30/2 + 10;
    y = y - h/2 + 30/2 + 20;
    if (cunit.action != -1){
        Entity* eDamIcon = eManager->createEntity();
        eDamIcon->add(new CDraw(CDraw::GUI5));
        eDamIcon->add(new CPosition(xAux, y));
        eDamIcon->add(new CTexture("type-" + int2str(e->get<CUnit>()->damage,2) + "-icon-01.png"));
        eDamIcon->add(new CDimensions(30, 30));
        eTip->get<CPanel>()->objects.push_back(eDamIcon);
        eTip->attachEmployee(eDamIcon);
    }

    /// !!! FIT !!!
    eObj->get<CPosition>()->x = xAux + 30/2 + 5;

    ///==================
    /// RESISTANCE BARS
    ///==================
    y += 27;
    vector<Entity*> bars(CUnit::N_DAMAGE_TYPES);
    vector<Entity*> resIcons(CUnit::N_DAMAGE_TYPES);

    double wBarFull = 10 + 30 + 15/2 + 3*15 + 10;
    double hBar = 10;
    double wBarUnit = wBarFull/3;
    xAux = x - wBarFull/2 + 30/2 + 10;

    for(int i = 0; i < CUnit::N_DAMAGE_TYPES; i++){
        /// ICON
        resIcons[i] = eManager->createEntity();
        resIcons[i]->add(new CDraw(CDraw::GUI5));
        resIcons[i]->add(new CPosition(xAux, y));
        resIcons[i]->add(new CTexture("shield-" + int2str(i, 2) + ".png"));
        resIcons[i]->add(new CDimensions(30, 30));
        /// BAR
        int res = CUnit::Map[e->get<CUnit>()->id].resistance[i];
        double xUnit = xAux + 30/2 + 15/2;
        for (int j = 0; j < res; j++){
            Entity* eUnit = eManager->createEntity();
            eUnit->add(new CPosition(xUnit, y));
            eUnit->add(new CTexture("type-" + int2str(i, 2) + "-gem.png"));
            eUnit->add(new CDraw(CDraw::GUI5));
            eUnit->add(new CDimensions(15, 15));
            eTip->get<CPanel>()->objects.push_back(eUnit);
            eTip->attachEmployee(eUnit);
            xUnit += 15;
        }
        eTip->get<CPanel>()->objects.push_back(resIcons[i]);
        eTip->attachEmployee(resIcons[i]);
        y += 23;
    }

    e->addObservedEntity("Tooltip", eTip);

    eTip->get<CDimensions>()->width = w;
}

void TooltipSystem::createActionTooltip(Entity* e){
    CAction act = CAction::Map[e->get<CAction>()->id];

    if (act.id >= 0 && act.id <= 99){
        createUnitActionTooltip(e);
    }else if (act.id >= 100 && act.id <= 199){
        createCaptainTooltip(e);
    }else if (act.id >= 200 && act.id <= 299){
        createCaptainActionTooltip(e);
    }else if (act.id >= 300 && act.id <= 399){
        createFormationTooltip(e);
    }else if (act.id >= 400 && act.id <= 499){
        createBattleClosureTooltip(e);
    }
}

void TooltipSystem::createUnitActionTooltip(Entity* e){
    Entity* eTip = eManager->createEntity();
    CAction act = CAction::Map[e->get<CAction>()->id];

    double w = 120;
    double h = 20;
    double xOff = 30;

    char text[300];
    sprintf(text, "TYPE: %s", act.damage.c_str());

    sf::Text txt;
    txt.setFont(Assets::getFont(Assets::getPrimaryFont()));
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color::Black);
    txt.setString(text);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(w,h));
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(1);

    eTip->add(new CPanel());
    eTip->add(new CRectShape(rect));
    eTip->add(new CPosition(e->get<CPosition>()->x + w/2 + xOff, e->get<CPosition>()->y));
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CTextbox2(txt, -w/2, -h/2, CTextbox2::NORMAL));
    eTip->add(new CParentPanel(e));

    e->addObservedEntity("Tooltip", eTip);
    e->attachEmployee(eTip);
}

void TooltipSystem::createCaptainActionTooltip(Entity* e){
    Entity* eTip = eManager->createEntity();
    CAction::ID id = e->get<CAction>()->id;

    double xOff = 30;

    string str;
    string name = toUpper(Assets::getString(CAction::Map[id].strName));
    str += name + ": " + Assets::getString(CAction::Map[id].strDescription);

    eTip->add(new CPanel());
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CTextbox2(str, Assets::getFont(Assets::getPrimaryFont()), 10, sf::Color::Black, 0, 0, CTextbox2::NORMAL));
    double w = eTip->get<CTextbox2>()->content.getLocalBounds().width + 2;
    double h = eTip->get<CTextbox2>()->content.getLocalBounds().height + 2;
    eTip->get<CTextbox2>()->xOff = -w/2;
    eTip->get<CTextbox2>()->yOff = -h/2;
    eTip->add(new CRectShape(w, h, sf::Color::White, 1, sf::Color::Black));
    eTip->add(new CPosition(e->get<CPosition>()->x + w/2 + xOff, e->get<CPosition>()->y));
    eTip->add(new CParentPanel(e));

    e->addObservedEntity("Tooltip", eTip);
    e->attachEmployee(eTip);
}

void TooltipSystem::createFormationTooltip(Entity* e){
    Entity* eTip = eManager->createEntity();
    CAction act = CAction::Map[e->get<CAction>()->id];

    double xOff = 30;
    double marginWidth = 10;

    string text = toUpper(act.name) + ": " + act.description;
    
    eTip->add(new CTextbox2(text, Assets::getFont(Assets::getPrimaryFont()), 12, sf::Color::Black, 0, 0, CTextbox2::NORMAL));
    double w = eTip->get<CTextbox2>()->content.getLocalBounds().width + 2*marginWidth;
    double h = eTip->get<CTextbox2>()->content.getLocalBounds().height + 2*marginWidth;
    eTip->get<CTextbox2>()->xOff = -w/2 + marginWidth;
    eTip->get<CTextbox2>()->yOff = -h/2 + marginWidth;
    eTip->add(new CPanel());
    //eTip->add(new CRectShape(w, h, sf::Color::White, 1, sf::Color::Black));
    eTip->add(new CTexture("9p-tooltip-bg-03.png"));
    eTip->add(new CDimensions(w, h));
    eTip->add(new CPosition(e->get<CPosition>()->x + w/2 + xOff, e->get<CPosition>()->y));
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CParentPanel(e));

    e->addObservedEntity("Tooltip", eTip);
    e->attachEmployee(eTip);
}

void TooltipSystem::createBattleClosureTooltip(Entity* e){
    Entity* eTip = eManager->createEntity();
    CAction act = CAction::Map[e->get<CAction>()->id];

    double xOff = 30;
    double marginWidth = 10;

    string text = toUpper(act.name) + ": " + act.description;
    
    eTip->add(new CTextbox2(text, Assets::getFont(Assets::getPrimaryFont()), 12, sf::Color::Black, 0, 0, CTextbox2::NORMAL));
    double w = eTip->get<CTextbox2>()->content.getLocalBounds().width + 2*marginWidth;
    double h = eTip->get<CTextbox2>()->content.getLocalBounds().height + 2*marginWidth;
    eTip->get<CTextbox2>()->xOff = -w/2 + marginWidth;
    eTip->get<CTextbox2>()->yOff = -h/2 + marginWidth;
    eTip->add(new CPanel());
    //eTip->add(new CRectShape(w, h, sf::Color::White, 1, sf::Color::Black));
    eTip->add(new CTexture("9p-tooltip-bg-03.png"));
    eTip->add(new CDimensions(w, h));
    eTip->add(new CPosition(e->get<CPosition>()->x + w/2 + xOff, e->get<CPosition>()->y));
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CParentPanel(e));

    e->addObservedEntity("Tooltip", eTip);
    e->attachEmployee(eTip);
}

void TooltipSystem::createEffectTooltip(Entity* e){    
    Entity* eTip = eManager->createEntity();
    CAction act = CAction::Map[e->get<CAction>()->id];

    double yOff = 30;
    double marginWidth = 10;

    string text = toUpper(act.name) + ": " + act.description;
    
    eTip->add(new CTextbox2(text, Assets::getFont(Assets::getPrimaryFont()), 11, sf::Color::Black, 0, 0, CTextbox2::NORMAL));
    double w = eTip->get<CTextbox2>()->content.getLocalBounds().width + 2*marginWidth;
    double h = eTip->get<CTextbox2>()->content.getLocalBounds().height + 2*marginWidth;
    eTip->get<CTextbox2>()->xOff = -w/2 + marginWidth;
    eTip->get<CTextbox2>()->yOff = -h/2 + marginWidth;
    eTip->add(new CPanel());
    //eTip->add(new CRectShape(w, h, sf::Color::White, 1, sf::Color::Black));
    eTip->add(new CTexture("9p-tooltip-bg-03.png"));
    eTip->add(new CDimensions(w, h));
    eTip->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y + h/2.f + yOff));
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CParentPanel(e));

    e->addObservedEntity("Tooltip", eTip);
    e->attachEmployee(eTip);
}

void TooltipSystem::createCaptainTooltip(Entity* e){
    Entity* eTip = eManager->createEntity();
    CCaptain cap = CCaptain::Map[e->get<CCaptain>()->id];

    double xOff = 30;

    string str;
    bool firstOne = true;
    for(list<CAction::ID>::iterator i = cap.actions.begin(); i != cap.actions.end(); i++){
        if (!firstOne) str += "\n\n";
        string strName = CAction::Map[*i].strName;
        string strDesc = CAction::Map[*i].strDescription;
        string name = toUpper(Assets::getString(strName));
        str += name + ": " + Assets::getString(strDesc);
        firstOne = false;
    }

    eTip->add(new CPanel());
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CTextbox2(str, Assets::getFont(Assets::getPrimaryFont()), 12, sf::Color::Black, 0, 0, CTextbox2::NORMAL));
    double w = eTip->get<CTextbox2>()->content.getLocalBounds().width + 2;
    double h = eTip->get<CTextbox2>()->content.getLocalBounds().height + 2;
    eTip->get<CTextbox2>()->xOff = -w/2;
    eTip->get<CTextbox2>()->yOff = -h/2;
    eTip->add(new CRectShape(w, h, sf::Color::White, 1, sf::Color::Black));
    eTip->add(new CPosition(e->get<CPosition>()->x + w/2 + xOff, e->get<CPosition>()->y));
    eTip->add(new CParentPanel(e));

    e->addObservedEntity("Tooltip", eTip);
    e->attachEmployee(eTip);
}

void TooltipSystem::createCaptainTooltip2(Entity* e){
    Entity* eTip = eManager->createEntity();
    CCaptain cap = *e->get<CCaptain>();

    double xOff = 30;
    string str;
    string capName = Assets::getString(e->get<CCaptain>()->strName) + "\n\n";
    str += toUpper(capName);

    bool firstOne = true;
    for(list<CAction::ID>::iterator i = cap.actions.begin(); i != cap.actions.end(); i++){
        if (!firstOne) str += "\n\n";
        string strName = CAction::Map[*i].strName;
        string strDesc = CAction::Map[*i].strDescription;
        string name = toUpper(Assets::getString(strName));
        str += name + ": " + Assets::getString(strDesc);
        firstOne = false;
    }

    eTip->add(new CPanel());
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CTextbox2(str, Assets::getFont(Assets::getPrimaryFont()), 12, sf::Color::Black, 0, 0, CTextbox2::NORMAL));
    double w = eTip->get<CTextbox2>()->content.getLocalBounds().width + 2;
    double h = eTip->get<CTextbox2>()->content.getLocalBounds().height + 2;
    eTip->get<CTextbox2>()->xOff = -w/2;
    eTip->get<CTextbox2>()->yOff = -h/2;
    eTip->add(new CRectShape(w, h, sf::Color::White, 1, sf::Color::Black));
    eTip->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y + xOff + h/2));
    eTip->add(new CParentPanel(e));

    e->addObservedEntity("Tooltip", eTip);
    e->attachEmployee(eTip);
}

void TooltipSystem::createTextTooltip1(Entity* e){
    Entity* eTip = eManager->createEntity();

    double yOff = 30;

    string text = e->get<CTooltip>()->text;

    eTip->add(new CTextbox2(text, Assets::getFont(Assets::getPrimaryFont()), 12, sf::Color::Black, 0, 0, CTextbox2::NORMAL));
    double w = eTip->get<CTextbox2>()->content.getLocalBounds().width + 2;
    double h = eTip->get<CTextbox2>()->content.getLocalBounds().height + 2;
    eTip->get<CTextbox2>()->xOff = -w/2;
    eTip->get<CTextbox2>()->yOff = -h/2;
    eTip->add(new CPanel());
    eTip->add(new CRectShape(w, h, sf::Color::White, 1, sf::Color::Black));
    eTip->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y + h/2 + yOff));
    eTip->add(new CDraw(CDraw::GUI2));
    eTip->add(new CParentPanel(e));

    e->addObservedEntity("Tooltip", eTip);
    e->attachEmployee(eTip);
}

void TooltipSystem::onUpdateTooltip(Entity* e){
    if (e->has(Component::TOOLTIP)){
        if (e->get<CTooltip>()->isShowing){
            Entity* eTip = e->getObservedEntity("Tooltip");

            eManager->removeEntity(eTip);

            e->removeObservedEntity("Tooltip");
            e->get<CTooltip>()->isShowing = false;
            e->get<CTooltip>()->timeOnHover.restart();
        }
    }
}





