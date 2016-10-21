#include "ArmyHUDSystem.h"

ArmyHUDSystem::ArmyHUDSystem(){
    addRequirement(Component::ARMY_HUD);

    addSubscription(HIGHLIGHT_UNITS);
    addSubscription(HIGHLIGHT_UNITS_OFF);
    addSubscription(SCORE_UPDATED);
}

ArmyHUDSystem::~ArmyHUDSystem(){
    //dtor
}

void ArmyHUDSystem::update(){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        updateNAlive(e);
        updateUnits(e);
        updateEffects(e);
        updateCaptain(e);
        updateCoin(e);
        //updateResistBars(e);
        //updateScore(e);
        //updateResistBars(e);
    }
    if (!entities.empty() && eResists.empty()){
        createResistanceHighlighters(entities.front());
    }
}

void ArmyHUDSystem::createResistanceHighlighters(Entity* e){
    double x = cxWindow - 30/2;
    double y = 70 - 30/2;

    for(int i = 0; i < CUnit::N_DAMAGE_TYPES; i+=2){
        Entity* eIcon = eManager->createEntity();
        eIcon->add(new CPosition(x, y));
        eIcon->add(new CDraw(CDraw::GUI1));
        eIcon->add(new CTexture("type-" + int2str(i,2) + "-icon-02.png"));
        eIcon->add(new CDimensions(30, 30));
        eIcon->add(new CButtonState());
        eIcon->add(new CButtonHitbox(30,30));
        eIcon->add(new CButtonTrigger());
        eIcon->add(new CHighlightTrigger(CUnitHighlight2::RESISTANCE, i));
        eIcon->add(new COwner(e));
        eIcon->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
        eIcon->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;
        eResists.push_back(eIcon);
        y += 30;
    }

    x = cxWindow + 30/2;
    y = 70 - 30/2;

    for(int i = 1; i < CUnit::N_DAMAGE_TYPES; i+=2){
        Entity* eIcon = eManager->createEntity();
        eIcon->add(new CPosition(x,y));
        eIcon->add(new CDraw(CDraw::GUI1));
        eIcon->add(new CTexture("type-" + int2str(i,2) + "-icon-02.png"));
        eIcon->add(new CDimensions(30, 30));
        eIcon->add(new CButtonState());
        eIcon->add(new CButtonHitbox(30,30));
        eIcon->add(new CButtonTrigger());
        eIcon->add(new CHighlightTrigger(CUnitHighlight2::RESISTANCE, i));
        eIcon->add(new COwner(e));
        eIcon->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
        eIcon->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;
        eResists.push_back(eIcon);
        y += 30;
    }
}

void ArmyHUDSystem::onScoreUpdated(Entity* e){
    double sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double xOff = 70;
    double yOff = 130;
    double x = cxWindow + sign*xOff;
    double y = yOff;
    double w = 30;
    double h = 40;

    string color = e->get<CArmy>()->side == CArmy::LEFT ? "blue":"red";

    if (e->get<CArmyHUD>()->medals.size() < e->get<CArmy>()->score){
        Entity* eObj = eManager->createEntity();
        eObj->add(new CAnimation(false, "medal-" + color + "-idle.png"));
        eObj->add(new CDimensions(30, 40));
        eObj->add(new CActor());
        eObj->add(new CDraw(CDraw::GUI1));
        eObj->add(new CVelocity());
        eObj->add(new CPosition(cxWindow, 150));
        e->get<CArmyHUD>()->medals.push_back(eObj);

        //OLD MEDALS
        double xMed = x + sign*w/2, yMed = y;
        for(int i = 0; i < e->get<CArmyHUD>()->medals.size()-1; i++){
            Entity* eMed = e->get<CArmyHUD>()->medals[i];
            xMed = eMed->get<CPosition>()->x;
            yMed = eMed->get<CPosition>()->y;
            eMed->get<CActor>()->timeline.push_back(new AMove(0.0, xMed + sign*w/2, yMed, 50));
        }
        //NEW MEDAL
        eObj->get<CActor>()->timeline.push_back(new AMove(1.0, xMed - sign*w/2, yMed, 50));

    }else if (e->get<CArmyHUD>()->medals.size() > e->get<CArmy>()->score){
        for(int i = 0; i < e->get<CArmyHUD>()->medals.size(); i++){
            eManager->removeEntity(e->get<CArmyHUD>()->medals[i]);
        }
        e->get<CArmyHUD>()->medals.clear();
    }
}

void ArmyHUDSystem::updateNAlive(Entity* e){
    double sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double w = 30;
    double h = 30;
    double xOff = 70;
    double yOff = 70;
    double x = cxWindow + sign*(xOff);
    double y = yOff;

    if (e->get<CArmyHUD>()->eNAlive == nullptr){
        Entity* eObj = eManager->createEntity();
        eObj->add(new CTexture("alive-counter.png", sign == -1 ? false:true));
        //eObj->add(new CDimensions(w, h));
        eObj->add(new CDraw(CDraw::GUI));
        eObj->add(new CPosition(x, y));
        eObj->add(new CDisplayer(CDisplayer::ALIVE_COUNT, e));
        eObj->add(new CTextbox2("", Assets::getFont(Assets::secondaryFont), 15));
        eObj->add(new CActor());
        eObj->add(new CVelocity());
        e->get<CArmyHUD>()->eNAlive = eObj;
    }
    if (e->get<CArmyHUD>()->eNAlive
    &&  e->get<CArmy>()->nAlive > 0
    &&  e->get<CArmyHUD>()->eNAlive->get<CActor>()->timeline.empty()
    &&  e->get<CArmyHUD>()->eNAlive->get<CPosition>()->x == x){

        animateAliveCounterIn(e);
    }else if(e->get<CArmyHUD>()->eNAlive
         &&  e->get<CArmy>()->nAlive == 0
         &&  e->get<CArmyHUD>()->eNAlive->get<CActor>()->timeline.empty()
         &&  e->get<CArmyHUD>()->eNAlive->get<CPosition>()->x != x){

        animateAliveCounterOut(e);
    }
}

void ArmyHUDSystem::animateAliveCounterIn(Entity* e){
    double sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double w = 30;
    double h = 30;
    double xOff = 70;
    double yOff = 70;
    double xTarget = cxWindow + sign*(xOff+60/2+(w-5)/2-2);
    double speed = 40;
    double tAux;

    Entity* eCounter = e->get<CArmyHUD>()->eNAlive;
    double xCurrent = eCounter->get<CPosition>()->x;
    double yCurrent = eCounter->get<CPosition>()->y;

    eCounter->get<CActor>()->timeline.push_back(new AMove(0.0, xTarget, yCurrent, speed));
}

void ArmyHUDSystem::animateAliveCounterOut(Entity* e){
    double sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double w = 30;
    double h = 30;
    double xOff = 70;
    double yOff = 70;
    double xTarget = cxWindow + sign*(xOff);
    double speed = 40;
    double tAux;

    Entity* eCounter = e->get<CArmyHUD>()->eNAlive;
    double xCurrent = eCounter->get<CPosition>()->x;
    double yCurrent = eCounter->get<CPosition>()->y;

    eCounter->get<CActor>()->timeline.push_back(new AMove(0.0, xTarget, yCurrent, speed));
}

void ArmyHUDSystem::updateResistBars(Entity* e){

    if (e->get<CArmyHUD>()->resistBars.empty()){
        int sign = e->get<CArmy>()->side == CPlayer::LEFT ? -1:1;
        double wBarFull = 60;
        double hBar = 10;
        double wBarUnit = wBarFull/3;
        CBarDisplayer::GrowDirection grow = sign == -1 ? CBarDisplayer::LEFT : CBarDisplayer::RIGHT;
        double x = cxWindow;
        double y = 45;

        e->get<CArmyHUD>()->resistBars.resize(CUnit::N_DAMAGE_TYPES, nullptr);
        for(int i = 0; i < CUnit::N_DAMAGE_TYPES; i++){
            Entity* eBar = eManager->createEntity();
            eBar->add(new CPosition(x,y));
            eBar->add(new CBarDisplayer((CBarDisplayer::ValueType)i, e, grow, wBarUnit, 10));
            eBar->add(new CRectShape(wBarFull, hBar, Assets::scale[e->get<CPlayer>()->color][2], 1));
            eBar->add(new CDraw(CDraw::GUI));
            y += 19;
            e->get<CArmyHUD>()->resistBars[i] = eBar;
        }
    }

    e->get<CAverageUnit>()->resistance[CUnit::FIRE] = ceil(getAverageResistance(e, CUnit::FIRE)-0.5);
    e->get<CAverageUnit>()->resistance[CUnit::WATER] = ceil(getAverageResistance(e, CUnit::WATER)-0.5);
    e->get<CAverageUnit>()->resistance[CUnit::EARTH] = ceil(getAverageResistance(e, CUnit::EARTH)-0.5);
    e->get<CAverageUnit>()->resistance[CUnit::AIR] = ceil(getAverageResistance(e, CUnit::AIR)-0.5);
}

void ArmyHUDSystem::updateCaptain(Entity* e){
    int sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double xOff = 70;
    double yOff = 70;
    double x = cxWindow + sign*xOff;
    double y = yOff;
    bool hFlip = sign == -1? false:true;

    Entity* eCap = e->get<CArmy>()->captain;

    if (eCap != nullptr){
        if (e->get<CArmyHUD>()->captain == nullptr){
            Entity* eIcon = eManager->createEntity();
            eIcon->add(new CPosition(x, y));
            eIcon->add(new CTexture(eCap->get<CCaptain>()->portrait, hFlip));
            eIcon->add(new CDraw(CDraw::GUI1));
            eIcon->add(new CCaptain(CCaptain::Map[eCap->get<CCaptain>()->id]));
            //eIcon->add(new CDimensions(60, 60));
            eIcon->add(new CButtonHitbox(60, 60));
            eIcon->add(new CButtonTrigger());
            eIcon->add(new CButtonState());
            eIcon->add(new CTooltip(CTooltip::CAPTAIN));
            e->get<CArmyHUD>()->captain = eIcon;
        }else{
            Entity* eIcon = e->get<CArmyHUD>()->captain;
            if (eIcon->get<CCaptain>()->id != eCap->get<CCaptain>()->id){
                ///PUFF
                Entity* eObj = eManager->createEntity();

                string puffAnimation = "poof-06.png";
                string sfxPoofIn = "sfx-poof-04.wav";
                string sfxPoofOut = "sfx-poof-05.wav";
                double puffDuration = Assets::getAnimation(puffAnimation).duration;
                eObj = eManager->createEntity();
                eObj->add(new CPosition(eIcon->get<CPosition>()->x, eIcon->get<CPosition>()->y));
                eObj->add(new CDraw(CDraw::GUI2));
                eObj->add(new CDimensions(90, 90));
                eObj->add(new CAnimation(false, puffAnimation));
                eObj->add(new CActor());

                eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
                eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
                eObj->get<CActor>()->addNode(new ADestroy(puffDuration));
            }
            eIcon->get<CTexture>()->file = eCap->get<CCaptain>()->portrait;
            *eIcon->get<CCaptain>() = CCaptain::Map[eCap->get<CCaptain>()->id];
            if (eIcon->get<CButtonState>()->state == CButtonState::LOCKED){
                eIcon->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
            }
        }
    }else{
        if (e->get<CArmyHUD>()->captain == nullptr){
            Entity* eIcon = eManager->createEntity();
            eIcon->add(new CPosition(x, y));
            eIcon->add(new CTexture("hero-portrait-empty.png", hFlip));
            eIcon->add(new CDraw(CDraw::GUI1));
            eIcon->add(new CCaptain());
            //eIcon->add(new CDimensions(60, 60));
            eIcon->add(new CButtonHitbox(60, 60));
            eIcon->add(new CButtonTrigger());
            eIcon->add(new CButtonState());
            eIcon->add(new CTooltip(CTooltip::CAPTAIN));
            e->get<CArmyHUD>()->captain = eIcon;
            eIcon->get<CButtonState>()->state = CButtonState::LOCKED;

            ///PUFF
            Entity* eObj = eManager->createEntity();

            string puffAnimation = "poof-06.png";
            string sfxPoofIn = "sfx-poof-04.wav";
            string sfxPoofOut = "sfx-poof-05.wav";
            double puffDuration = Assets::getAnimation(puffAnimation).duration;
            eObj = eManager->createEntity();
            eObj->add(new CPosition(eIcon->get<CPosition>()->x, eIcon->get<CPosition>()->y));
            eObj->add(new CDraw(CDraw::GUI2));
            eObj->add(new CDimensions(90, 90));
            eObj->add(new CAnimation(false, puffAnimation));
            eObj->add(new CActor());

            eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
            eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
            eObj->get<CActor>()->addNode(new ADestroy(puffDuration));
        }else{
            Entity* eIcon = e->get<CArmyHUD>()->captain;
            if (eIcon->get<CTexture>()->file != "hero-portrait-empty.png"){
                ///PUFF
                Entity* eObj = eManager->createEntity();

                string puffAnimation = "poof-06.png";
                string sfxPoofIn = "sfx-poof-04.wav";
                string sfxPoofOut = "sfx-poof-05.wav";
                double puffDuration = Assets::getAnimation(puffAnimation).duration;
                eObj = eManager->createEntity();
                eObj->add(new CPosition(eIcon->get<CPosition>()->x, eIcon->get<CPosition>()->y));
                eObj->add(new CDraw(CDraw::GUI2));
                eObj->add(new CDimensions(90, 90));
                eObj->add(new CAnimation(false, puffAnimation));
                eObj->add(new CActor());

                eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
                eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
                eObj->get<CActor>()->addNode(new ADestroy(puffDuration));
            }
            eIcon->get<CTexture>()->file = "hero-portrait-empty.png";
            eIcon->get<CButtonState>()->state = CButtonState::LOCKED;
        }
    }
}
/*
void ArmyHUDSystem::updateUnits(Entity* e){
    int sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double xOff = 160;
    double yOff = 70;
    double dx = 50;
    double x = cxWindow + sign*xOff;
    double y = yOff;
    bool hFlip = sign == -1? false:true;

    list<CUnit::ID> uniqueUnits;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++){
        Entity* eUnit = *i;
        if (eManager->isDead(eUnit)) continue;
        if (eUnit->get<CUnit>()->dead) continue;
        if (!contains(uniqueUnits, eUnit->get<CUnit>()->id)){
            uniqueUnits.push_back(eUnit->get<CUnit>()->id);
        }
    }

    list< map<CUnit::ID, Entity*>::iterator > toErase;
    for(map<CUnit::ID, Entity*>::iterator i = e->get<CArmyHUD>()->units.begin(); i != e->get<CArmyHUD>()->units.end(); i++){
        if (!contains(uniqueUnits, i->first)){
            toErase.push_back(i);
        }
    }

    list< map<CUnit::ID, Entity*>::iterator >::iterator iter;
    for(iter = toErase.begin(); iter != toErase.end(); iter++){
        eManager->removeEntity((*iter)->second);
        e->get<CArmyHUD>()->units.erase(*iter);
    }

    for(list<CUnit::ID>::iterator i = uniqueUnits.begin(); i != uniqueUnits.end(); i++){
        map<CUnit::ID, Entity*>::iterator it;
        it = e->get<CArmyHUD>()->units.find(*i);
        if (it == e->get<CArmyHUD>()->units.end()){
            CUnit::ID id = *i;

            Entity* eDisplayer = eManager->createEntity();
            eDisplayer->add(new CUnit(CUnit::Map[id]));
            eDisplayer->add(new CPosition(x, y));
            eDisplayer->add(new CTexture(CUnit::Map[id].displayer, hFlip));
            eDisplayer->add(new CDraw(CDraw::HUD2));
            eDisplayer->add(new CButtonState());
            eDisplayer->add(new CButtonHitbox(Assets::getTexture(CUnit::Map[id].displayer)->getSize().x, Assets::getTexture(CUnit::Map[id].displayer)->getSize().y));
            eDisplayer->add(new CButtonTrigger());
            eDisplayer->add(new CDisplayer(CDisplayer::UNIT_COUNT, eDisplayer));
            eDisplayer->add(new CTooltip(CTooltip::UNIT));
            eDisplayer->add(new COwner(e));
            eDisplayer->add(new CHighlightTrigger(CUnitHighlight2::UNIT, id));
            eDisplayer->add(new CDisplayer(CDisplayer::UNIT_COUNT, eDisplayer));
            eDisplayer->add(new CTextbox2("", Assets::getFont(Assets::secondaryFont), 15, sf::Color::Black, 0, 16));
            eDisplayer->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
            eDisplayer->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;

            e->get<CArmyHUD>()->units.insert(make_pair(id, eDisplayer));
            x += sign*dx;
        }
    }

    x = cxWindow + sign*xOff;
    y = yOff;
    for(map<CUnit::ID, Entity*>::iterator i = e->get<CArmyHUD>()->units.begin(); i != e->get<CArmyHUD>()->units.end(); i++){
        Entity* eDisp = i->second;
        eDisp->get<CPosition>()->x = x;
        eDisp->get<CPosition>()->y = y;
        x += sign*dx;
    }
}
*/
void ArmyHUDSystem::animateUnitOut(IconDisplayerPair& p){
    double yTarget = -50;
    double spacing = 30;
    double speed = 150;
    double speed2 = 40;
    double tAux;

    double xCurrent = p.eDisplayer->get<CPosition>()->x;
    double yCurrent = p.eDisplayer->get<CPosition>()->y;

    p.eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yCurrent + spacing/2, speed2));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yCurrent + spacing/2, speed2);
    p.eIcon->get<CActor>()->timeline.push_back(new AMove(tAux, xCurrent, yTarget, speed));
    tAux = getTravelTime(xCurrent, yCurrent + spacing/2, xCurrent, yTarget, speed);
    p.eIcon->get<CActor>()->timeline.push_back(new ADestroy(tAux));

    p.eDisplayer->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yCurrent - spacing/2, speed2));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yCurrent - spacing/2, speed2);
    p.eDisplayer->get<CActor>()->timeline.push_back(new AMove(tAux, xCurrent, yTarget, speed));
    tAux = getTravelTime(xCurrent, yCurrent - spacing/2, xCurrent, yTarget, speed);
    p.eDisplayer->get<CActor>()->timeline.push_back(new ADestroy(tAux));
}
void ArmyHUDSystem::animateUnitIn(IconDisplayerPair& p){
    double yTarget = 70;
    double spacing = 30;
    double speed = 150;
    double speed2 = 40;
    double tAux;

    double xCurrent = p.eDisplayer->get<CPosition>()->x;
    double yCurrent = p.eDisplayer->get<CPosition>()->y;

    p.eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed);
    p.eIcon->get<CActor>()->timeline.push_back(new AMove(tAux, xCurrent, yTarget - spacing/2, speed2));

    p.eDisplayer->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed);
    p.eDisplayer->get<CActor>()->timeline.push_back(new AMove(tAux, xCurrent, yTarget + spacing/2, speed2));
}

void ArmyHUDSystem::updateUnits(Entity* e){
    int sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double xOff = 160;
    double yOff = 70;
    double dx = 50;
    double x = cxWindow + sign*xOff;
    double y = yOff;
    bool hFlip = sign == -1? false:true;
    bool rearrange = false;

    list<CUnit::ID> uniqueUnits;
    for(auto& i : e->get<CArmy>()->unitCount){
        if (i.second > 0){
            uniqueUnits.push_back(i.first);
        }
    }

    ///REMOVE DISPLAYERS THAT ARE NO LONGER NEEDED
    map<CUnit::ID, IconDisplayerPair> newList;
    for(map<CUnit::ID, IconDisplayerPair>::iterator i = e->get<CArmyHUD>()->units2.begin(); i != e->get<CArmyHUD>()->units2.end(); i++){
        if (!contains(uniqueUnits, i->first)){
            //eManager->removeEntity(i->second.eIcon);
            //eManager->removeEntity(i->second.eDisplayer);
            animateUnitOut(i->second);
            rearrange = true;
        }else{
            newList.insert(*i);
        }
    }
    e->get<CArmyHUD>()->units2 = newList;

    //spacing between icon and displayer
    double spacing = 30;

    ///CREATE NEW DISPLAYERS
    for(list<CUnit::ID>::iterator i = uniqueUnits.begin(); i != uniqueUnits.end(); i++){
        map<CUnit::ID, IconDisplayerPair>::iterator it;
        it = e->get<CArmyHUD>()->units2.find(*i);
        if (it == e->get<CArmyHUD>()->units2.end()){
            CUnit::ID id = *i;

            Entity* eIcon = eManager->createEntity();
            eIcon->add(new CUnit(CUnit::Map[id]));
            eIcon->add(new CPosition(x, -50));
            eIcon->add(new CTexture(CUnit::Map[id].displayer, hFlip));
            eIcon->add(new CDraw(CDraw::HUD2));
            eIcon->add(new CButtonState());
            eIcon->add(new CButtonHitbox(Assets::getTexture(CUnit::Map[id].displayer)->getSize().x, Assets::getTexture(CUnit::Map[id].displayer)->getSize().y));
            eIcon->add(new CButtonTrigger());
            eIcon->add(new CTooltip(CTooltip::UNIT));
            eIcon->add(new COwner(e));
            eIcon->add(new CHighlightTrigger(CUnitHighlight2::UNIT, id));
            eIcon->add(new CVelocity());
            eIcon->add(new CActor());
            eIcon->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
            eIcon->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;

            Entity* eDisplayer = eManager->createEntity();
            eDisplayer->add(new CPosition(x, -50));
            eDisplayer->add(new CTexture("unit-counter.png", hFlip));
            eDisplayer->add(new CDisplayer(CDisplayer::UNIT_COUNT, eDisplayer));
            eDisplayer->add(new CTextbox2("", Assets::getFont(Assets::secondaryFont), 15, sf::Color::Black, 0, -2));
            eDisplayer->add(new CDraw(CDraw::HUD1));
            eDisplayer->add(new COwner(e));
            eDisplayer->add(new CUnit(CUnit::Map[id]));
            eDisplayer->add(new CVelocity());
            eDisplayer->add(new CActor());

            IconDisplayerPair iconDisp(eIcon, eDisplayer);
            e->get<CArmyHUD>()->units2.insert(make_pair(id, iconDisp));

            x = cxWindow + sign*xOff + getIndex(e->get<CArmyHUD>()->units2, id)*sign*dx;
            eIcon->get<CPosition>()->x = x;
            eDisplayer->get<CPosition>()->x = x;
            animateUnitIn(iconDisp);
            rearrange = true;
        }
    }

    if (rearrange) arrangeUnits(e);

}

void ArmyHUDSystem::arrangeUnits(Entity* e){
    int sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double xOff = 160;
    double yOff = 70;
    double x = cxWindow + sign*xOff;
    double y = yOff;
    double speed = 50;
    double dx = 50;

    for(auto& i : e->get<CArmyHUD>()->units2){
        if (i.second.eDisplayer->get<CPosition>()->x != x && i.second.eDisplayer->get<CActor>()->timeline.empty()){
            i.second.eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, x, i.second.eIcon->get<CPosition>()->y, speed));
            i.second.eDisplayer->get<CActor>()->timeline.push_back(new AMove(0.0, x, i.second.eDisplayer->get<CPosition>()->y, speed));
        }
        x += sign*dx;
    }
}

void ArmyHUDSystem::arrangeEffects(Entity* e){
    int sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double xOff = 160;
    double yOff = 130;
    double x = cxWindow + sign*xOff;
    double y = yOff;
    double speed = 50;
    double dx = 50;

    for(map<CAction::ID, Entity*>::iterator i = e->get<CArmyHUD>()->effects.begin(); i != e->get<CArmyHUD>()->effects.end(); i++){
        Entity* eIcon = i->second;
        if (eIcon->get<CPosition>()->x != x && eIcon->get<CActor>()->timeline.empty()){
            eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, x, eIcon->get<CPosition>()->y, speed));
            eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, x, eIcon->get<CPosition>()->y, speed));
        }
        x += sign*dx;
    }
}

void ArmyHUDSystem::updateEffects(Entity* e){
    list<CAction::ID> effects;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++){
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        for (map<CAction::ID, CAction>::iterator j = eUnit->get<CUnit>()->effects.begin(); j != eUnit->get<CUnit>()->effects.end(); j++){
            int id = j->first;
            if (!contains(effects, id)){
                effects.push_back(id);
            }
        }
    }
    for(map<CAction::ID, CAction>::iterator i = e->get<CArmy>()->armyEffects.begin(); i != e->get<CArmy>()->armyEffects.end(); i++){
        int id = i->first;
        if (!contains(effects, id)){
            effects.push_back(id);
        }
    }

    list<CAction::ID> oldEffects;
    for(map<CAction::ID, Entity*>::iterator it = e->get<CArmyHUD>()->effects.begin(); it != e->get<CArmyHUD>()->effects.end(); it++){
        oldEffects.push_back(it->first);
    }

    bool rearrange = false;
    int nEffects = effects.size();
    int sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
    double xOff = 160;
    double yOff = 130;
    double dx = 50;
    double x = cxWindow + sign*xOff + sign*(nEffects-1)*dx;
    double y = yOff;
    ///====================
    /// INSERT NEW EFFECTS
    ///====================
    for(list<CAction::ID>::iterator i = effects.begin(); i != effects.end(); i++){
        map<CAction::ID, Entity*>::iterator it;
        it = e->get<CArmyHUD>()->effects.find(*i);
        if (it == e->get<CArmyHUD>()->effects.end()){
            Entity* eIcon = eManager->createEntity();
            eIcon->add(new CDraw(CDraw::GUI1));
            eIcon->add(new CPosition(x, y));
            eIcon->add(new CTexture("button-action-00-default.png"));
            eIcon->add(new CDimensions(36,36));
            eIcon->add(new CButtonHitbox(36,36));
            eIcon->add(new CButtonState());
            eIcon->add(new CButtonTrigger());
            eIcon->add(new CHighlightTrigger(CUnitHighlight2::EFFECT, *i));
            eIcon->add(new COwner(e));
            eIcon->add(new CAction(*i));
            eIcon->add(new CActor());
            eIcon->add(new CVelocity());
            eIcon->add(new CTooltip(CTooltip::EFFECT));
            eIcon->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
            eIcon->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;
            e->get<CArmyHUD>()->effects.insert(make_pair(*i, eIcon));
            animateButtonInPuff(eIcon, 0.0);
            x += sign*dx;
            rearrange = true;
        }
    }

    ///====================
    /// REMOVE OLD EFFECTS
    ///====================
    for(list<CAction::ID>::iterator i = oldEffects.begin(); i != oldEffects.end(); i++){
        if (!contains(effects, *i)){
            animateButtonOutPuff(e->get<CArmyHUD>()->effects[*i], 0.0);
            map<CAction::ID, Entity*>::iterator it = e->get<CArmyHUD>()->effects.find(*i);
            e->get<CArmyHUD>()->effects.erase(it);
            rearrange = true;
        }
    }

    ///====================
    /// REPOSITION
    ///====================
    //if (rearrange) arrangeEffects(e);
}

void ArmyHUDSystem::updateCoin(Entity* e){
    if (e->get<CArmy>()->hasCoin && e->get<CArmyHUD>()->eCoin == nullptr){
        int sign = e->get<CArmy>()->side == CArmy::LEFT ? -1:1;
        double xOff = 50;
        double yOff = 90;
        double x = cxWindow + sign*xOff;
        double y = yOff;
        bool hFlip = sign == -1? false:true;

        Entity* eCoin = eManager->createEntity();
        eCoin->add(new CPosition(cxWindow + sign*xOff, yOff));
        eCoin->add(new CTexture(sign == -1 ? "blue-coin-official.png" : "red-coin-official.png"));
        eCoin->add(new CDimensions(20,20));
        eCoin->add(new CDraw(CDraw::GUI2));

        e->get<CArmyHUD>()->eCoin = eCoin;

    }else if (!e->get<CArmy>()->hasCoin && e->get<CArmyHUD>()->eCoin != nullptr){
        eManager->removeEntity(e->get<CArmyHUD>()->eCoin);
        e->get<CArmyHUD>()->eCoin = nullptr;
    }
}

double ArmyHUDSystem::getAverageResistance(Entity* e, CUnit::DamageType dmgType){
    double sum = 0;
    int n = 0;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++){
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sum += eUnit->get<CUnit>()->resistance[dmgType];
        n++;
    }
    if (n == 0) return 0;
    return sum/n;
}

void ArmyHUDSystem::onHighlightUnits(Entity* e){
    if (e->get<CHighlightTrigger>()->info == CUnitHighlight2::ATTACK){
        Entity* eArmy = e->get<COwner>()->e;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            if (eUnit->get<CUnit>()->dead) continue;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            if (eUnit->get<CUnit>()->id == value){
                eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::ON;
                eUnit->get<CUnitHighlight2>()->info = info;
                eUnit->get<CUnitHighlight2>()->value = value;
                eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][2];
            }
        }
        eArmy = eArmy->get<CPlayer>()->enemy;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            if (eUnit->get<CUnit>()->dead) continue;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            int damageType = CUnit::Map[value].damage;
            int res = eUnit->get<CUnit>()->realResist[damageType];
            eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::ON;
            eUnit->get<CUnitHighlight2>()->info = info;
            eUnit->get<CUnitHighlight2>()->value = value;
            eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][res];
        }
    }else if (e->get<CHighlightTrigger>()->info == CUnitHighlight2::ACTING){
        list<TargetOutcome>::iterator it;
        for(it = e->get<CActionOutcome>()->outcomes.begin(); it != e->get<CActionOutcome>()->outcomes.end(); it++){
            TargetOutcome out = *it;
            Entity* eAtk = out.eCauser;
            Entity* eDef = out.eTarget;
            Entity* eArmy;

            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;

            eArmy = eAtk->get<COwner>()->e;
            eAtk->get<CUnitHighlight2>()->state = CUnitHighlight2::ON;
            eAtk->get<CUnitHighlight2>()->info = info;
            eAtk->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][2];

            eArmy = eDef->get<COwner>()->e;
            int damageType = eAtk->get<CUnit>()->damage;
            int res = eDef->get<CUnit>()->realResist[damageType];
            eDef->get<CUnitHighlight2>()->state = CUnitHighlight2::ON;
            eDef->get<CUnitHighlight2>()->info = info;
            eDef->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][res];
        }
    }else if (e->get<CHighlightTrigger>()->info == CUnitHighlight2::RESISTANCE){
        Entity* eArmy = e->get<COwner>()->e;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            if (eUnit->get<CUnit>()->dead) continue;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::ON;
            eUnit->get<CUnitHighlight2>()->info = CUnitHighlight2::RESISTANCE;
            eUnit->get<CUnitHighlight2>()->value = value;
            eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][eUnit->get<CUnit>()->realResist[value]];
        }
        eArmy = eArmy->get<CPlayer>()->enemy;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            if (eUnit->get<CUnit>()->dead) continue;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::ON;
            eUnit->get<CUnitHighlight2>()->info = CUnitHighlight2::RESISTANCE;
            eUnit->get<CUnitHighlight2>()->value = value;
            eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][eUnit->get<CUnit>()->realResist[value]];
        }
    }else{
        Entity* eArmy = e->get<COwner>()->e;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            if (eUnit->get<CUnit>()->dead) continue;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            switch(info){
                case CUnitHighlight2::UNIT:
                    if (eUnit->get<CUnit>()->id == value){
                        eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::ON;
                        eUnit->get<CUnitHighlight2>()->info = CUnitHighlight2::UNIT;
                        eUnit->get<CUnitHighlight2>()->value = value;
                        eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][2];
                    }
                    break;
                case CUnitHighlight2::EFFECT:
                    if (mapContains(eUnit->get<CUnit>()->effects, value)){
                        eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::ON;
                        eUnit->get<CUnitHighlight2>()->info = CUnitHighlight2::EFFECT;
                        eUnit->get<CUnitHighlight2>()->value = value;
                        eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][2];
                    }
                    break;
                default: break;
            }
        }
    }
}

void ArmyHUDSystem::onHighlightUnitsOff(Entity* e){
    if (e->get<CHighlightTrigger>()->info == CUnitHighlight2::ATTACK){
        Entity* eArmy = e->get<COwner>()->e;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            if (eUnit->get<CUnit>()->dead) continue;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            if (eUnit->get<CUnit>()->id == value){
                eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::OFF;
                eUnit->get<CUnitHighlight2>()->info = info;
                eUnit->get<CUnitHighlight2>()->value = value;
                eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][2];
            }
        }
        eArmy = eArmy->get<CPlayer>()->enemy;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            if (eUnit->get<CUnit>()->dead) continue;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            int damageType = eUnit->get<CUnit>()->damage;
            int res = eUnit->get<CUnit>()->realResist[damageType];
            eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::OFF;
            eUnit->get<CUnitHighlight2>()->info = info;
            eUnit->get<CUnitHighlight2>()->value = value;
            eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][res];
        }
    }else if (e->get<CHighlightTrigger>()->info == CUnitHighlight2::ACTING){
        list<TargetOutcome>::iterator it;
        for(it = e->get<CActionOutcome>()->outcomes.begin(); it != e->get<CActionOutcome>()->outcomes.end(); it++){
            TargetOutcome out = *it;
            Entity* eAtk = out.eCauser;
            Entity* eDef = out.eTarget;
            Entity* eArmy;

            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;

            eArmy = eAtk->get<COwner>()->e;
            eAtk->get<CUnitHighlight2>()->state = CUnitHighlight2::OFF;
            eAtk->get<CUnitHighlight2>()->info = info;
            eAtk->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][2];

            eArmy = eDef->get<COwner>()->e;
            int damageType = eAtk->get<CUnit>()->damage;
            int res = eDef->get<CUnit>()->realResist[damageType];
            eDef->get<CUnitHighlight2>()->state = CUnitHighlight2::OFF;
            eDef->get<CUnitHighlight2>()->info = info;
            eDef->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][res];
        }
    }else if (e->get<CHighlightTrigger>()->info == CUnitHighlight2::RESISTANCE){
        Entity* eArmy = e->get<COwner>()->e;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            if (eUnit->get<CUnitHighlight2>()->info == info && eUnit->get<CUnitHighlight2>()->value == value){
                eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::OFF;
                eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][eUnit->get<CUnit>()->realResist[value]];
            }
        }
        eArmy = eArmy->get<CPlayer>()->enemy;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            if (eUnit->get<CUnitHighlight2>()->info == info && eUnit->get<CUnitHighlight2>()->value == value){
                eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::OFF;
                eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][eUnit->get<CUnit>()->realResist[value]];
            }
        }
    }else{
        Entity* eArmy = e->get<COwner>()->e;
        for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
            Entity* eUnit = *i;
            CUnitHighlight2::Info info = e->get<CHighlightTrigger>()->info;
            int value = e->get<CHighlightTrigger>()->value;
            switch(info){
                case CUnitHighlight2::UNIT:
                    if (eUnit->get<CUnitHighlight2>()->info == info && eUnit->get<CUnitHighlight2>()->value == value){
                        eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::OFF;
                        eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][2];
                    }
                    break;
                case CUnitHighlight2::EFFECT:
                    if (mapContains(eUnit->get<CUnit>()->effects, value)){
                        eUnit->get<CUnitHighlight2>()->state = CUnitHighlight2::OFF;
                        eUnit->get<CUnitHighlight2>()->info = CUnitHighlight2::EFFECT;
                        eUnit->get<CUnitHighlight2>()->value = value;
                        eUnit->get<CUnitHighlight2>()->color = Assets::scale[eArmy->get<CPlayer>()->color][2];
                    }
                    break;
                default: break;
            }
        }
    }
}

bool ArmyHUDSystem::mapContains(map<CAction::ID, CAction>& m, CAction::ID id){
    map<CAction::ID, CAction>::iterator it;
    it = m.find(id);
    return it != m.end();
}

int ArmyHUDSystem::getIndex(map<CUnit::ID, IconDisplayerPair>& m, CUnit::ID id){
    int index = 0;
    for (auto& i : m){
        if (i.first == id) return index;
        else index++;
    }
    return index;
}
void ArmyHUDSystem::animateButtonInPuff(Entity* e, double after, bool sound){
    e->get<CDraw>()->isHidden = true;
    e->get<CButtonState>()->state = CButtonState::LOCKED;
    e->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::HIDDEN, true));
    e->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::BUT_LOCKED, true));

    Entity* eSmoke = eManager->createEntity();
    string animation = "poof-03.png";
    double w = Assets::getAnimation(animation).wSprite;
    double h = Assets::getAnimation(animation).hSprite;
    if (e->has(Component::DIMENSIONS)){
        w = max(w, e->get<CDimensions>()->width);
        h = max(h, e->get<CDimensions>()->height);
    }
    eSmoke->add(new CAnimation(false, animation));
    eSmoke->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y));
    eSmoke->add(new CDraw(CDraw::GUI5));
    eSmoke->add(new CDimensions(w, h));
    eSmoke->add(new CActor());

    eSmoke->get<CDraw>()->isHidden = true;
    eSmoke->get<CActor>()->timeline.push_back(new AVariable(after, AVariable::HIDDEN, false));
    eSmoke->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
    if (sound) eSmoke->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-poof-01.wav"));
    eSmoke->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));

    e->get<CActor>()->timeline.push_back(new AVariable(after+Assets::getAnimation(animation).rate, AVariable::HIDDEN, false));
    e->get<CActor>()->timeline.push_back(new AVariable(Assets::getAnimation(animation).duration, AVariable::BUT_LOCKED, false));
}
void ArmyHUDSystem::animateButtonOutPuff(Entity* e, double after, bool sound){
    notify(BUTTON_LOST_FOCUS, e);
    e->get<CDraw>()->isHidden = true;
    e->get<CButtonState>()->state = CButtonState::LOCKED;
    e->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::HIDDEN, true));
    e->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::BUT_LOCKED, true));

    Entity* eSmoke = eManager->createEntity();
    string animation = "poof-03.png";
    double w = Assets::getAnimation(animation).wSprite;
    double h = Assets::getAnimation(animation).hSprite;
    if (e->has(Component::DIMENSIONS)){
        w = max(w, e->get<CDimensions>()->width);
        h = max(h, e->get<CDimensions>()->height);
    }
    eSmoke->add(new CAnimation(false, animation));
    eSmoke->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y));
    eSmoke->add(new CDraw(CDraw::GUI5));
    eSmoke->add(new CDimensions(w, h));
    eSmoke->add(new CActor());

    eSmoke->get<CDraw>()->isHidden = true;
    eSmoke->get<CActor>()->timeline.push_back(new AVariable(after, AVariable::HIDDEN, false));
    eSmoke->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
    if (sound) eSmoke->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-poof-03.wav"));
    eSmoke->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));

    e->get<CActor>()->timeline.push_back(new ADestroy(after+Assets::getAnimation(animation).duration));
}

