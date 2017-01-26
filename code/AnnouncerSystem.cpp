#include "AnnouncerSystem.h"

AnnouncerSystem::AnnouncerSystem() {
    addSubscription(INITIALIZE_WAR);
    addSubscription(END_MATCH);
    addSubscription(KEY_PRESSED);

    eText = nullptr;
    eBanner = nullptr;
    active = false;
    currentString = "";
}


AnnouncerSystem::~AnnouncerSystem() {
    //dtor
}

void AnnouncerSystem::update() {
    if (active) {
        if (eBanner == nullptr) createAnnouncer();
        nextString = getGameStateString();
        if (nextString != currentString) {
            startTextTransition();
        }
    }
}

void AnnouncerSystem::createAnnouncer() {
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

void AnnouncerSystem::startTextTransition() {
    //unused:
    //double tAux = 0;
    //eText->get<CActor>()->addNode(new AFade(0.0, -255, 0));
    eText->get<CActor>()->addNode(new ADestroy(0.0));
    bool isHidden = eText->get<CDraw>()->isHidden;

    eText = eManager->createEntity();
    eText->add(new CPosition(cxWindow, 15));
    eText->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 16));
    eText->add(new CDraw(CDraw::GUI2));
    eText->add(new CTypingEffect(nextString, 40));
    eText->add(new CActor());
    eText->get<CDraw>()->isHidden = isHidden;

    //eText->get<CActor>()->addNode(new AFade(1.0, 255, 255));
    CTextbox2 tb(nextString, Assets::getFont(Assets::getPrimaryFont()), 16);

    double wTarget = tb.content.getLocalBounds().width + 60;
    double stretchSpeed = wTarget > eBanner->get<CDimensions>()->width ? 350 : -350;

    for (list<AnimationNode*>::iterator it = eBanner->get<CActor>()->timeline.begin(); it != eBanner->get<CActor>()->timeline.end(); it++) {
        delete *it;
    }
    eBanner->get<CActor>()->timeline.clear();

    eBanner->get<CActor>()->addNode(new AVariable(0.0, AVariable::AUTO_P, true));
    eBanner->get<CActor>()->addNode(new AZoom(0.0, stretchSpeed, 0, wTarget, 31));
    eBanner->get<CActor>()->addNode(new AVariable(abs(wTarget-eBanner->get<CDimensions>()->width)/abs(stretchSpeed), AVariable::AUTO_P, false));

    currentString = nextString;
}

void AnnouncerSystem::onInitializeWar(Entity* e) {
    active = true;
}

void AnnouncerSystem::onEndMatch(Entity* e) {
    eText = nullptr;
    eBanner = nullptr;
    active = false;
    currentString = "";
}

string AnnouncerSystem::getGameStateString() {
    Entity* ePlayer = war.getActor();
    CAction::ID actionID = -1;
    string playerName;
    string stateString;
    if (ePlayer) playerName = ePlayer->get<CPlayer>()->name;
    if (ePlayer) actionID = war.getNextActionOutcome(ePlayer->get<CPlayer>()->id).action;

    if (war.getSystemAction() == war.NONE) {
        return "";
    } else if (war.getSystemAction() == war.COIN) {
        return getString("BANNER-COIN");
    } else if (war.getSystemAction() == war.ASK_HERO_PICK) {
        return playerName + getString("BANNER-PLAYER-PICKING-HERO");
    } else if (war.getSystemAction() == war.ASK_FORMATION) {
        return playerName + getString("BANNER-PLAYER-SELECTING-FORMATION");
    } else if (war.getSystemAction() == war.PLAY_FORMATION) {
        return playerName + getString("BANNER-PLAYER-SELECTING-FORMATION");
    } else if (war.getSystemAction() == war.ADVANCE_ARMY) {
        return "";
    } else if (war.getSystemAction() == war.PLAY_ACTION) {
    } else if (war.getSystemAction() == war.BETWEEN_ROUNDS) {
    } else if (war.getSystemAction() == war.ASK_CAPTAIN_ACTION) {
        if (war.getNextActionOutcome(ePlayer->get<CPlayer>()->id).action == -1) {
            return playerName + getString("BANNER-PLAYER-SELECTING-HERO-ACTION");
        } else {
            return playerName + getString("BANNER-PLAYER-USED-HERO-ACTION")+ Assets::getString("NAME-ACTION-" + int2str(actionID)) + "!";
        }
    } else if (war.getSystemAction() == war.ASK_ARMY_ACTION) {
        if (war.getNextActionOutcome(ePlayer->get<CPlayer>()->id).action == -1) {
            return playerName + getString("BANNER-PLAYER-SELECTING-ATTACK");
        } else {
            return playerName + getString("BANNER-PLAYER-ATTACKING");
        }
    } else if (war.getSystemAction() == war.ASK_BATTLE_CLOSURE) {
        if (war.getPlayingAnimation()) {
            std::string winnerName = war.getPlayer(war.getBattleWinner())->get<CPlayer>()->name;
            std::string loserName = war.getPlayer(war.getBattleLoser())->get<CPlayer>()->name;
            if (war.getBattleClosure() == war.ALL_KILLED) return  getString("BANNER-WHEN-ALL-KILLED");
            if (war.getBattleClosure() == war.ARMY_VS_ARMY) return getString("BANNER-WHEN-ARMY-VS-ARMY");
            if (war.getBattleClosure() == war.MERCY) return winnerName + getString("BANNER-WHEN-MERCY");
            if (war.getBattleClosure() == war.CONFINE) return loserName + getString("BANNER-WHEN-CONFINE");
            if (war.getBattleClosure() == war.MAN_VS_MAN) return getString("BANNER-WHEN-MAN-VS-MAN");
        } else {
            return playerName + getString("BANNER-PLAYER-SELECTING-BATTLE-CLOSURE");
        }
    } else if (war.getSystemAction() == war.CLEAR_BATTLE_FIELD) {
        std::string winnerName = war.getPlayer(war.getBattleWinner())->get<CPlayer>()->name;
        std::string loserName = war.getPlayer(war.getBattleLoser())->get<CPlayer>()->name;
        if (war.getBattleClosure() == war.ALL_KILLED) return  getString("BANNER-WHEN-ALL-KILLED");
        if (war.getBattleClosure() == war.ARMY_VS_ARMY) return getString("BANNER-WHEN-ARMY-VS-ARMY");
        if (war.getBattleClosure() == war.MERCY) return winnerName + getString("BANNER-WHEN-MERCY");
        if (war.getBattleClosure() == war.CONFINE) return loserName + getString("BANNER-WHEN-CONFINE");
        if (war.getBattleClosure() == war.MAN_VS_MAN) return getString("BANNER-WHEN-MAN-VS-MAN");

    } else if (war.getSystemAction() == war.ASK_ARMY_ASSEMBLE) {
        return playerName + getString("BANNER-PLAYER-RECRUITING");
    } else if (war.getSystemAction() == war.ASK_CAPTAIN_SELECTION) {
        return playerName + getString("BANNER-PLAYER-SELECTING-HERO");
    } else if (war.getSystemAction() == war.PRESENT_ARMY) {
    } else if (war.getSystemAction() == war.PRESENT_ARMIES) {
        return getString("BANNER-WHEN-PRESENTING-ARMIES");
    } else if (war.getSystemAction() == war.RECOMPOSE_ARMY) {
    } else if (war.getSystemAction() == war.SHOW_HERO_POOL) {

    } else if (war.getSystemAction() == war.START_FIRST_BATTLE) {
    } else if (war.getSystemAction() == war.START_BATTLE) {
    } else if (war.getSystemAction() == war.FIELD_CLEANUP) {
    } else if (war.getSystemAction() == war.BETWEEN_TURNS) {
    } else if (war.getSystemAction() == war.ENDING) {
    } else if (war.getSystemAction() == war.UPDATE_SCORE) {
    } else if (war.getSystemAction() == war.ANNOUNCE_VICTORY) {
        Entity* eWinner = war.getPlayer(war.getBattleWinner());
        return eWinner->get<CPlayer>()->name + getString("BANNER-ANNOUNCE-VICTORY");
    }
    return "";
}

void AnnouncerSystem::onKeyPressed(Entity* e) {
    if (e->get<CKeyboardInput>()->code == sf::Keyboard::F2) {
        if (eText) eText->get<CDraw>()->isHidden = !eText->get<CDraw>()->isHidden;
        if (eBanner) eBanner->get<CDraw>()->isHidden = !eBanner->get<CDraw>()->isHidden;
    }
}

std::string AnnouncerSystem::getString(std::string id) {
    return Assets::getString(id);
}
