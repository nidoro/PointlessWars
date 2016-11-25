#include "AnnouncerSystem.h"

AnnouncerSystem::AnnouncerSystem(){
    addSubscription(INITIALIZE_WAR);
    addSubscription(END_MATCH);

    eText = nullptr;
    eBanner = nullptr;
    active = false;
    currentString = "";
}

AnnouncerSystem::~AnnouncerSystem(){
    //dtor
}

void AnnouncerSystem::update(){
    if (active){
        if (eBanner == nullptr) createAnnouncer();
        nextString = getGameStateString();
        if (nextString != currentString){
            startTextTransition();
        }
    }
}

void AnnouncerSystem::createAnnouncer(){
    eBanner = eManager->createEntity();
    eBanner->add(new CPosition(cxWindow, 20));
    eBanner->add(new CTexture("9p-banner.png"));
    eBanner->add(new CDimensions(250, 31));
    eBanner->add(new CActor());
    eBanner->add(new CDraw(CDraw::GUI1, 0));
    //eBanner->add(new CAutoPilot());

    eBanner->get<CActor>()->addNode(new AFade(0.0, 127, 255));


    eText = eManager->createEntity();
    eText->add(new CPosition(cxWindow, 20));
    eText->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 16));
    eText->add(new CDraw(CDraw::GUI2, 0));
    eText->add(new CActor());
}

void AnnouncerSystem::startTextTransition(){
    double tAux = 0;
    eText->get<CActor>()->addNode(new AFade(0.0, -255, 0));
    eText->get<CActor>()->addNode(new ADestroy(1.0));

    eText = eManager->createEntity();
    eText->add(new CPosition(cxWindow, 15));
    eText->add(new CTextbox2(nextString, Assets::getFont(Assets::getPrimaryFont()), 16));
    eText->add(new CDraw(CDraw::GUI2, 0));
    eText->add(new CActor());

    eText->get<CActor>()->addNode(new AFade(1.0, 255, 255));

    double wTarget = eText->get<CTextbox2>()->content.getLocalBounds().width + 60;
    double stretchSpeed = wTarget > eBanner->get<CDimensions>()->width ? 150 : -150;

    for (list<AnimationNode*>::iterator it = eBanner->get<CActor>()->timeline.begin(); it != eBanner->get<CActor>()->timeline.end(); it++){
        delete *it;
    }
    eBanner->get<CActor>()->timeline.clear();

    eBanner->get<CActor>()->addNode(new AVariable(0.0, AVariable::AUTO_P, true));
    eBanner->get<CActor>()->addNode(new AZoom(0.0, stretchSpeed, 0, wTarget, 31));
    eBanner->get<CActor>()->addNode(new AVariable(abs(wTarget-eBanner->get<CDimensions>()->width)/abs(stretchSpeed), AVariable::AUTO_P, false));

    currentString = nextString;
}

void AnnouncerSystem::onInitializeWar(Entity* e){
    active = true;
}

void AnnouncerSystem::onEndMatch(Entity* e){
    eText = nullptr;
    eBanner = nullptr;
    active = false;
    currentString = "";
}

string AnnouncerSystem::getGameStateString(){
    Entity* ePlayer = war.getActor();
    string playerName;
    string stateString;
    if (ePlayer) playerName = ePlayer->get<CPlayer>()->name;
    if (war.getSystemAction() == war.NONE){
        return "";
    }else if (war.getSystemAction() == war.COIN){
        return "Behold the almighty coin...";
    }else if (war.getSystemAction() == war.ASK_HERO_PICK){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_FORMATION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.PLAY_FORMATION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ADVANCE_ARMY){
        return "";
    }else if (war.getSystemAction() == war.PLAY_ACTION){
    }else if (war.getSystemAction() == war.BETWEEN_ROUNDS){
    }else if (war.getSystemAction() == war.ASK_CAPTAIN_ACTION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_ARMY_ACTION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_BATTLE_CLOSURE){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_ARMY_ASSEMBLE){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_CAPTAIN_SELECTION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.PRESENT_ARMY){
    }else if (war.getSystemAction() == war.PRESENT_ARMIES){
        return "Presenting for duty...";
    }else if (war.getSystemAction() == war.RECOMPOSE_ARMY){
    }else if (war.getSystemAction() == war.SHOW_HERO_POOL){

    }else if (war.getSystemAction() == war.START_FIRST_BATTLE){
    }else if (war.getSystemAction() == war.START_BATTLE){
    }else if (war.getSystemAction() == war.FIELD_CLEANUP){
    }else if (war.getSystemAction() == war.BETWEEN_TURNS){
    }else if (war.getSystemAction() == war.ENDING){
    }else if (war.getSystemAction() == war.UPDATE_SCORE){
    }
    return "";
}


