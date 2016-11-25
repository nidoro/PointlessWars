#include "WarSystem.h"

WarSystem::WarSystem(){
    addSubscription(INITIALIZE_WAR);
    addSubscription(PACKET_RECEIVED);
    addSubscription(END_MATCH);
    addSubscription(SET_MATCH_CONFIG);
    inBattle = false;
    hasMatchEnded = true;
}

WarSystem::~WarSystem(){
    //dtor
}

void WarSystem::update(){
    if (!hasMatchEnded){
        //if (!war.getRemotelyControled(0)){
            if (war.getActionCompleted() && !war.getPlayingAnimation()){
                callNextSystemAction();
                initializeState();
                notify(SYSTEM_ACTION);
                //sendStateToPeer();
            }
        //}

        for (int i = 0; i < 3; i++){
            if (war.getMultiplayer() && !war.getRemotelyControled(i) && !war.getActionSent(i) && war.getNextActionOutcome(i).ready){
                sendAction(i);
                war.setActionSent(i, true);
            }
            if ((war.getMultiplayer() && war.getActionReceived(i) && !war.getActionCompleted(i)) || (!war.getMultiplayer() && !war.getActionCompleted(i))){
                war.setPlayerReady(i, true);
            }
        }
    }
}

void WarSystem::sendAction(CPlayer::ID id){
    sf::Packet packet;
    packet << sf::String("ACTION");
    packet << sf::Int32(id);
    packet << war.getNextActionOutcome(id);
    Entity* ePacket = eManager->createEntity();
    ePacket->add(new CPacket(packet));
    notify(SEND_PACKET, ePacket);
}

void WarSystem::onInitializeWar(Entity* e){

    war.getMatchConfig().randomArmy = false;
    war.getMatchConfig().maxRepicks = 3;

    Entity* eBtReturn = eManager->createEntity();
    eBtReturn->add(new CScreen(CScreen::MAIN_MENU, CScreen::FADE_BLACK));
    eBtReturn->add(new CButtonState());
    eBtReturn->add(new CButtonHitbox());
    eBtReturn->add(new CPosition());
    eBtReturn->add(new CButtonTrigger(OPEN_MENU, CButtonTrigger::ON_RELEASE, sf::Keyboard::Escape));

    Entity* P1 = eManager->createEntity();
    Entity* P2 = eManager->createEntity();

    P1->add(new CArmy(CArmy::LEFT));
    P2->add(new CArmy(CArmy::RIGHT));

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

    if (war.getRemotelyControled(1)){
        P1->get<CPlayer>()->side = CPlayer::RIGHT;
        P2->get<CPlayer>()->side = CPlayer::LEFT;
        P1->get<CArmy>()->side = CArmy::RIGHT;
        P2->get<CArmy>()->side = CArmy::LEFT;
        P1->get<CPlayer>()->color = RED;
        P2->get<CPlayer>()->color = BLUE;
        P1->add(new CRemotelyControled());
        P2->add(new CRemoteControler());
    }else{
        P1->get<CPlayer>()->side = CPlayer::LEFT;
        P2->get<CPlayer>()->side = CPlayer::RIGHT;
        P1->get<CArmy>()->side = CArmy::LEFT;
        P2->get<CArmy>()->side = CArmy::RIGHT;
        P1->get<CPlayer>()->color = BLUE;
        P2->get<CPlayer>()->color = RED;
        P2->add(new CRemotelyControled());
        P1->add(new CRemoteControler());
    }

    P1->get<CPlayer>()->name = "Tukimitzu";
    P2->get<CPlayer>()->name = "Geogab";

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

void WarSystem::startHeroPoolActionQueue(Entity* eFirst){
    sysActionQueue.clear();
    Entity* eSecond = eFirst->get<CPlayer>()->enemy;
    CPlayer::ID idFirst = eFirst->get<CPlayer>()->id;
    CPlayer::ID idSecond = eSecond->get<CPlayer>()->id;
    int nPicks = war.getMatchConfig().nPlayerHeroes;
    addSystemAction(War::SHOW_HERO_POOL);
    bool firstPick = true;
    int nHeroesP1 = 0;
    int nHeroesP2 = 0;
    while(nHeroesP1 < nPicks && nHeroesP2 < nPicks){
        addSystemAction(War::ASK_HERO_PICK, idFirst);
        nHeroesP1++;
        if (!firstPick && nHeroesP1 < nPicks){
            addSystemAction(War::ASK_HERO_PICK, idFirst);
            nHeroesP1++;
        }
        addSystemAction(War::ASK_HERO_PICK, idSecond);
        nHeroesP2++;
        if (nHeroesP2 < nPicks){
            addSystemAction(War::ASK_HERO_PICK, idSecond);
            nHeroesP2++;
        }
        firstPick = false;
    }
    addSystemAction(War::END_HERO_POOL);
    addSystemAction(War::START_FIRST_BATTLE, idFirst);
}

void WarSystem::addSystemAction(War::SystemAction s, CPlayer::ID p){
    sysActionQueue.push_back(make_pair(s,p));
}

void WarSystem::callNextSystemAction(){
    checkBattleClosure();
    if (!sysActionQueue.empty()){
        war.setSystemAction(sysActionQueue.front().first);
        if (sysActionQueue.front().second != -1) war.setActorID(sysActionQueue.front().second);
        sysActionQueue.pop_front();
    }else{
        //printf("empty queue???\n");
    }
}

void WarSystem::initializeState(){
    for(int i = 0; i < 3; i++){
        war.getNextActionOutcome(i) = ActionOutcome();
        war.setPlayerReady(i, false);
        war.setNextAction(i, -1);
        war.setActionCompleted(i, true);
        war.setActionSent(i, false);
        war.setActionReceived(i, false);
    }
    Entity* eActor = war.getActor();

    if (war.getSystemAction() == war.START_FIRST_BATTLE){
        notify(INITIALIZE_PLAYER, war.getPlayer1());
        notify(INITIALIZE_PLAYER, war.getPlayer2());
        war.setBattleWinner(0);
        war.setBattleLoser(0);
        war.setBattleClosure(war.NO_CLOSURE_YET);

        sysActionQueue.clear();

        CPlayer::ID idFirst = war.getActorID();
        CPlayer::ID idSecond = war.getPlayer(idFirst)->get<CPlayer>()->enemy->get<CPlayer>()->id;

        notify(ASSIGN_RANDOM_ARMY, war.getPlayer(1));
        notify(ASSIGN_RANDOM_ARMY, war.getPlayer(2));
        //addSystemAction(war.ASK_ARMY_ASSEMBLE, idFirst);
        //addSystemAction(war.ASK_ARMY_ASSEMBLE, idSecond);

        addSystemAction(war.BEGINING);
        addSystemAction(war.PRESENT_ARMIES);

        if (war.getMultiplayer()){
            if (!war.getRemotelyControled(idFirst))
                addSystemAction(war.ASK_CAPTAIN_SELECTION, idFirst);
            else
                addSystemAction(war.ASK_CAPTAIN_SELECTION, idSecond);
        }else{
            addSystemAction(war.ASK_CAPTAIN_SELECTION, idFirst);
            addSystemAction(war.ASK_CAPTAIN_SELECTION, idSecond);
        }
        addSystemAction(war.PRESENT_HEROES);

        addSystemAction(war.ASK_FORMATION, idFirst);
        addSystemAction(war.ASK_FORMATION, idSecond);
        addSystemAction(war.ADVANCE_ARMIES);
        addSystemAction(war.COIN, -1);
/*
        addSystemAction(war.COIN, -1);
        addSystemAction(war.COIN, -1);
        addSystemAction(war.COIN, -1);
        addSystemAction(war.COIN, -1);
        addSystemAction(war.COIN, -1);
        addSystemAction(war.PLAY_INITIAL_INTIMIDATION, idFirst);
*/

        addSystemAction(war.UPDATE_BATTLE_QUEUE, -1);
    }else if (war.getSystemAction() == war.UPDATE_BATTLE_QUEUE){
        sysActionQueue.clear();

        CPlayer::ID idFirst = war.getActorID();
        CPlayer::ID idSecond = war.getPlayer(idFirst)->get<CPlayer>()->enemy->get<CPlayer>()->id;
        int nActions = war.getMatchConfig().nTurns;
        for(int i = 0; i < nActions; i++){
            addSystemAction(war.ASK_CAPTAIN_ACTION, idFirst);
            addSystemAction(war.ASK_ARMY_ACTION, idFirst);

            addSystemAction(war.BETWEEN_TURNS);

            addSystemAction(war.ASK_CAPTAIN_ACTION, idSecond);
            addSystemAction(war.ASK_ARMY_ACTION, idSecond);

            addSystemAction(war.BETWEEN_TURNS);

            addSystemAction(war.ADVANCE_ARMIES);
        }
        addSystemAction(war.ASK_BATTLE_CLOSURE);

    }else if (war.getSystemAction() == war.ASK_CAPTAIN_ACTION){
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    }else if (war.getSystemAction() == war.ASK_ARMY_ACTION){
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    }else if (war.getSystemAction() == war.PLAY_INITIAL_INTIMIDATION){
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setNextAction(war.getActor()->get<CPlayer>()->enemy->get<CPlayer>()->id, 202);
        war.setActionCompleted(war.getActor()->get<CPlayer>()->enemy->get<CPlayer>()->id, false);

    }else if (war.getSystemAction() == war.ASK_ARMY_ASSEMBLE){
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    }else if (war.getSystemAction() == war.ASK_CAPTAIN_SELECTION){
        /*
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        */
        if (war.getMultiplayer()){
            war.setActionCompleted(1, false);
            war.setActionCompleted(2, false);
        }else{
            war.setActionCompleted(war.getActorID(), false);
        }

    }else if (war.getSystemAction() == war.ASK_FORMATION){
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    }else if (war.getSystemAction() == war.ASK_BATTLE_CLOSURE){
        CPlayer::ID idPlayer;
        if (war.getPlayer(1)->get<CArmy>()->nAlive > war.getPlayer(2)->get<CArmy>()->nAlive){
            idPlayer = 1;
        }else if (war.getPlayer(1)->get<CArmy>()->nAlive < war.getPlayer(2)->get<CArmy>()->nAlive){
            idPlayer = 2;
        }else{
            idPlayer = 0;
        }
        war.setActorID(idPlayer);
        if (idPlayer == 0){
            war.setNextAction(idPlayer, 400);
        }else{
            war.setNextAction(idPlayer, -1);
        }
        war.setActionCompleted(idPlayer, false);

    }else if (war.getSystemAction() == war.PRESENT_HEROES){

    }else if (war.getSystemAction() == war.COIN){
        war.setNextAction(0, 512);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, false);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);

    }else if (war.getSystemAction() == war.PRESENT_ARMIES){
        //gameObserver.isPlayingAnimation = true;
    }else if(war.getSystemAction() == war.ASK_HERO_PICK){
        war.setNextAction(0, -1);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, true);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        war.setActionCompleted(war.getActorID(), false);

    }else if(war.getSystemAction() == war.END_HERO_POOL){
        war.setNextAction(0, 513);
        war.setActionCompleted(0, false);

    }else if(war.getSystemAction() == war.SHOW_HERO_POOL){
        war.setNextAction(0, 510);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, false);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);

    }else if (war.getSystemAction() == war.UPDATE_SCORE){
        //printf("update score war\n");

    }else if (war.getSystemAction() == war.CLEAR_BATTLE_FIELD){
        war.setNextAction(0, 514);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, false);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);

    }else if (war.getSystemAction() == war.ANNOUNCE_VICTORY){
        inBattle = false;
        war.setNextAction(0, 515);
        war.setNextAction(1, -1);
        war.setNextAction(2, -1);
        war.setActionCompleted(0, false);
        war.setActionCompleted(1, true);
        war.setActionCompleted(2, true);
        hasMatchEnded = true;

    }else if (war.getSystemAction() == war.ENDING){
        inBattle = false;

    }else if (war.getSystemAction() == war.BEGINING){
        inBattle = true;
        war.setBattleClosure(war.NO_CLOSURE_YET);

    }else{

    }
}

void WarSystem::startBattleActionQueue(){
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
    addSystemAction(war.ASK_CAPTAIN_SELECTION, idFirst);
    addSystemAction(war.ASK_CAPTAIN_SELECTION, idSecond);
    addSystemAction(war.PRESENT_HEROES);

    addSystemAction(war.ASK_FORMATION, idFirst);
    addSystemAction(war.ASK_FORMATION, idSecond);
    addSystemAction(war.ADVANCE_ARMIES);

    int nActions = war.getMatchConfig().nTurns;
    for(int i = 0; i < nActions; i++){
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

void WarSystem::checkBattleClosure(){
    if (!inBattle) return;
    if (war.getBattleClosure() == war.NO_CLOSURE_YET){
        if (war.getPlayer(1)->get<CArmy>()->nAlive == 0 && war.getPlayer(2)->get<CArmy>()->nAlive == 0){
            war.setBattleClosure(war.DRAW);
        }else if (war.getPlayer(1)->get<CArmy>()->nAlive == 0){
            war.setBattleClosure(war.ALL_KILLED);
            war.setFirstMover(1);
            war.setBattleWinner(2);
            war.setBattleLoser(1);
        }else if (war.getPlayer(2)->get<CArmy>()->nAlive == 0){
            war.setBattleClosure(war.ALL_KILLED);
            war.setFirstMover(2);
            war.setBattleWinner(1);
            war.setBattleLoser(2);
        }
    }
    if (war.getBattleClosure() != war.NO_CLOSURE_YET){
        if (war.getPlayer(war.getBattleWinner())){
            if (war.getPlayer(war.getBattleWinner())->get<CArmy>()->score >= war.getMatchConfig().streakToWin){
                sysActionQueue.clear();
                addSystemAction(war.ANNOUNCE_VICTORY);
            }
        }
        if (war.getSystemAction() != war.UPDATE_SCORE && war.getSystemAction() != war.CLEAR_BATTLE_FIELD) startBattleActionQueue();
    }
}

void WarSystem::onPacketReceived(Entity* e){
    sf::Packet& packet = e->get<CPacket>()->packet;
    sf::String id;
    packet >> id;
    if (id == "ACTION"){
        sf::Int32 idActor;
        packet >> idActor;
        packet >> war.getNextActionOutcome(idActor);

        Entity* eSend = eManager->createEntity();
        eSend->add(new CPacket());
        eSend->get<CPacket>()->packet << sf::String("ACTION-RECEIVED");
        eSend->get<CPacket>()->packet << sf::Int32(idActor);
        notify(SEND_PACKET, eSend);
        war.setActionReceived(idActor, true);
        printf("received: player %d action %d\n", idActor, war.getNextActionOutcome(idActor).action);

    }else if (id == "ACTION-RECEIVED"){
        sf::Int32 idActor;
        packet >> idActor;
        war.setActionReceived(idActor, true);
    }
}

void WarSystem::onEndMatch(Entity* e){
    war = War();
}

void WarSystem::onSetMatchConfig(Entity* e){
    war.setMatchConfig(e->get<CStringMessage>()->value);
}

/*
void WarSystem::onEndCaptainsPoolPhase(Entity* e){
    for(EntityListIt i = captainPool.begin(); i != captainPool.end(); i++){
        eManager->removeEntity(*i);
    }
    captainPool.clear();

    Entity* P1;
    Entity* P2;

    if (e->get<CArmy>()->side == CArmy::LEFT){
        P1 = e;
        P2 = e->get<CPlayer>()->enemy;
    }else{
        P1 = e->get<CPlayer>()->enemy;
        P2 = e;
    }

    notify(INITIALIZE_PLAYER, P1);
    notify(INITIALIZE_PLAYER, P2);

    Entity* eBattle = eManager->createEntity();
    eBattle->add(new CBattle(P1, P2, e));

    notify(START_NEW_BATTLE, eBattle);

    double x = cxWindow - 30/2;
    double y = 45;

    for(int i = 0; i < CUnit::N_DAMAGE_TYPES; i+=2){
        Entity* eIcon = eManager->createEntity();
        eIcon->add(new CPosition(x,y));
        eIcon->add(new CDraw(CDraw::GUI1));
        eIcon->add(new CTexture("type-" + int2str(i,2) + "-icon.png"));
        eIcon->add(new CDimensions(30, 30));
        eIcon->add(new CButtonState());
        eIcon->add(new CButtonHitbox(30,30));
        eIcon->add(new CButtonTrigger());
        eIcon->add(new CHighlightTrigger(CUnitHighlight2::RESISTANCE, i));
        eIcon->add(new COwner(P1));
        eIcon->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
        eIcon->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;
        y += 30;
    }

    x = cxWindow + 30/2;
    y = 45;

    for(int i = 1; i < CUnit::N_DAMAGE_TYPES; i+=2){
        Entity* eIcon = eManager->createEntity();
        eIcon->add(new CPosition(x,y));
        eIcon->add(new CDraw(CDraw::GUI1));
        eIcon->add(new CTexture("type-" + int2str(i,2) + "-icon.png"));
        eIcon->add(new CDimensions(30, 30));
        eIcon->add(new CButtonState());
        eIcon->add(new CButtonHitbox(30,30));
        eIcon->add(new CButtonTrigger());
        eIcon->add(new CHighlightTrigger(CUnitHighlight2::RESISTANCE, i));
        eIcon->add(new COwner(P1));
        eIcon->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
        eIcon->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;
        y += 30;
    }

}

void WarSystem::showCaptainsPool(Entity* e){
    captainPool.clear();

    list<CCaptain::ID> options = match.heroPool;
    double spacing = 70;
    double x = cxWindow - (options.size()-1)*spacing/2;
    double y = cyWindow - spacing;

    for (list<CCaptain::ID>::iterator i = options.begin(); i != options.end(); i++){
        CCaptain cap = CCaptain::Map[*i];
        Entity* eOption = eManager->createEntity();
        eOption->add(new CPosition(x,y));
        eOption->add(new CTexture());
        eOption->add(new CDimensions(60, 60));
        eOption->add(new CDefaultTexture(cap.portrait));
        eOption->add(new CHoverTexture(cap.portrait));
        eOption->add(new CActiveTexture(cap.portrait));
        eOption->add(new CDraw(CDraw::GUI));
        eOption->add(new CButtonState());
        eOption->add(new CButtonHitbox(60, 60));
        eOption->add(new CButtonTrigger(TAKE_CAPTAIN));
        eOption->add(new CCaptain(cap));
        eOption->add(new CTooltip(CTooltip::CAPTAIN));
        captainPool.push_back(eOption);
        x += spacing;
    }

    eChooser = e;

    eTurnDisplayer = eManager->createEntity();
    eTurnDisplayer->add(new CPosition(cxWindow, 60));
    eTurnDisplayer->add(new CDraw(CDraw::GUI1));
    eTurnDisplayer->add(new CTextbox2(e->get<CPlayer>()->name + " is choosing a captain", Assets::getFont(Assets::getPrimaryFont()), 14));
    eTurnDisplayer->add(new CRectShape(280, 20, sf::Color::White, 1));
}

StateMachine::StateMachine(){
    addRequirement(Component::BATTLE);
    addSubscription(START_NEW_BATTLE);
    addSubscription(SCENE_ENDED);
    addSubscription(SCENE_STARTED);
    addSubscription(ACTION_SELECTED);
    addSubscription(ANNOUNCE_VICTORY);
    addSubscription(OPEN_MENU);
    addSubscription(RESUME_GAME);
    eBattle = nullptr;
    isPaused = false;
    nextStateOnUnpause = false;
}

StateMachine::~StateMachine(){

}

void StateMachine::update(){
    if (!eBattle) return;
}

void StateMachine::onStartNewBattle(Entity* e){
    e->get<CBattle>()->P1->get<CPlayer>()->side = CPlayer::LEFT;
    e->get<CBattle>()->P2->get<CPlayer>()->side = CPlayer::RIGHT;
/*
    notify(ASSIGN_RANDOM_ARMY, e->get<CBattle>()->P1);
    notify(ASSIGN_RANDOM_ARMY, e->get<CBattle>()->P2);
    eBattle = e;
*/
/*
    Entity* eStateDisp = eManager->createEntity();
    eStateDisp->add(new CTexture("banner.png"));
    eStateDisp->add(new CPosition(cxWindow, 20));
    eStateDisp->add(new CDraw(CDraw::GUI));
    eStateDisp->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14));
    eStateDisp->add(new CDisplayer(CDisplayer::BATTLE_STATE_DESCRIPTION, eBattle));

    Entity* eObj = eManager->createEntity();
    eObj->add(new CRectShape(wWindow, hWindow, sf::Color::Black));
    eObj->add(new CDraw(CDraw::SKY, 240));
    eObj->add(new CPosition(cxWindow, cyWindow));

    restartStateQueue(true);
    nextState();
}

void StateMachine::nextState(){
    if (isPaused){
        nextStateOnUnpause = true;
        return;
    }
    Entity* eWin = getWinner();
    if (eWin){
        if (eWin == eBattle->get<CBattle>()->P1){
            eBattle->get<CBattle>()->P1streak++;
            eBattle->get<CBattle>()->P2streak = 0;
            eBattle->get<CBattle>()->P1->get<CArmy>()->score = eBattle->get<CBattle>()->P1streak;
            eBattle->get<CBattle>()->P2->get<CArmy>()->score = eBattle->get<CBattle>()->P2streak;
            if (eBattle->get<CBattle>()->P1streak >= war.getMatchConfig().streakToWin){
                notify(ANNOUNCE_VICTORY, eWin);
                return;
            }
        }else if (eWin == eBattle->get<CBattle>()->P2){
            eBattle->get<CBattle>()->P2streak++;
            eBattle->get<CBattle>()->P1streak = 0;
            eBattle->get<CBattle>()->P1->get<CArmy>()->score = eBattle->get<CBattle>()->P1streak;
            eBattle->get<CBattle>()->P2->get<CArmy>()->score = eBattle->get<CBattle>()->P2streak;
            if (eBattle->get<CBattle>()->P2streak >= war.getMatchConfig().streakToWin){
                notify(ANNOUNCE_VICTORY, eWin);
                return;
            }
        }
        stateQueue.clear();
        stateQueue.push_back(BattleState(BattleState::FIELD_CLEANUP, eWin, nullptr, SCENE_ENDED));
        stateQueue.push_back(BattleState(BattleState::ENDING));
        stateQueue.push_back(BattleState(BattleState::BETWEEN_ROUNDS, eWin, eWin->get<CPlayer>()->enemy));
        stateQueue.push_back(BattleState(BattleState::BETWEEN_TURNS));
        stateQueue.push_back(BattleState(BattleState::ASK_ARMY_ASSEMBLE, eWin->get<CPlayer>()->enemy, nullptr, ACTION_SELECTED));
    }

    do{
        eBattle->get<CBattle>()->state = pollNextState();

        if (eBattle->get<CBattle>()->state.id == BattleState::ASK_BATTLE_CLOSURE){
            Entity* eP1 = eBattle->get<CBattle>()->P1;
            Entity* eP2 = eBattle->get<CBattle>()->P2;
            Entity* e = getAliveUnits(eP1).size() > getAliveUnits(eP2).size() ? eP1 : eP2;
            if (getAliveUnits(eP1).size() == getAliveUnits(eP2).size()){
                eBattle->get<CBattle>()->state = BattleState(BattleState::PLAY_ACTION, e, nullptr, SCENE_ENDED);
                e->get<CArmy>()->nextAction = 400;
            }else{
                eBattle->get<CBattle>()->state.actors.push_back(e);
                stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, e, nullptr, SCENE_ENDED));
            }
        }

        notify(START_BATTLE_STATE, eBattle);
    }while (eBattle->get<CBattle>()->state.endSignal == EMPTY_MESSAGE);
    updateStateDescription();
}


void StateMachine::restartStateQueue(bool firstBattle){
    BattleState state;
    Entity* eFirst = eBattle->get<CBattle>()->eFirst;
    Entity* eSecon = eFirst->get<CPlayer>()->enemy;

    stateQueue.clear();

    if (firstBattle){
        stateQueue.push_back(BattleState(BattleState::ASK_ARMY_ASSEMBLE, eFirst, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::ASK_ARMY_ASSEMBLE, eSecon, nullptr, ACTION_SELECTED));

        stateQueue.push_back(BattleState(BattleState::BEGINING));
        stateQueue.push_back(BattleState(BattleState::PRESENT_ARMY, eFirst, eSecon, SCENE_ENDED));
        stateQueue.push_back(BattleState(BattleState::ASK_CAPTAIN_SELECTION, eFirst, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::ASK_CAPTAIN_SELECTION, eSecon, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eFirst, eSecon, SCENE_ENDED));

        stateQueue.push_back(BattleState(BattleState::ASK_FORMATION, eFirst, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::ASK_FORMATION, eSecon, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eFirst, eSecon, SCENE_ENDED));

        stateQueue.push_back(BattleState(BattleState::COIN, nullptr, nullptr, SCENE_ENDED));
    }else{

        stateQueue.push_back(BattleState(BattleState::BEGINING));
        stateQueue.push_back(BattleState(BattleState::PRESENT_ARMY, eFirst, eSecon, SCENE_ENDED));
        stateQueue.push_back(BattleState(BattleState::ASK_CAPTAIN_SELECTION, eFirst, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::ASK_CAPTAIN_SELECTION, eSecon, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eFirst, eSecon, SCENE_ENDED));

        stateQueue.push_back(BattleState(BattleState::ASK_FORMATION, eFirst, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::ASK_FORMATION, eSecon, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eFirst, eSecon, SCENE_ENDED));

        int nActions = war.getMatchConfig().nTurns;
        for(int i = 0; i < nActions; i++){
            stateQueue.push_back(BattleState(BattleState::ASK_CAPTAIN_ACTION, eFirst, nullptr, ACTION_SELECTED));
            stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eFirst, nullptr, SCENE_ENDED));
            stateQueue.push_back(BattleState(BattleState::ASK_ARMY_ACTION, eFirst, nullptr, ACTION_SELECTED));
            stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eFirst, nullptr, SCENE_ENDED));

            stateQueue.push_back(BattleState(BattleState::BETWEEN_ROUNDS, eSecon));
            stateQueue.push_back(BattleState(BattleState::BETWEEN_TURNS));

            stateQueue.push_back(BattleState(BattleState::ASK_CAPTAIN_ACTION, eSecon, nullptr, ACTION_SELECTED));
            stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eSecon, nullptr, SCENE_ENDED));
            stateQueue.push_back(BattleState(BattleState::ASK_ARMY_ACTION, eSecon, nullptr, ACTION_SELECTED));
            stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eSecon, nullptr, SCENE_ENDED));

            stateQueue.push_back(BattleState(BattleState::BETWEEN_ROUNDS, eFirst));
            stateQueue.push_back(BattleState(BattleState::BETWEEN_TURNS));

            stateQueue.push_back(BattleState(BattleState::ADVANCE_ARMY, eFirst, eSecon, SCENE_ENDED));
        }

     //   if (!firstBattle){
        stateQueue.push_back(BattleState(BattleState::ASK_BATTLE_CLOSURE, nullptr, nullptr, ACTION_SELECTED));
     //   }else{
     //       stateQueue.push_back(CBattle::State(CBattle::PLAY_MELEE_BATTLE, eFirst));
     //   }
    }
}

void StateMachine::completeQueue(){
    BattleState state;
    Entity* eFirst = eBattle->get<CBattle>()->eFirst;
    Entity* eSecon = eFirst->get<CPlayer>()->enemy;

    stateQueue.clear();

    eSecon->get<CArmy>()->nextAction = 202;
    stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eSecon, nullptr, SCENE_ENDED));

    int nActions = war.getMatchConfig().nTurns;
    for(int i = 0; i < nActions; i++){
        stateQueue.push_back(BattleState(BattleState::ASK_CAPTAIN_ACTION, eFirst, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eFirst, nullptr, SCENE_ENDED));
        stateQueue.push_back(BattleState(BattleState::ASK_ARMY_ACTION, eFirst, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eFirst, nullptr, SCENE_ENDED));

        stateQueue.push_back(BattleState(BattleState::BETWEEN_ROUNDS, eSecon));
        stateQueue.push_back(BattleState(BattleState::BETWEEN_TURNS));

        stateQueue.push_back(BattleState(BattleState::ASK_CAPTAIN_ACTION, eSecon, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eSecon, nullptr, SCENE_ENDED));
        stateQueue.push_back(BattleState(BattleState::ASK_ARMY_ACTION, eSecon, nullptr, ACTION_SELECTED));
        stateQueue.push_back(BattleState(BattleState::PLAY_ACTION, eSecon, nullptr, SCENE_ENDED));

        stateQueue.push_back(BattleState(BattleState::BETWEEN_ROUNDS, eFirst));
        stateQueue.push_back(BattleState(BattleState::BETWEEN_TURNS));

        stateQueue.push_back(BattleState(BattleState::ADVANCE_ARMY, eFirst, eSecon, SCENE_ENDED));
    }

    stateQueue.push_back(BattleState(BattleState::ASK_BATTLE_CLOSURE, nullptr, nullptr, ACTION_SELECTED));
}

BattleState StateMachine::pollNextState(){
    if (stateQueue.empty()){
        if (eBattle->get<CBattle>()->state.id == BattleState::COIN){
            completeQueue();
        }else{
            restartStateQueue(false);
        }
    }
    BattleState state = stateQueue.front();
    stateQueue.pop_front();
    return state;
}

void StateMachine::onSceneEnded(Entity* e){
    if (eBattle->get<CBattle>()->state.endSignal == SCENE_ENDED){
        nextState();
    }
}

void StateMachine::onActionSelected(Entity* e){
    if (eBattle->get<CBattle>()->state.endSignal == ACTION_SELECTED){
        nextState();
    }
}

Entity* StateMachine::getWinner(){
    if (checkWinCondition(eBattle->get<CBattle>()->P1) && checkWinCondition(eBattle->get<CBattle>()->P2)) return nullptr;
    if (checkWinCondition(eBattle->get<CBattle>()->P1)) return eBattle->get<CBattle>()->P1;
    if (checkWinCondition(eBattle->get<CBattle>()->P2)) return eBattle->get<CBattle>()->P2;
    return nullptr;
}

bool StateMachine::checkWinCondition(Entity* e){
    if (!e->get<CArmy>()->ready) return false;
    if (e->get<CArmy>()->win) return true;
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    bool allDead = true;
    for(EntityListIt i = eEnemy->get<CArmy>()->allUnits.begin(); i != eEnemy->get<CArmy>()->allUnits.end(); i++){
        Entity* eUnit = *i;
        if (!eUnit->get<CUnit>()->dead){
            allDead = false;

        }
    }

    e->get<CArmy>()->win = allDead;

    return allDead;
}

void StateMachine::updateStateDescription(){
    BattleState state = eBattle->get<CBattle>()->state;
    switch(state.id){
        case BattleState::ASK_FORMATION:
            eBattle->get<CBattle>()->stateDescription = state.actors.front()->get<CPlayer>()->name + " is choosing formation";

        case BattleState::ASK_CAPTAIN_SELECTION:
            eBattle->get<CBattle>()->stateDescription = state.actors.front()->get<CPlayer>()->name + " is choosing captain";

        case BattleState::ASK_CAPTAIN_ACTION:
            eBattle->get<CBattle>()->stateDescription = state.actors.front()->get<CPlayer>()->name + " is choosing captain action";

        case BattleState::ASK_ARMY_ACTION:
            eBattle->get<CBattle>()->stateDescription = state.actors.front()->get<CPlayer>()->name + " is choosing army action";

        case BattleState::ASK_ARMY_ASSEMBLE:
            eBattle->get<CBattle>()->stateDescription = state.actors.front()->get<CPlayer>()->name + " is building his army";

        case BattleState::ASK_BATTLE_CLOSURE:
            eBattle->get<CBattle>()->stateDescription = state.actors.front()->get<CPlayer>()->name + " is choosing an ending";

        default: eBattle->get<CBattle>()->stateDescription = "";
    }
}

void StateMachine::onAnnounceVictory(Entity* e){
    eBattle->get<CBattle>()->stateDescription = e->get<CPlayer>()->name + " won!";
}


EntityList StateMachine::getAliveUnits(Entity* e){
    EntityList l;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++){
        Entity* eUnit = *i;
        if (!eUnit->get<CUnit>()->dead) l.push_back(eUnit);
    }
    return l;
}

void StateMachine::onOpenMenu(Entity* e){
    isPaused = true;
}

void StateMachine::onResumeGame(Entity* e){
    isPaused = false;
    if (nextStateOnUnpause) nextState();
    nextStateOnUnpause = false;
}

*/
