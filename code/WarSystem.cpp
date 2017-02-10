#include "WarSystem.h"

WarSystem::WarSystem() {
    addSubscription(INITIALIZE_WAR);
    addSubscription(PACKET_RECEIVED);
    addSubscription(END_MATCH);
    addSubscription(SET_MATCH_CONFIG);
    
    inBattle = false;
    hasMatchEnded = true;
}

void WarSystem::update() {
    processPackets();
    if (!hasMatchEnded) {
        if (war.getActionCompleted() && !war.getPlayingAnimation()) {
            callNextSystemAction();
            initializeState();
            notify(SYSTEM_ACTION);
            //sendStateToPeer();
        }
    }

    for (int i = 0; i < 3; i++) {
        if (war.getMultiplayer() && !war.getRemotelyControled(i) && !war.getActionSent(i) && war.getNextActionOutcome(i).ready) {
            sendAction(i);
            war.setActionSent(i, true);
        }
        if ((war.getMultiplayer() && war.getActionReceived(i) && !war.getActionCompleted(i)) || (!war.getMultiplayer() && !war.getActionCompleted(i))) {
            war.setPlayerReady(i, true);
        }
        if (war.getRemotelyControled(i) && war.hasPendingAction(i) && !war.getActionCompleted(i) && !war.getActionReceived(i)) {
            war.getNextActionOutcome(i) = war.pullPendingAction(i);
            war.setActionReceived(i, true);

            Entity* eSend = eManager->createEntity();
            eSend->add(new CPacket());
            eSend->get<CPacket>()->packet << sf::String("ACTION-RECEIVED");
            eSend->get<CPacket>()->packet << sf::Int32(i);
            notify(SEND_PACKET, eSend);
        }
    }
}

void WarSystem::sendAction(CPlayer::ID id) {
    sf::Packet packet;
    packet << sf::String("ACTION");
    packet << sf::Int32(id);
    packet << war.getNextActionOutcome(id);
    Entity* ePacket = eManager->createEntity();
    ePacket->add(new CPacket(packet));
    notify(SEND_PACKET, ePacket);
}

void WarSystem::onInitializeWar(Entity* e) {

    war.getMatchConfig().randomArmy = false;
    war.getMatchConfig().maxRepicks = 3;

    Entity* eBtReturn = eManager->createEntity();
    eBtReturn->add(new CScreen(CScreen::MAIN_MENU, CScreen::FADE_BLACK));
    eBtReturn->add(new CButtonState());
    eBtReturn->add(new CButtonHitbox());
    eBtReturn->add(new CPosition());
    eBtReturn->add(new CButtonTrigger(CREATE_GUI_GROUP, CButtonTrigger::ON_RELEASE, sf::Keyboard::Escape));

    Entity* eGUI = eManager->createEntity();
    eGUI->add(new CGUIGroup("window", "in-game-menu"));
    eGUI->add(new CUILayer(CUILayer::L1));
    eGUI->add(new CDraw(CDraw::GUI_00));

    eBtReturn->addObservedEntity("create-gui-group", eGUI);

    Entity* P1 = eManager->createEntity();
    Entity* P2 = eManager->createEntity();

    P1->add(new CArmy(CPlayer::LEFT));
    P2->add(new CArmy(CPlayer::RIGHT));

    P1->add(new CArmyHUD());
    P2->add(new CArmyHUD());

    //P1->add(new CAverageUnit());
    //P2->add(new CAverageUnit());

    P1->add(new CPosition());
    P2->add(new CPosition());

    P1->add(new CPlayer());
    P2->add(new CPlayer());
    P1->get<CPlayer>()->enemy = P2;
    P2->get<CPlayer>()->enemy = P1;
    P1->get<CPlayer>()->maxPicks = war.getMatchConfig().armySize;
    P2->get<CPlayer>()->maxPicks = war.getMatchConfig().armySize;

    P1->get<CPlayer>()->id = 1;
    P2->get<CPlayer>()->id = 2;

    if (war.getRemotelyControled(1)) {
        P1->get<CPlayer>()->side = CPlayer::RIGHT;
        P2->get<CPlayer>()->side = CPlayer::LEFT;
        P1->get<CArmy>()->side = CPlayer::RIGHT;
        P2->get<CArmy>()->side = CPlayer::LEFT;
        P1->get<CPlayer>()->color = RED;
        P2->get<CPlayer>()->color = BLUE;
        P1->add(new CRemotelyControled());
        P2->add(new CRemoteControler());
    } else {
        P1->get<CPlayer>()->side = CPlayer::LEFT;
        P2->get<CPlayer>()->side = CPlayer::RIGHT;
        P1->get<CArmy>()->side = CPlayer::LEFT;
        P2->get<CArmy>()->side = CPlayer::RIGHT;
        P1->get<CPlayer>()->color = BLUE;
        P2->get<CPlayer>()->color = RED;
        P2->add(new CRemotelyControled());
        P1->add(new CRemoteControler());
    }

    P1->get<CPlayer>()->name = "Jeff";
    P2->get<CPlayer>()->name = "Bobby bot";

    P1->get<CPlayer>()->maxRepicks = war.getMatchConfig().maxRepicks;
    P2->get<CPlayer>()->maxRepicks = war.getMatchConfig().maxRepicks;

    P1->get<CPlayer>()->unitDeck = war.getMatchConfig().unitPool;
    P2->get<CPlayer>()->unitDeck = war.getMatchConfig().unitPool;

    if (!war.getMultiplayer()) P2->add(new CAInt());

    war.setPlayers(P1, P2);
    war.setPlaying(true);

    startHeroPoolActionQueue(P1);
    callNextSystemAction();
    initializeState();
    notify(SYSTEM_ACTION);

    hasMatchEnded = false;
    inBattle = false;

    /*
    Entity* eDisplayer = eManager->createEntity();
    eDisplayer->add(new CPosition(cxWindow, 25));
    eDisplayer->add(new CRectShape(300, 20, sf::Color::White, 1));
    eDisplayer->add(new CDisplayer(CDisplayer::GAME_STATE, nullptr));
    eDisplayer->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 16));
    eDisplayer->add(new CDraw(CDraw::GUI1));

    Entity* eMusic = eManager->createEntity();
    eMusic->add(new CMusic("forest-loop.ogg"));
    notify(PLAY_MUSIC, eMusic);
    */

}

void WarSystem::startHeroPoolActionQueue(Entity* eFirst) {
    sysActionQueue.clear();
    Entity* eSecond = eFirst->get<CPlayer>()->enemy;
    CPlayer::ID idFirst = eFirst->get<CPlayer>()->id;
    CPlayer::ID idSecond = eSecond->get<CPlayer>()->id;
    int nPicks = war.getMatchConfig().nPlayerHeroes;
    addSystemAction(War::SHOW_HERO_POOL);
    bool firstPick = true;
    int nHeroesP1 = 0;
    int nHeroesP2 = 0;
    while(nHeroesP1 < nPicks && nHeroesP2 < nPicks) {
        addSystemAction(War::ASK_HERO_PICK, idFirst);
        nHeroesP1++;
        if (!firstPick && nHeroesP1 < nPicks) {
            addSystemAction(War::ASK_HERO_PICK, idFirst);
            nHeroesP1++;
        }
        addSystemAction(War::ASK_HERO_PICK, idSecond);
        nHeroesP2++;
        if (nHeroesP2 < nPicks) {
            addSystemAction(War::ASK_HERO_PICK, idSecond);
            nHeroesP2++;
        }
        firstPick = false;
    }
    addSystemAction(War::END_HERO_POOL);
    addSystemAction(War::START_FIRST_BATTLE, idFirst);
}

void WarSystem::addSystemAction(War::SystemAction s, CPlayer::ID p) {
    sysActionQueue.push_back(make_pair(s,p));
}

void WarSystem::callNextSystemAction() {
    checkBattleClosure();
    if (!sysActionQueue.empty()) {
        war.setSystemAction(sysActionQueue.front().first);
        if (sysActionQueue.front().second != -1) war.setActorID(sysActionQueue.front().second);
        sysActionQueue.pop_front();
    } else {
        //printf("empty queue???\n");
    }
}

void WarSystem::initializeState() {
    for(int i = 0; i < 3; i++) {
        war.getNextActionOutcome(i) = ActionOutcome();
        war.setPlayerReady(i, false);
        war.setNextAction(i, -1);
        war.setActionCompleted(i, true);
        war.setActionSent(i, false);
        war.setActionReceived(i, false);
    }

    if (war.getSystemAction() == war.START_FIRST_BATTLE) {
        notify(INITIALIZE_PLAYER, war.getPlayer1());
        notify(INITIALIZE_PLAYER, war.getPlayer2());
        war.setBattleWinner(0);
        war.setBattleLoser(0);
        war.setBattleClosure(war.NO_CLOSURE_YET);

        sysActionQueue.clear();

        CPlayer::ID idFirst = war.getActorID();
        CPlayer::ID idSecond = war.getPlayer(idFirst)->get<CPlayer>()->enemy->get<CPlayer>()->id;

        if (war.getMultiplayer()) {
            if (!war.getRemotelyControled(idFirst))
                addSystemAction(war.ASSIGN_RANDOM_ARMY, idFirst);
            else
                addSystemAction(war.ASSIGN_RANDOM_ARMY, idSecond);
        } else {
            addSystemAction(war.ASSIGN_RANDOM_ARMY, idFirst);
            addSystemAction(war.ASSIGN_RANDOM_ARMY, idSecond);
        }

        addSystemAction(war.BEGINING);
        addSystemAction(war.PRESENT_ARMIES);

        if (war.getMultiplayer()) {
            if (!war.getRemotelyControled(idFirst))
                addSystemAction(war.ASK_CAPTAIN_SELECTION, idFirst);
            else
                addSystemAction(war.ASK_CAPTAIN_SELECTION, idSecond);
        } else {
            addSystemAction(war.ASK_CAPTAIN_SELECTION, idFirst);
            addSystemAction(war.ASK_CAPTAIN_SELECTION, idSecond);
        }
        addSystemAction(war.PRESENT_HEROES);

        if (war.getMultiplayer()) {
            if (!war.getRemotelyControled(idFirst))
                addSystemAction(war.ASK_FORMATION, idFirst);
            else
                addSystemAction(war.ASK_FORMATION, idSecond);
        } else {
            addSystemAction(war.ASK_FORMATION, idFirst);
            addSystemAction(war.ASK_FORMATION, idSecond);
        }
        addSystemAction(war.SET_FORMATION_EFFECT, idFirst);
        addSystemAction(war.SET_FORMATION_EFFECT, idSecond);
        addSystemAction(war.ADVANCE_ARMIES);
        addSystemAction(war.COIN, -1);

        addSystemAction(war.UPDATE_BATTLE_QUEUE, -1);
    } else if (war.getSystemAction() == war.UPDATE_BATTLE_QUEUE) {
        sysActionQueue.clear();

        CPlayer::ID idFirst = war.getActorID();
        CPlayer::ID idSecond = war.getPlayer(idFirst)->get<CPlayer>()->enemy->get<CPlayer>()->id;
        int nActions = war.getMatchConfig().nTurns;
        for(int i = 0; i < nActions; i++) {
            addSystemAction(war.ASK_CAPTAIN_ACTION, idFirst);
            addSystemAction(war.ASK_ARMY_ACTION, idFirst);

            addSystemAction(war.BETWEEN_TURNS);

            addSystemAction(war.ASK_CAPTAIN_ACTION, idSecond);
            addSystemAction(war.ASK_ARMY_ACTION, idSecond);

            addSystemAction(war.BETWEEN_TURNS);

            addSystemAction(war.ADVANCE_ARMIES);
        }
        addSystemAction(war.ASK_BATTLE_CLOSURE);

    } else if (war.getSystemAction() == war.ASK_CAPTAIN_ACTION) {
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    } else if (war.getSystemAction() == war.ASK_ARMY_ACTION) {
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    } else if (war.getSystemAction() == war.PLAY_INITIAL_INTIMIDATION) {
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setNextAction(war.getActor()->get<CPlayer>()->enemy->get<CPlayer>()->id, 202);
        war.setActionCompleted(war.getActor()->get<CPlayer>()->enemy->get<CPlayer>()->id, false);

    } else if (war.getSystemAction() == war.ASK_ARMY_ASSEMBLE) {
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    } else if (war.getSystemAction() == war.ASSIGN_RANDOM_ARMY) {

        if (war.getMultiplayer()) {
            war.setNextAction(war.getActorID(), 511);
            war.setActionCompleted(1, false);
            war.setActionCompleted(2, false);
            //war.setActionCompleted(2, false);
        } else {
            war.setNextAction(war.getActorID(), 511);
            war.setActionCompleted(war.getActorID(), false);
        }

    } else if (war.getSystemAction() == war.ASK_CAPTAIN_SELECTION) {
        /*
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        */
        if (war.getMultiplayer()) {
            war.setActionCompleted(1, false);
            war.setActionCompleted(2, false);
        } else {
            war.setActionCompleted(war.getActorID(), false);
        }

    } else if (war.getSystemAction() == war.ASK_FORMATION) {
        /*
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);
        */
        if (war.getMultiplayer()) {
            war.setActionCompleted(1, false);
            war.setActionCompleted(2, false);
        } else {
            war.setActionCompleted(war.getActorID(), false);
        }

    } else if (war.getSystemAction() == war.ASK_BATTLE_CLOSURE) {
        CPlayer::ID idPlayer;
        if (war.getPlayer(1)->get<CArmy>()->nAlive > war.getPlayer(2)->get<CArmy>()->nAlive) {
            idPlayer = 1;
        } else if (war.getPlayer(1)->get<CArmy>()->nAlive < war.getPlayer(2)->get<CArmy>()->nAlive) {
            idPlayer = 2;
        } else {
            idPlayer = 0;
        }
        war.setActorID(idPlayer);
        if (idPlayer == 0) {
            war.setNextAction(idPlayer, 400);
        } else {
            war.setNextAction(idPlayer, -1);
        }
        war.setActionCompleted(idPlayer, false);

    } else if (war.getSystemAction() == war.PRESENT_HEROES) {

    } else if (war.getSystemAction() == war.COIN) {
        war.setNextAction(0, 512);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, false);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);

    } else if (war.getSystemAction() == war.PRESENT_ARMIES) {
        
    } else if(war.getSystemAction() == war.ASK_HERO_PICK) {
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    } else if(war.getSystemAction() == war.END_HERO_POOL) {
        war.setNextAction(0, 513);
        war.setActionCompleted(0, false);

    } else if(war.getSystemAction() == war.SHOW_HERO_POOL) {
        war.setNextAction(0, 510);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, false);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);

    } else if (war.getSystemAction() == war.UPDATE_SCORE) {
        //printf("update score war\n");

    } else if (war.getSystemAction() == war.CLEAR_BATTLE_FIELD) {
        war.setNextAction(0, 514);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, false);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);

    } else if (war.getSystemAction() == war.ANNOUNCE_VICTORY) {
        inBattle = false;
        war.setNextAction(0, 515);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, false);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        hasMatchEnded = true;

    } else if (war.getSystemAction() == war.ENDING) {
        inBattle = false;

    } else if (war.getSystemAction() == war.BEGINING) {
        inBattle = true;
        war.setBattleClosure(war.NO_CLOSURE_YET);

    } else {

    }
    /*
        for(int i = 0; i < 3; i++){
            if (war.hasPendingAction(i)){
                war.getNextActionOutcome(i) = war.pullPendingAction(i);
                war.setPlayerReady(i, true);
                war.setNextAction(i, war.getNextActionOutcome(i).action);
                war.setActionCompleted(i, false);
                war.setActionSent(i, true);
                war.setActionReceived(i, true);

                Entity* eSend = eManager->createEntity();
                eSend->add(new CPacket());
                eSend->get<CPacket>()->packet << sf::String("ACTION-RECEIVED");
                eSend->get<CPacket>()->packet << sf::Int32(i);
                notify(SEND_PACKET, eSend);
            }
        }
    */
}

void WarSystem::startBattleActionQueue() {
    sysActionQueue.clear();

    CPlayer::ID idFirst = war.getFirstMover();
    CPlayer::ID idSecond = war.getFirstMover() == 1 ? 2 : 1;

    addSystemAction(war.UPDATE_SCORE);
    addSystemAction(war.CLEAR_BATTLE_FIELD);
    addSystemAction(war.ENDING);

    addSystemAction(war.RECOMPOSE_ARMY, war.getBattleWinner());
    addSystemAction(war.ASK_ARMY_ASSEMBLE, war.getBattleLoser());


    addSystemAction(war.BEGINING);
    addSystemAction(war.PRESENT_ARMIES);

    if (war.getMultiplayer()) {
        if (!war.getRemotelyControled(idFirst))
            addSystemAction(war.ASK_CAPTAIN_SELECTION, idFirst);
        else
            addSystemAction(war.ASK_CAPTAIN_SELECTION, idSecond);
    } else {
        addSystemAction(war.ASK_CAPTAIN_SELECTION, idFirst);
        addSystemAction(war.ASK_CAPTAIN_SELECTION, idSecond);
    }
    addSystemAction(war.PRESENT_HEROES);


    if (war.getMultiplayer()) {
        if (!war.getRemotelyControled(idFirst))
            addSystemAction(war.ASK_FORMATION, idFirst);
        else
            addSystemAction(war.ASK_FORMATION, idSecond);
    } else {
        addSystemAction(war.ASK_FORMATION, idFirst);
        addSystemAction(war.ASK_FORMATION, idSecond);
    }
    addSystemAction(war.SET_FORMATION_EFFECT, idFirst);
    addSystemAction(war.SET_FORMATION_EFFECT, idSecond);
    addSystemAction(war.ADVANCE_ARMIES);

    int nActions = war.getMatchConfig().nTurns;
    for(int i = 0; i < nActions; i++) {
        addSystemAction(war.ASK_CAPTAIN_ACTION, idFirst);
        addSystemAction(war.ASK_ARMY_ACTION, idFirst);

        addSystemAction(war.BETWEEN_TURNS);

        addSystemAction(war.ASK_CAPTAIN_ACTION, idSecond);
        addSystemAction(war.ASK_ARMY_ACTION, idSecond);

        addSystemAction(war.BETWEEN_TURNS);

        addSystemAction(war.ADVANCE_ARMIES);
    }
    addSystemAction(war.ASK_BATTLE_CLOSURE);
}

void WarSystem::checkBattleClosure() {
    if (!inBattle) return;
    if (war.getBattleClosure() == war.NO_CLOSURE_YET) {
        if (war.getPlayer(1)->get<CArmy>()->nAlive == 0 && war.getPlayer(2)->get<CArmy>()->nAlive == 0) {
            war.setBattleClosure(war.DRAW);
        } else if (war.getPlayer(1)->get<CArmy>()->nAlive == 0) {
            war.setBattleClosure(war.ALL_KILLED);
            war.setFirstMover(1);
            war.setBattleWinner(2);
            war.setBattleLoser(1);
        } else if (war.getPlayer(2)->get<CArmy>()->nAlive == 0) {
            war.setBattleClosure(war.ALL_KILLED);
            war.setFirstMover(2);
            war.setBattleWinner(1);
            war.setBattleLoser(2);
        }
    }
    if (war.getBattleClosure() != war.NO_CLOSURE_YET) {
        if (war.getPlayer(war.getBattleWinner())) {
            if (war.getPlayer(war.getBattleWinner())->get<CArmy>()->score >= war.getMatchConfig().streakToWin) {
                sysActionQueue.clear();
                addSystemAction(war.ANNOUNCE_VICTORY);
            }
        }
        if (war.getSystemAction() != war.UPDATE_SCORE && war.getSystemAction() != war.CLEAR_BATTLE_FIELD) startBattleActionQueue();
    }
}

void WarSystem::onPacketReceived(Entity* e) {
    sf::Packet packet = e->get<CPacket>()->packet;
    packetsQueue.push_back(packet);
}

void WarSystem::processPackets() {
    if (packetsQueue.empty()) return;
    sf::Packet packet = packetsQueue.front();
    sf::String id;
    packet >> id;
    if (id == "ACTION") {
        ActionOutcome action;
        sf::Int32 idActor;
        packet >> idActor;
        packet >> action;
        war.addPendingAction(idActor, action);
        //printf("received: player %d action %d\n", idActor, war.getNextActionOutcome(idActor).action);
        for (auto& p : war.getNextActionOutcome(idActor).armyComposition) {
            std::cout << p.first << std::endl;
        }
    } else if (id == "ACTION-RECEIVED") {
        sf::Int32 idActor;
        packet >> idActor;
        war.setActionReceived(idActor, true);
        //printf("remote client received %d's action\n", idActor);
    }

    packetsQueue.pop_front();
}

void WarSystem::onEndMatch(Entity* e) {
    war = War();
    hasMatchEnded = true;
    inBattle = false;
}

void WarSystem::onSetMatchConfig(Entity* e) {
    war.setMatchConfig(e->get<CStringMessage>()->value);
}

