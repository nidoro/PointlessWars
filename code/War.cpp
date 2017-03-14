#include "War.h"

War::War() {
    P1 = nullptr;
    P2 = nullptr;
    isPlayingAnimation = false;

    isP1ready = true;
    isP2ready = true;
    isSystemReady = true;

    isMultiplayer = false;
    didPeerReceiveState = false;

    stateRemotelyControled = false;
    P1remotelyControled = false;
    P2remotelyControled = false;

    P1actionReceived = true;
    P2actionReceived = true;
    sysActionReceived = true;

    P1actionSent = true;
    P2actionSent = true;
    sysActionSent = true;

    playing = false;

    pendingActions.resize(3);
    sysAction = NONE;
    
    nicknameP1 = "";
    nicknameP2 = "";
}

void War::setMatchConfig(string config) {
    matchConfig.loadFromFile(config);
}
void War::setMatchConfig(MatchConfig& config) {
    matchConfig = config;
}
void War::shutdown() {

}
MatchConfig& War::getMatchConfig() {
    return matchConfig;
}
bool War::getPaused() {
    return isPaused;
}
void War::setPaused(bool p) {
    isPaused = p;
}
void War::setPlayers(Entity* P1, Entity* P2) {
    this->P1 = P1;
    this->P2 = P2;
}
Entity* War::getPlayer1() {
    return P1;
}
Entity* War::getPlayer2() {
    return P2;
}
Entity* War::getPlayer(int id) {
    if (P1 && P1->get<CPlayer>()->id == id) return P1;
    if (P2 && P2->get<CPlayer>()->id == id) return P2;
    return nullptr;
}

bool War::getPlayingAnimation() {
    return isPlayingAnimation;
}

void War::setPlayingAnimation(bool playing) {
    isPlayingAnimation = playing;
}

CPlayer::ID War::getActorID() {
    return idActor;
}
void War::setActorID(CPlayer::ID id) {
    idActor = id;
}
War::SystemAction War::getSystemAction() {
    return sysAction;
}
void War::setSystemAction(SystemAction s) {
    sysAction = s;
}
Entity* War::getActor() {
    return getPlayer(idActor);
}

ActionOutcome& War::getNextActionOutcome(CPlayer::ID id) {
    if (id == 1) return P1nextOutcome;
    if (id == 2) return P2nextOutcome;
    return nextOutcome;
}

void War::setNextAction(CAction::ID id) {
    nextOutcome = ActionOutcome();
    nextOutcome.action = id;
    nextOutcome.ready = false;
}
void War::setNextAction(CPlayer::ID player, CAction::ID id) {
    getNextActionOutcome(player).action = id;
    getNextActionOutcome(player).ready = false;
}

bool War::getPlayerReady(CPlayer::ID id) {
    if (id == 1) return isP1ready;
    if (id == 2) return isP2ready;
    return isSystemReady;
}

void War::setPlayerReady(CPlayer::ID id, bool b) {
    if (id == 1) isP1ready = b;
    else if (id == 2) isP2ready = b;
    else isSystemReady = b;
}

bool War::getPlayersReady() {
    return isP1ready && isP2ready;
}

bool War::getSystemReady() {
    return isSystemReady;
}

bool War::getPeerReady() {
    //return peerReady;
    return false;
}

void War::setPeerReady(bool b) {
    //isPeerReady = b;
}

void War::setSystemReady(bool b) {
    isSystemReady = b;
}

bool War::getActionCompleted() {
    return actionP1Completed && actionP2Completed && actionSysCompleted;
}
bool War::getActionCompleted(CPlayer::ID id) {
    if (id == 1) return actionP1Completed;
    else if (id == 2) return actionP2Completed;
    else return actionSysCompleted;
}

void War::setActionCompleted(CPlayer::ID id, bool c) {
    if (id == 1) actionP1Completed = c;
    else if (id == 2) actionP2Completed = c;
    else actionSysCompleted = c;
}

Entity* War::getLocalPlayer() {
    if (!P1->has(Component::REMOTE_CONTROLER)) return P1;
    if (!P2->has(Component::REMOTE_CONTROLER)) return P2;
    return nullptr;
}

bool War::getPeerReceivedState() {
    return didPeerReceiveState;
}

void War::setPeerReceivedState(bool b) {
    didPeerReceiveState = true;
}

bool War::getMultiplayer() {
    return isMultiplayer;
}

void War::setMultiplayer(bool b) {
    isMultiplayer = b;
}

void War::setRemotelyControled(CPlayer::ID id, bool b) {
    if (id == 1) P1remotelyControled = b;
    else if (id == 2) P2remotelyControled = b;
    else stateRemotelyControled = b;
}

bool War::getRemotelyControled(CPlayer::ID id) {
    if (id == 1) return P1remotelyControled;
    else if (id == 2) return P2remotelyControled;
    else return stateRemotelyControled;
}
bool War::getActionReceived(CPlayer::ID id) {
    if (id == 1) return P1actionReceived;
    else if (id == 2) return P2actionReceived;
    else return sysActionReceived;
}
void War::setActionReceived(CPlayer::ID id, bool b) {
    if (id == 1) P1actionReceived = b;
    else if (id == 2) P2actionReceived = b;
    else sysActionReceived = b;
}
bool War::getActionSent(CPlayer::ID id) {
    if (id == 1) return P1actionSent;
    else if (id == 2) return P2actionSent;
    else return sysActionSent;
}
void War::setActionSent(CPlayer::ID id, bool b) {
    if (id == 1) P1actionSent = b;
    else if (id == 2) P2actionSent = b;
    else sysActionSent = b;
}
bool War::getPlaying() {
    return playing;
}
void War::setPlaying(bool b) {
    playing = b;
}
void War::setBattleWinner(CPlayer::ID id) {
    idWinner = id;
}
void War::setBattleLoser(CPlayer::ID id) {
    idLoser = id;
}
CPlayer::ID War::getBattleWinner() {
    return idWinner;
}
CPlayer::ID War::getBattleLoser() {
    return idLoser;
}
void War::setBattleClosure(BattleClosure closure) {
    battleClosure = closure;
}
War::BattleClosure War::getBattleClosure() {
    return battleClosure;
}

CPlayer::ID War::getFirstMover() {
    return idFirstMover;
}
void War::setFirstMover(CPlayer::ID id) {
    idFirstMover = id;
}

void War::addPendingAction(CPlayer::ID id, ActionOutcome action) {
    pendingActions[id].push_back(action);
}

ActionOutcome War::pullPendingAction(CPlayer::ID id) {
    ActionOutcome action = pendingActions[id].front();
    pendingActions[id].pop_front();
    return action;
}

bool War::hasPendingAction(CPlayer::ID id) {
    return !pendingActions[id].empty();
}

