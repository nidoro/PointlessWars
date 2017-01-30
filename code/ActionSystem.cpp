#include "ActionSystem.h"

ActionSystem::ActionSystem() {
    addSubscription(START_BATTLE_STATE);
    //addSubscription(PLAY_ACTION);
    addSubscription(SCENE_ENDED);
    //addSubscription(ACTION_SELECTED);
    //addSubscription(SYSTEM_ACTION);
}

ActionSystem::~ActionSystem() {

}

void ActionSystem::update() {
    if (!war.getPlaying()) return;
    for(int i = 0; i < 3; i++) {
        if (!war.getNextActionOutcome(i).ready && war.getNextActionOutcome(i).action >= 0 && !war.getRemotelyControled(i)) {
            if (i > 0 && war.getPlayer(i)->has(Component::AI)) {
                if (war.getPlayer(i)->get<CAInt>()->actionSelected) {
                    ///PREPROCESS ACTION
                    preprocessAction(i);
                    ///ACTION READY TO BE PLAYED
                    war.getNextActionOutcome(i).ready = true;
                    war.getPlayer(i)->get<CAInt>()->actionSelected = false;
                }
            } else {
                ///PREPROCESS ACTION
                preprocessAction(i);
                ///ACTION READY TO BE PLAYED
                war.getNextActionOutcome(i).ready = true;
            }
        }

        ///IF ACTION IS READY TO BE PLAYED AND PLAYER IS READY TO PLAY
        if (war.getNextActionOutcome(i).ready && war.getPlayerReady(i)) {
            //printf("executing player %d action %d\n", i, war.getNextActionOutcome(i).action);
            ///EXECUTE ACTION
            executeAction(i);
            ///set action completed and player is not ready to act again until next state
            war.setActionCompleted(i, true);
            war.setPlayerReady(i, false);
        }
    }
}

void ActionSystem::preprocessAction(CPlayer::ID id) {
    ///temp dummy entity
    //Entity* e;

    switch(war.getNextActionOutcome(id).action) {
    case -1:
        doNothing(war.getPlayer(id));
        break;
    case 0:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 1:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 2:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 3:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 4:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 5:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 6:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 7:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 8:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 9:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 10:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 11:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 12:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 13:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 14:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 15:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 16:
        preprocessUnitAttack(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    ///HERO SELECTION
    case 100:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 101:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 102:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 103:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 104:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 105:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 106:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 107:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 108:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 109:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 110:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 111:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 112:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 113:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    case 114:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 100;
        break;
    ///HERO ACTIONS
    case 200:
        playEffectAction(war.getPlayer(id));
        break;
    case 201:
        playFocusFire(war.getPlayer(id));
        break;
    case 202:
        preprocessIntimidation(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 203:
        preprocessResurect(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 204:
        preprocessBuffDebuff(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 205:
        preprocessBuffDebuff(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 206:
        preprocessBuffDebuff(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 207:
        preprocessBuffDebuff(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 208:
        preprocessBuffDebuff(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 209:
        preprocessBuffDebuff(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    //case 210: playFocus(e, CUnit::FIRE); break;
    //case 211: playFocus(e, CUnit::WATER); break;
    case 212:
        preprocessChangeFormation(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 213:
        preprocessTimeDilatation(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    //case 213: playHeroAttack(e, 213); break;
    //case 214: playHeroAttack(e, 214); break;
    //case 215: playHeroAttack(e, 215); break;
    case 216:
        preprocessPurify(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 217:
        preprocessConversion(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 218:
        preprocessSummon(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 219:
        preprocessTrueForm(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 220:
        preprocessSwap(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 221:
        preprocessSomniferous(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 222:
        preprocessCurse(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 223:
        preprocessHex(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 227:
        preprocessTarot(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 228:
        preprocessTeleport(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 230:
        preprocessConfusion(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 231:
        preprocessHelp(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 232:
        preprocessBuffDebuff(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 233:
        preprocessBuffDebuff(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 235:
        preprocessSleep(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    //case 237: playHeroAttack(e, 237); break;
    case 238:
        preprocessImmortality(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 239:
        preprocessSlavesCall(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 240:
        preprocessCroak(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 241:
        preprocessStampede(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    //case 242: playBecomeHuman(e); break;
    ///FORMATION SELECTION
    case 300:
        war.getActor()->get<CArmy>()->formation = (CArmy::Formation) (war.getNextActionOutcome(id).action - 300);
        break;
    case 301:
        war.getActor()->get<CArmy>()->formation = (CArmy::Formation) (war.getNextActionOutcome(id).action - 300);
        break;
    case 302:
        war.getActor()->get<CArmy>()->formation = (CArmy::Formation) (war.getNextActionOutcome(id).action - 300);
        break;
    case 303:
        war.getActor()->get<CArmy>()->formation = (CArmy::Formation) (war.getNextActionOutcome(id).action - 300);
        break;
    case 304:
        war.getActor()->get<CArmy>()->formation = (CArmy::Formation) (war.getNextActionOutcome(id).action - 300);
        break;
    ///BATTLE CLOSURES
    case 400:
        preprocessArmyVsArmy(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 401:
        preprocessManVsMan(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 402:
        preprocessEnslave(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 403:
        preprocessMercy(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    ///SYSTEM ACTIONS
    case 500:
        playPresentArmy(war.getPlayer(id));
        break;
    case 502:
        playAdvanceArmy(war.getPlayer(id));
        break;
    case 503:
        playReturnArmy(war.getPlayer(id));
        break;
    case 504:
        playRemoveDead(war.getPlayer(id));
        break;
    case 510:
        war.getNextActionOutcome(id).heroes = war.getMatchConfig().heroPool;
        break;
    case 511:
        preprocessArmyComposition(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    case 512:
        preprocessCoinThrow(war.getNextActionOutcome(id), war.getPlayer(id));
        break;
    //case 513: preprocessRandomArmy(war.getNextActionOutcome(id), war.getPlayer(id)); break;
    ///HERO PICKS
    case 600:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 601:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 602:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 603:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 604:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 605:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 606:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 607:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 608:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 609:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 610:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 611:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    case 612:
        war.getNextActionOutcome(id).hero = (CCaptain::ID)war.getNextActionOutcome(id).action - 600;
        break;
    default:
        break;
    }
}

void ActionSystem::executeAction(CPlayer::ID id) {
    if (isWithinClosedRange(war.getNextActionOutcome(id).action, 0, 99)) {
        notify(PLAY_ACTION, war.getPlayer(id));
    } else if (isWithinClosedRange(war.getNextActionOutcome(id).action, 100, 199)) {
        //war.getPlayer(id)->get<CArmy>()->captain = war.getPlayer(id)->get<CArmy>()->captains[war.getNextActionOutcome(id).hero];
        war.getPlayer(id)->get<CArmy>()->idCaptain = war.getNextActionOutcome(id).hero;
    } else if (isWithinClosedRange(war.getNextActionOutcome(id).action, 200, 299)) {
        notify(PLAY_ACTION, war.getPlayer(id));
    } else if (isWithinClosedRange(war.getNextActionOutcome(id).action, 300, 399)) {
        notify(PLAY_ACTION, war.getPlayer(id));
    } else if (isWithinClosedRange(war.getNextActionOutcome(id).action, 400, 499)) {
        switch(war.getNextActionOutcome(id).action) {
        case 400:
            war.setBattleClosure(war.ARMY_VS_ARMY);
            war.setFirstMover(war.getNextActionOutcome(id).idLoser);
            break;
        case 401:
            war.setBattleClosure(war.MAN_VS_MAN);
            war.setFirstMover(war.getNextActionOutcome(id).idWinner);
            break;
        case 402:
            war.setBattleClosure(war.CONFINE);
            war.setFirstMover(war.getNextActionOutcome(id).idWinner);
            break;
        case 403:
            war.setBattleClosure(war.MERCY);
            war.setFirstMover(war.getNextActionOutcome(id).idWinner);
            break;
        default:
            break;
        }
        war.setBattleWinner(war.getNextActionOutcome(id).idWinner);
        war.setBattleLoser(war.getNextActionOutcome(id).idLoser);
        notify(PLAY_ACTION, war.getPlayer(id));

    } else if (isWithinClosedRange(war.getNextActionOutcome(id).action, 500, 599)) {
        switch(war.getNextActionOutcome(id).action) {
        case 510:
            notify(SHOW_HERO_POOL);
            break;
        case 511:
            createArmy(war.getPlayer(id));
            break;
        case 512:
            notify(PLAY_ACTION, war.getPlayer(id));
            break;
        case 513:
            notify(END_HERO_POOL);
            break;
        case 514:
            notify(PLAY_ACTION, war.getPlayer(id));
            break;
        case 515:
            notify(PLAY_ACTION, war.getPlayer(id));
            break;
        default:
            break;
        }
    } else if (isWithinClosedRange(war.getNextActionOutcome(id).action, 600, 699)) {
        ///HERO PICKS
        CCaptain::ID heroID = war.getNextActionOutcome(id).hero;
        war.getPlayer(id)->get<CPlayer>()->heroDeck.push_back(war.getNextActionOutcome(id).hero);
        if (war.getRemotelyControled(id) || war.getPlayer(id)->has(Component::AI)) {
            Entity* eAct = eManager->createEntity();
            eAct->add(new CAction(war.getNextActionOutcome(id).action));
            notify(SELECT_ACTION, eAct);
        }
        war.getPlayer(1)->get<CPlayer>()->heroPool.remove(heroID);
        war.getPlayer(2)->get<CPlayer>()->heroPool.remove(heroID);
        //notify(REMOVE_FROM_HERO_POOL, war.getPlayer(id));
    }
}

void ActionSystem::composeArmy(Entity* eArmy) {
    map<CUnit::ID, CUnit>& units = CUnit::Map;

    int nUnits = 0;
    for (auto& i : eArmy->get<CArmy>()->unitCount) {
        nUnits += i.second;
    }

    int nRemaining = war.getMatchConfig().armySize - nUnits;
    list<CUnit::ID>& deck = eArmy->get<CPlayer>()->unitDeck;
    for(int i = 0; i < nRemaining; i += war.getMatchConfig().recruitGroup) {
        CUnit unit = units[getRandom(deck)];

        map<CUnit::ID, int>::iterator it = eArmy->get<CArmy>()->unitCount.find(unit.id);
        if (it == eArmy->get<CArmy>()->unitCount.end()) {
            eArmy->get<CArmy>()->unitCount.insert(make_pair(unit.id, war.getMatchConfig().recruitGroup));
        } else {
            eArmy->get<CArmy>()->unitCount[unit.id] += war.getMatchConfig().recruitGroup;
        }
    }
}

void ActionSystem::createArmy(Entity* e) {
    e->get<CArmy>()->unitCount = war.getNextActionOutcome(e->get<CPlayer>()->id).armyComposition;
    e->get<CArmy>()->allUnits.clear();
    int sign = e->get<CArmy>()->side == CPlayer::LEFT ? -1:1;
    //unused:
    //double wField = 0.70*wWindow;
    //double step = wField/((war.getMatchConfig().nTurns+2)*2);
    e->get<CArmy>()->x = 0;

    bool flip = e->get<CArmy>()->side == CPlayer::LEFT ? false:true;
    double wSpawn = 180;
    double hSpawn = 30;
    double xSpawn = cxWindow + sign*wWindow/2 + sign*wSpawn;
    double ySpawn = cyWindow;

    for(map<CUnit::ID, int>::iterator i = e->get<CArmy>()->unitCount.begin(); i != e->get<CArmy>()->unitCount.end(); i++) {
        CUnit::ID id = i->first;
        int n = i->second;
        for(int c = 0; c < n; c++) {
            Entity* eUnit = eManager->createEntity();
            eUnit->add(new CUnit(CUnit::Map[id]));
            eUnit->add(new COwner(e));
            eUnit->add(new CAnimation(flip, CUnit::Map[id].aIdle));
            eUnit->add(new CDraw(CDraw::WORLD_1));
            eUnit->add(new CVelocity());
            eUnit->add(new CPosition(randomDouble(xSpawn - wSpawn/2, xSpawn + wSpawn/2),
                                     randomDouble(ySpawn - hSpawn/2, ySpawn + hSpawn/2)));
            eUnit->get<CVelocity>()->speed = 80;
            eUnit->add(new CActor());
            eUnit->add(new CUnitHighlight2(Assets::colors[id]));
            eUnit->get<CUnit>()->armyID = e->get<CArmy>()->nextUnitID++;

            e->get<CArmy>()->allUnits.push_back(eUnit);
        }
    }

    e->get<CArmy>()->lastArmy = e->get<CArmy>()->unitCount;
}

void ActionSystem::onExecuteActions(Entity* e) {
    //unused:
    //Entity* P1 = e->get<CBattle>()->P1;
    //Entity* P2 = e->get<CBattle>()->P2;
}
void ActionSystem::executeAction(Entity* e) {

}

void ActionSystem::onStartBattleState(Entity* e) {
    if (e->get<CBattle>()->state.id == BattleState::COIN) {
        //throwCoin(e);
    }

    for(EntityListIt i = e->get<CBattle>()->state.actors.begin(); i != e->get<CBattle>()->state.actors.end(); i++) {
        Entity* actor = *i;
        if (e->get<CBattle>()->state.id == BattleState::PLAY_ACTION) {
            playAction(actor);
        }
    }
}

void ActionSystem::onSystemAction(Entity* e) {
    if (war.getSystemAction() == war.PLAY_ACTIONS) {
        playAction(war.getPlayer1());
        playAction(war.getPlayer2());
    } else if (war.getSystemAction() == war.PLAY_ACTION) {
        playAction(war.getActor());
    } else if (war.getSystemAction() == war.COIN) {
        throwCoin();
    }
}

void ActionSystem::onPlayAction(Entity* e) {
    playAction(e);
}

void ActionSystem::playAction(Entity* e) {
    switch(e->get<CArmy>()->nextAction) {
    case -1:
        doNothing(e);
        break;
    case 0:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 1:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 2:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 3:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 4:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 5:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 6:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 7:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 8:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 9:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 10:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 11:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 12:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 13:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 14:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 15:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 16:
        playUnitAttack(e, e->get<CArmy>()->nextAction);
        break;
    case 100:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 101:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 102:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 103:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 104:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 105:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 106:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 107:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 108:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 109:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 110:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 111:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 112:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 113:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 114:
        e->get<CArmy>()->captain = e->get<CArmy>()->captains[e->get<CArmy>()->nextAction - 100];
        playPresentCaptain(e);
        break;
    case 200:
        playEffectAction(e);
        break;
    case 201:
        playFocusFire(e);
        break;
    case 202:
        playIntimidate(e);
        break;
    case 203:
        playResurrect(e);
        break;
    case 204:
        playBuffDebuff(e, CUnit::FIRE, true);
        break;
    case 205:
        playBuffDebuff(e, CUnit::WATER, true);
        break;
    case 206:
        playBuffDebuff(e, CUnit::EARTH, true);
        break;
    case 207:
        playBuffDebuff(e, CUnit::FIRE, false);
        break;
    case 208:
        playBuffDebuff(e, CUnit::WATER, false);
        break;
    case 209:
        playBuffDebuff(e, CUnit::EARTH, false);
        break;
    //case 210: playFocus(e, CUnit::FIRE); break;
    //case 211: playFocus(e, CUnit::WATER); break;
    case 212:
        playChangeFormation(e);
        break;
    case 213:
        playTimeDilatation(e);
        break;
    //case 213: playHeroAttack(e, 213); break;
    //case 214: playHeroAttack(e, 214); break;
    //case 215: playHeroAttack(e, 215); break;
    case 216:
        playPurify(e);
        break;
    case 217:
        playConvert(e);
        break;
    case 218:
        playSummon(e);
        break;
    case 219:
        playTrueForm(e);
        break;
    case 220:
        playSwapHeroes(e);
        break;
    case 221:
        playPoison(e);
        break;
    case 222:
        playCurse(e);
        break;
    case 223:
        playHex(e);
        break;
    case 227:
        playTarot(e);
        break;
    case 228:
        playTeleport(e);
        break;
    case 230:
        playConfusion(e);
        break;
    case 231:
        playMindControl(e);
        break;
    case 232:
        playBuffDebuff(e, CUnit::AIR, true);
        break;
    case 233:
        playBuffDebuff(e, CUnit::AIR, false);
        break;
    case 235:
        playSilence(e);
        break;
    //case 237: playHeroAttack(e, 237); break;
    case 238:
        playImmortality(e);
        break;
    case 239:
        playCallSlaves(e);
        break;
    case 240:
        playCroak(e);
        break;
    case 241:
        playStampede(e);
        break;
    //case 242: playBecomeHuman(e); break;

    case 300:
        e->get<CArmy>()->formation = (CArmy::Formation)(e->get<CArmy>()->nextAction - 300);
        break;
    case 301:
        e->get<CArmy>()->formation = (CArmy::Formation)(e->get<CArmy>()->nextAction - 300);
        break;
    case 302:
        e->get<CArmy>()->formation = (CArmy::Formation)(e->get<CArmy>()->nextAction - 300);
        break;
    case 303:
        e->get<CArmy>()->formation = (CArmy::Formation)(e->get<CArmy>()->nextAction - 300);
        break;

    case 400:
        playMeleeBattle(e);
        break;
    case 401:
        playManVsMan(e);
        break;
    case 402:
        playEnslave(e);
        break;
    case 500:
        playPresentArmy(e);
        break;
    case 502:
        playAdvanceArmy(e);
        break;
    case 503:
        playReturnArmy(e);
        break;
    case 504:
        playRemoveDead(e);
        break;
    default:
        break;
    }
    if (e->get<CArmy>()->nextAction >= 200 && e->get<CArmy>()->nextAction <= 299) {
        CAction::ID id = e->get<CArmy>()->nextAction;
        if (CAction::Map[id].oneTimeBattle || CAction::Map[id].oneTimeWar) {
            e->get<CArmy>()->captain->get<CCaptain>()->actions.remove(e->get<CArmy>()->nextAction);
        }
    }
}
bool lowerFireResistance(Entity* u1, Entity* u2) {
    CUnit::DamageType t = CUnit::FIRE;
    return u1->get<CUnit>()->resistance[t] < u2->get<CUnit>()->resistance[t];
}
bool lowerWaterResistance(Entity* u1, Entity* u2) {
    CUnit::DamageType t = CUnit::WATER;
    return u1->get<CUnit>()->resistance[t] < u2->get<CUnit>()->resistance[t];
}
bool lowerEarthResistance(Entity* u1, Entity* u2) {
    CUnit::DamageType t = CUnit::EARTH;
    return u1->get<CUnit>()->resistance[t] < u2->get<CUnit>()->resistance[t];
}
bool lowerAirResistance(Entity* u1, Entity* u2) {
    CUnit::DamageType t = CUnit::AIR;
    return u1->get<CUnit>()->resistance[t] < u2->get<CUnit>()->resistance[t];
}
bool higherFireResistance(Entity* u1, Entity* u2) {
    CUnit::DamageType t = CUnit::FIRE;
    return u1->get<CUnit>()->resistance[t] > u2->get<CUnit>()->resistance[t];
}
bool higherWaterResistance(Entity* u1, Entity* u2) {
    CUnit::DamageType t = CUnit::WATER;
    return u1->get<CUnit>()->resistance[t] > u2->get<CUnit>()->resistance[t];
}
bool higherEarthResistance(Entity* u1, Entity* u2) {
    CUnit::DamageType t = CUnit::EARTH;
    return u1->get<CUnit>()->resistance[t] > u2->get<CUnit>()->resistance[t];
}
bool higherAirResistance(Entity* u1, Entity* u2) {
    CUnit::DamageType t = CUnit::AIR;
    return u1->get<CUnit>()->resistance[t] > u2->get<CUnit>()->resistance[t];
}
bool prioritizeFire(Entity* e1, Entity* e2) {
    CUnit::DamageType t = CUnit::FIRE;
    return e1->get<CUnit>()->damage == t && e1->get<CUnit>()->damage != t;
}
bool prioritizeWater(Entity* e1, Entity* e2) {
    CUnit::DamageType t = CUnit::WATER;
    return e1->get<CUnit>()->damage == t && e1->get<CUnit>()->damage != t;
}
bool prioritizeEarth(Entity* e1, Entity* e2) {
    CUnit::DamageType t = CUnit::EARTH;
    return e1->get<CUnit>()->damage == t && e1->get<CUnit>()->damage != t;
}
bool prioritizeAir(Entity* e1, Entity* e2) {
    CUnit::DamageType t = CUnit::AIR;
    return e1->get<CUnit>()->damage == t && e1->get<CUnit>()->damage != t;
}

void ActionSystem::playPresentArmy(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(500, e));
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playPresentCaptain(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(501, e));
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playAdvanceArmy(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(502, e));
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playReturnArmy(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(503, e));
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playRemoveDead(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(504, e));
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::computeMeleeBattle(Entity* P, Entity* eOut) {
    Entity* P1 = P;
    Entity* P2 = P->get<CPlayer>()->enemy;

    Entity* eWin;
    Entity* eLos;

    EntityList P1alive = getAliveUnits(P1);
    EntityList P2alive = getAliveUnits(P2);

    int nP1 = P1alive.size();
    int nP2 = P2alive.size();
    int total = nP1 + nP2;

    if (randomInt(1, total) < nP1) {
        eWin = P1;
        eLos = P2;
        if (nP1 > nP2) eOut->get<CActionOutcome>()->targets.merge(pickNRandom(P1alive, nP1 - nP2));
        else eOut->get<CActionOutcome>()->targets.merge(pickNRandom(P1alive, nP1 - 1));
    } else {
        eWin = P2;
        eLos = P1;
        if (nP1 > nP2) eOut->get<CActionOutcome>()->targets.merge(pickNRandom(P2alive, nP2 - nP1));
        else eOut->get<CActionOutcome>()->targets.merge(pickNRandom(P2alive, nP2 - 1));
    }

    eOut->get<CActionOutcome>()->targets.merge(getAliveUnits(eLos));
    eOut->get<CActionOutcome>()->eWinner = eWin;
    eOut->get<CActionOutcome>()->eLoser = eLos;
}
void ActionSystem::playMeleeBattle(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(400, e));
    computeMeleeBattle(e, eOutcome);
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playEnslave(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(402, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;

    notify(START_ANIMATION, eOutcome);

    eEnemy->get<CArmy>()->captains.erase(eEnemy->get<CArmy>()->captains.find(eEnemy->get<CArmy>()->captain->get<CCaptain>()->id));
    //eEnemy->get<CArmy>()->captain = nullptr;
}

void ActionSystem::onSceneEnded(Entity* e) {

}

EntityList ActionSystem::getAliveUnits(Entity* e) {
    EntityList l;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (!eUnit->get<CUnit>()->dead) l.push_back(eUnit);
    }
    return l;
}

EntityList ActionSystem::getDeadUnits(Entity* e) {
    EntityList l;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) l.push_back(eUnit);
    }
    return l;
}

void ActionSystem::playEffectAction(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(200, e));
    eOutcome->get<CActionOutcome>()->targets = getAliveUnits(e);
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playUnitAttack(Entity* e, CUnit::ID uID) {
    Entity* eOutcome = eManager->createEntity();

    Entity* eAttacker = e;
    Entity* eDefender = e->get<CPlayer>()->enemy;

    EntityList aliveAtk = getAliveUnits(eAttacker);
    EntityList aliveDef = getAliveUnits(eDefender);

    EntityList unitsAtk;
    EntityList unitsDef;

    list<TargetOutcome> outcomes;
    CUnit::DamageType dmg;

    for(EntityListIt i = aliveAtk.begin(); i != aliveAtk.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->id == uID) {
            unitsAtk.push_back(eUnit);
            dmg = eUnit->get<CUnit>()->damage;
        }
    }

    orderTargets(aliveDef, eAttacker->get<CArmy>()->armyEffects, dmg);
    int nCount = 0;
    int nAtk = unitsAtk.size();
    while(nCount < nAtk) {
        for(EntityListIt i = aliveDef.begin(); i != aliveDef.end(); i++) {
            unitsDef.push_back(*i);
            if (++nCount == nAtk) break;
        }
    }
    nCount = unitsDef.size();

    for(EntityListIt i = unitsDef.begin(); i != unitsDef.end(); i++) {
        Entity* eUnit = *i;
        TargetOutcome outcome;
        outcome.eCauser = pickRandom(unitsAtk);
        unitsAtk.remove(outcome.eCauser);
        outcome.eTarget = eUnit;

        int resPoints = eUnit->get<CUnit>()->realResist[outcome.eCauser->get<CUnit>()->damage];
        double resChance = resPoints*war.getMatchConfig().uBlockChance;
        if (randomDouble(0,1) <= resChance) {
            outcome.id = outcome.BLOCKED;
        } else {
            outcome.id = outcome.DIED;
        }
        outcomes.push_back(outcome);
    }

    eOutcome->add(new CActionOutcome(uID, e));
    eOutcome->get<CActionOutcome>()->outcomes = outcomes;
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playManVsMan(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(401, e));

    Entity* eWin;
    Entity* eLos;

    double winChance = 0.75;

    Entity* eEnemy = e->get<CPlayer>()->enemy;

    if (randomInt(0, 100) <= winChance*100) {
        eWin = e;
        eLos = eEnemy;
    } else {
        eWin = eEnemy;
        eLos = e;
    }
    eLos->get<CPlayer>()->maxPicks -= war.getMatchConfig().recruitGroup;
    eLos->get<CPlayer>()->maxRepicks--;

    EntityList eLosAlive = getAliveUnits(eLos);
    EntityList eWinAlive = getAliveUnits(eWin);

    eOutcome->get<CActionOutcome>()->eWinner = eWin;
    eOutcome->get<CActionOutcome>()->eLoser = eLos;

    eOutcome->get<CActionOutcome>()->actors.push_back(pickRandom(eLosAlive));
    eOutcome->get<CActionOutcome>()->actors.push_back(pickRandom(eWinAlive));

    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playIntimidate(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(202, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;
    EntityList alive = getAliveUnits(eEnemy);

    Entity* eHero = e->get<CArmy>()->captain;

    int bailChance = 10 + (eHero->get<CCaptain>()->morale-1)*2;
    for(EntityListIt i = alive.begin(); i != alive.end(); i++) {
        if (randomInt(1, 100) <= bailChance) {
            eOutcome->get<CActionOutcome>()->targets.push_back(*i);
            //eEnemy->get<CArmy>()->allUnits.remove(*i);
        }
    }

    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playFocusFire(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(201, e));

    EntityList alive = getAliveUnits(e);

    for(EntityListIt i = alive.begin(); i != alive.end(); i++) {
        (*i)->get<CUnit>()->effects[201] = CAction::Map[201];
    }

    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playResurrect(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(203, e));

    unsigned int nRes = 5;
    EntityList candidates;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) {
            candidates.push_back(eUnit);
        }
    }

    EntityList targets;
    if (nRes >= candidates.size()) {
        targets = candidates;
    } else {
        targets = pickNRandom(candidates, nRes);
    }

    for(EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<CUnit>()->dead = false;
    }

    eOutcome->get<CActionOutcome>()->targets = targets;

    notify(START_ANIMATION, eOutcome);

}

void ActionSystem::playFireBuffAction(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(204, e));

    unsigned int nTargets = 20;
    //unused:
    //int buff = 1;

    EntityList candidates = getAliveUnits(e);
    EntityList targets;
    if (nTargets >= candidates.size()) {
        targets = candidates;
    } else {
        targets = pickNRandom(candidates, nTargets);
    }

    for(EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<CUnit>()->effects[204] = CAction::Map[204];
    }

    eOutcome->get<CActionOutcome>()->targets = targets;
    notify(START_ANIMATION, eOutcome);
}
void ActionSystem::playWaterBuffAction(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(205, e));

    unsigned int nTargets = 20;
    //unused
    //int buff = 1;

    EntityList candidates = getAliveUnits(e);
    EntityList targets;
    if (nTargets >= candidates.size()) {
        targets = candidates;
    } else {
        targets = pickNRandom(candidates, nTargets);
    }

    for(EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<CUnit>()->effects[205] = CAction::Map[205];
    }

    eOutcome->get<CActionOutcome>()->targets = targets;
    notify(START_ANIMATION, eOutcome);
}
void ActionSystem::playEarthBuffAction(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(206, e));

    unsigned int nTargets = 20;
    //unused:
    //int buff = 1;

    EntityList candidates = getAliveUnits(e);
    EntityList targets;
    if (nTargets >= candidates.size()) {
        targets = candidates;
    } else {
        targets = pickNRandom(candidates, nTargets);
    }

    for(EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<CUnit>()->effects[206] = CAction::Map[206];
    }

    eOutcome->get<CActionOutcome>()->targets = targets;
    notify(START_ANIMATION, eOutcome);
}
void ActionSystem::playFireDebuffAction(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(207, e));

    unsigned int nTargets = 20;
    //unused
    //int buff = -1;

    EntityList candidates = getAliveUnits(e);
    EntityList targets;
    if (nTargets >= candidates.size()) {
        targets = candidates;
    } else {
        targets = pickNRandom(candidates, nTargets);
    }

    for(EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<CUnit>()->effects[207] = CAction::Map[207];
    }

    eOutcome->get<CActionOutcome>()->targets = targets;
    notify(START_ANIMATION, eOutcome);
}
void ActionSystem::playWaterDebuffAction(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(208, e));

    int nTargets = 20;
    //unused:
    //int buff = -1;

    EntityList candidates = getAliveUnits(e);
    EntityList targets;
    if ((unsigned) nTargets >= candidates.size()) {
        targets = candidates;
    } else {
        targets = pickNRandom(candidates, nTargets);
    }

    for(EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<CUnit>()->effects[208] = CAction::Map[208];
    }

    eOutcome->get<CActionOutcome>()->targets = targets;
    notify(START_ANIMATION, eOutcome);
}
void ActionSystem::playEarthDebuffAction(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(209, e));

    int nTargets = 20;
    //unused:
    //int buff = -1;

    EntityList candidates = getAliveUnits(e);
    EntityList targets;
    if ((unsigned) nTargets >= candidates.size()) {
        targets = candidates;
    } else {
        targets = pickNRandom(candidates, nTargets);
    }

    for(EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<CUnit>()->effects[209] = CAction::Map[209];
    }

    eOutcome->get<CActionOutcome>()->targets = targets;
    notify(START_ANIMATION, eOutcome);
}
void ActionSystem::playConvert(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(217, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;

    EntityList candidates = getAliveUnits(eEnemy);
    EntityList allies = getAliveUnits(e);
    int nTargets = min(10, (int)candidates.size());
    int armySpace = war.getMatchConfig().armySize - allies.size();
    if (nTargets > armySpace) nTargets = armySpace;

    EntityList convert = pickNRandom(candidates, nTargets);

    for(EntityListIt i = convert.begin(); i != convert.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<COwner>()->e = e;
        e->get<CArmy>()->allUnits.push_back(eUnit);
        eEnemy->get<CArmy>()->allUnits.remove(eUnit);
        eUnit->get<CUnit>()->effects.clear();
    }

    eOutcome->get<CActionOutcome>()->targets = convert;
    notify(START_ANIMATION, eOutcome);
}

void ActionSystem::playSummon(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(218, e));
    int hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;

    CUnit::ID id = 19;
    EntityList allies = getAliveUnits(e);
    int armySpace = war.getMatchConfig().armySize - allies.size();
    int n = randomInt(1, 10);
    if (n > armySpace) n = armySpace;

    for(int i = 0; i < n; i++) {
        Entity* eUnit = eManager->createEntity();
        int subUnit = randomInt(1, 2);
        CUnit u = CUnit::Map[id];
        u.aIdle = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aWalk = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aDeath = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aDead = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        eUnit->add(new CUnit(u));
        eUnit->add(new COwner(e));
        eUnit->add(new CAnimation(hFlip, u.aIdle));
        eUnit->add(new CDraw(CDraw::WORLD_1));
        eUnit->add(new CVelocity());
        eUnit->add(new CPosition());
        eUnit->get<CVelocity>()->speed = 80;
        eUnit->add(new CActor());
        eUnit->add(new CUnitHighlight2(Assets::colors[id]));
        eUnit->get<CUnit>()->armyID = e->get<CArmy>()->nextUnitID++;

        e->get<CArmy>()->allUnits.push_back(eUnit);
        eOut->get<CActionOutcome>()->actors.push_back(eUnit);
    }

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playCallSlaves(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(239, e));
    int hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;

    CUnit::ID id = 20;
    EntityList allies = getAliveUnits(e);
    int armySpace = war.getMatchConfig().armySize - allies.size();
    int n = randomInt(1, 10);
    if (n > armySpace) n = armySpace;

    for(int i = 0; i < n; i++) {
        Entity* eUnit = eManager->createEntity();
        int subUnit = randomInt(1, 4);
        CUnit u = CUnit::Map[id];
        u.aIdle = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aWalk = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aDeath = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aDead = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        eUnit->add(new CUnit(u));
        eUnit->add(new COwner(e));
        eUnit->add(new CAnimation(hFlip, u.aIdle));
        eUnit->add(new CDraw(CDraw::WORLD_1));
        eUnit->add(new CVelocity());
        eUnit->add(new CPosition());
        eUnit->get<CVelocity>()->speed = 80;
        eUnit->add(new CActor());
        eUnit->add(new CUnitHighlight2(Assets::colors[id]));
        eUnit->get<CUnit>()->armyID = e->get<CArmy>()->nextUnitID++;

        e->get<CArmy>()->allUnits.push_back(eUnit);
        eOut->get<CActionOutcome>()->actors.push_back(eUnit);
    }

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playTrueForm(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(219, e));

    Entity* eCaptain = e->get<CArmy>()->captain;
    e->get<CPlayer>()->heroDeck.remove((int)eCaptain->get<CCaptain>()->id);
    eManager->removeEntity(eCaptain);
    map<CCaptain::ID, Entity*>::iterator it;
    it = e->get<CArmy>()->captains.find(eCaptain->get<CCaptain>()->id);
    e->get<CArmy>()->captains.erase(it);

    CCaptain::ID id = 13;
    e->get<CPlayer>()->heroDeck.push_back(id);

    Entity* eCap = eManager->createEntity();
    eCap->add(new CCaptain(CCaptain::Map[id]));
    eCap->add(new CAnimation(false, CCaptain::Map[id].aIdle));
    eCap->add(new CDraw(CDraw::WORLD_1));
    eCap->add(new CVelocity());
    eCap->add(new CPosition(eCaptain->get<CPosition>()->x, eCaptain->get<CPosition>()->y));
    eCap->add(new CActor());
    eCap->get<CVelocity>()->speed = 80;

    e->get<CArmy>()->captains.insert(make_pair(id, eCap));
    e->get<CArmy>()->captain = eCap;

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playHex(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(219, e));
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    Entity* eCaptain = eEnemy->get<CArmy>()->captain;
    CCaptain::ID original = eCaptain->get<CCaptain>()->id;

    if (original == 14) {
        eEnemy->get<CPlayer>()->heroDeck.remove((int)eCaptain->get<CCaptain>()->id);
        eManager->removeEntity(eCaptain);
        map<CCaptain::ID, Entity*>::iterator it;
        it = eEnemy->get<CArmy>()->captains.find(eCaptain->get<CCaptain>()->id);
        eEnemy->get<CArmy>()->captains.erase(it);

        CCaptain::ID id = 14;
        eEnemy->get<CPlayer>()->heroDeck.push_back(id);

        Entity* eCap = eManager->createEntity();
        eCap->add(new CCaptain(CCaptain::Map[id]));
        eCap->add(new CAnimation(false, CCaptain::Map[id].aIdle));
        eCap->add(new CDraw(CDraw::WORLD_1));
        eCap->add(new CVelocity());
        eCap->add(new CPosition(eCaptain->get<CPosition>()->x, eCaptain->get<CPosition>()->y));
        eCap->add(new CActor());
        eCap->add(new CDimensions(50, 50));
        eCap->add(new COriginalHero(original));
        eCap->get<CVelocity>()->speed = 80;

        eEnemy->get<CArmy>()->captains.insert(make_pair(id, eCap));
        eEnemy->get<CArmy>()->captain = eCap;

        notify(START_ANIMATION, eOut);
    } else {
        notify(START_ANIMATION, eOut);
    }
}

void ActionSystem::playStampede(Entity* e) {
    Entity* eOutcome = eManager->createEntity();
    eOutcome->add(new CActionOutcome(241, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;
    EntityList alive = getAliveUnits(eEnemy);

    double chance = 0.2;
    for(EntityListIt i = alive.begin(); i != alive.end(); i++) {
        if (randomInt(0, 100) <= chance*100) {
            eOutcome->get<CActionOutcome>()->targets.push_back(*i);
        }
    }

    notify(START_ANIMATION, eOutcome);

    for(EntityListIt i = eOutcome->get<CActionOutcome>()->targets.begin(); i != eOutcome->get<CActionOutcome>()->targets.end(); i++) {
        eEnemy->get<CArmy>()->allUnits.remove(*i);
    }
}
bool ActionSystem::contains(map<CAction::ID, CAction>& m, CAction::ID id) {
    map<CAction::ID, CAction>::iterator it;
    it = m.find(id);
    return it != m.end();
}

void ActionSystem::playSwapHeroes(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(220, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;

    Entity* eCap1 = e->get<CArmy>()->captain;
    Entity* eCap2 = eEnemy->get<CArmy>()->captain;

    e->get<CArmy>()->captains.erase(e->get<CArmy>()->captains.erase(eCap1->get<CCaptain>()->id));
    e->get<CArmy>()->captains.insert(make_pair(eCap2->get<CCaptain>()->id, eCap2));
    e->get<CArmy>()->captain = eCap2;

    eEnemy->get<CArmy>()->captains.erase(e->get<CArmy>()->captains.erase(eCap2->get<CCaptain>()->id));
    eEnemy->get<CArmy>()->captains.insert(make_pair(eCap1->get<CCaptain>()->id, eCap1));
    eEnemy->get<CArmy>()->captain = eCap1;

    eEnemy->get<CArmy>()->captain->get<CCaptain>()->actions.remove(220);

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playPurify(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(216, e));

    /// ARMY EFFECTS
    map<CAction::ID, CAction>::iterator it;
    IntList toRemove;
    for(it = e->get<CArmy>()->armyEffects.begin(); it != e->get<CArmy>()->armyEffects.end(); it++) {
        if (it->second.effectType == CAction::DEBUFF) {
            toRemove.push_back(it->first);
        }
    }
    for(IntListIt i = toRemove.begin(); i != toRemove.end(); i++) {
        e->get<CArmy>()->armyEffects.erase(e->get<CArmy>()->armyEffects.find(*i));
    }
    toRemove.clear();

    /// UNIT EFFECTS
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        for(it = eUnit->get<CUnit>()->effects.begin(); it != eUnit->get<CUnit>()->effects.end(); it++) {
            if (it->second.effectType == CAction::DEBUFF) {
                toRemove.push_back(it->first);
            }
        }
        for(IntListIt j = toRemove.begin(); j != toRemove.end(); j++) {
            eUnit->get<CUnit>()->effects.erase(eUnit->get<CUnit>()->effects.find(*j));
        }
        toRemove.clear();
    }

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playConfusion(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(230, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;
    eEnemy->get<CArmy>()->armyEffects.insert(make_pair(230, CAction::Map[230]));

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playFocus(Entity* e, CUnit::DamageType dmgType) {
    Entity* eOut = eManager->createEntity();
    CAction::ID actID;
    switch(dmgType) {
    case CUnit::FIRE:
        actID = 210;
        break;
    case CUnit::WATER:
        actID = 211;
        break;
    case CUnit::EARTH:
        actID = 212;
        break;
    case CUnit::AIR:
        actID = 234;
        break;
    default:
        break;
    }
    eOut->add(new CActionOutcome(actID, e));

    EntityList alive = getAliveUnits(e->get<CPlayer>()->enemy);
    alive = pickNRandom(alive, alive.size());
    int nTargets = 0;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        if (eUnit->get<CUnit>()->damage == dmgType) nTargets++;
    }

    int nCount = 0;
    for(EntityListIt i = alive.begin(); i != alive.end(); i++) {
        Entity* eUnit = *i;
        eUnit->get<CUnit>()->effects.insert(make_pair(actID, CAction::Map[actID]));
        if (++nCount >= nTargets) break;
    }

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playHeroAttack(Entity* e, CAction::ID actID) {
    Entity* eOut = eManager->createEntity();
    CUnit::DamageType dmgType;
    switch(actID) {
    case 213:
        dmgType = CUnit::FIRE;
        break;
    case 214:
        dmgType = CUnit::WATER;
        break;
    case 215:
        dmgType = CUnit::EARTH;
        break;
    case 237:
        dmgType = CUnit::AIR;
        break;
    default:
        break;
    }
    eOut->add(new CActionOutcome(actID, e));

    Entity* eAttacker = e;
    Entity* eDefender = e->get<CPlayer>()->enemy;

    EntityList aliveAtk = getAliveUnits(eAttacker);
    EntityList aliveDef = getAliveUnits(eDefender);

    EntityList unitsAtk;
    EntityList unitsDef = aliveDef;

    list<TargetOutcome> outcomes;

    for(EntityListIt i = unitsDef.begin(); i != unitsDef.end(); i++) {
        Entity* eUnit = *i;
        TargetOutcome outcome;
        outcome.eTarget = eUnit;

        int resPoints = eUnit->get<CUnit>()->resistance[dmgType];
        double resChance = resPoints*war.getMatchConfig().uBlockChance;
        if (randomDouble(0,1) <= resChance) {
            outcome.id = outcome.BLOCKED;
        } else {
            outcome.id = outcome.DIED;
        }
        outcomes.push_back(outcome);
    }

    eOut->get<CActionOutcome>()->outcomes = outcomes;

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playMindControl(Entity* e) {

    Entity* eCap = e->get<CArmy>()->captain;
    EntityList caps;
    for(map<CCaptain::ID, Entity*>::iterator i = e->get<CArmy>()->captains.begin(); i != e->get<CArmy>()->captains.end(); i++) {
        if (eCap == i->second) continue;
        caps.push_back(i->second);
    }
    Entity* eHelp = pickRandom(caps);

    int actID;
    vector<int> actions;
    for(IntListIt i = eHelp->get<CCaptain>()->actions.begin(); i != eHelp->get<CCaptain>()->actions.end(); i++) {
        actions.push_back(*i);
    }
    actID = actions[randomInt(0, actions.size()-1)];

    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    bool hFlip = sign == -1 ? false:true;

    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Help!", 2));

    //unused:
    //double wFormation = 15;
    //double uxFormation = 16;
    double hFormation = 23;
    double uyFormation = 13;
    double x = cxWindow + sign*wWindow/2 + sign*60;
    double y = cyWindow + (hFormation+2)*uyFormation/2;

    eHelp->get<CActor>()->timeline.push_back(new ATeleport(0.0, x, y));
    eHelp->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, hFlip));
    eHelp->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eHelp->get<CCaptain>()->aIdle));
    eHelp->get<CActor>()->timeline.push_back(new AMove(0.0, x - 2*sign*60, y, 200));
    eHelp->get<CActor>()->timeline.push_back(new ASpriteAnimation(getTravelTime(x,y,x - 2*sign*60, y,200), eHelp->get<CCaptain>()->aIdle));

    e->get<CArmy>()->captain = eHelp;
    e->get<CArmy>()->nextAction = actID;
    playAction(e);
    //notify(START_ANIMATION, eOut);
    e->get<CArmy>()->captain = eCap;

    eHelp->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eHelp->get<CCaptain>()->aIdle));
    eHelp->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, opposite(hFlip)));
    eHelp->get<CActor>()->timeline.push_back(new AMove(0.0, x, y, 200));
    eHelp->get<CActor>()->timeline.push_back(new ASpriteAnimation(getTravelTime(x,y,x - 2*sign*60, y,200), eHelp->get<CCaptain>()->aIdle));
    eHelp->get<CActor>()->timeline.push_back(new ATeleport(0.0, x, cyWindow));

}

void ActionSystem::playPoison(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(221, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;
    eEnemy->get<CArmy>()->armyEffects.insert(make_pair(221, CAction::Map[221]));

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playCurse(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(222, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;
    eEnemy->get<CArmy>()->armyEffects.insert(make_pair(222, CAction::Map[222]));

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playTeleport(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(228, e));

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playCroak(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(240, e));

    double chance = 0.6;
    if (randomDouble(0, 100) <= chance*100) {
        Entity* eCaptain = e->get<CArmy>()->captain;
        CCaptain original = eCaptain->get<COriginalHero>()->hero;

        e->get<CPlayer>()->heroDeck.remove((int)eCaptain->get<CCaptain>()->id);
        eManager->removeEntity(eCaptain);
        map<CCaptain::ID, Entity*>::iterator it;
        it = e->get<CArmy>()->captains.find(eCaptain->get<CCaptain>()->id);
        e->get<CArmy>()->captains.erase(it);

        CCaptain::ID id = original.id;
        e->get<CPlayer>()->heroDeck.push_back(id);

        Entity* eCap = eManager->createEntity();
        eCap->add(new CCaptain(original));
        eCap->add(new CAnimation(false, CCaptain::Map[id].aIdle));
        eCap->add(new CDraw(CDraw::WORLD_1));
        eCap->add(new CVelocity());
        eCap->add(new CPosition(eCaptain->get<CPosition>()->x, eCaptain->get<CPosition>()->y));
        eCap->add(new CActor());
        eCap->add(new CDimensions(50, 50));
        eCap->get<CVelocity>()->speed = 80;

        e->get<CArmy>()->captains.insert(make_pair(id, eCap));
        e->get<CArmy>()->captain = eCap;
    }

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playSilence(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(235, e));

    //unused:
    //Entity* eAttacker = e;
    Entity* eDefender = e->get<CPlayer>()->enemy;

    EntityList aliveDef = getAliveUnits(eDefender);

    EntityList unitsDef = aliveDef;

    list<TargetOutcome> outcomes;
    double chance = 0.3;

    for(EntityListIt i = unitsDef.begin(); i != unitsDef.end(); i++) {
        Entity* eUnit = *i;
        TargetOutcome outcome;
        outcome.eTarget = eUnit;

        if (randomDouble(0,1) > chance) {
            outcome.id = outcome.BLOCKED;
        } else {
            outcome.id = outcome.DIED;
            eUnit->get<CUnit>()->effects.insert(make_pair(235, CAction::Map[235]));
        }
        outcomes.push_back(outcome);
    }

    eOut->get<CActionOutcome>()->outcomes = outcomes;

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playImmortality(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(238, e));

    EntityList alive = getAliveUnits(e);
    double chance = 0.3;
    for(EntityListIt i = alive.begin(); i != alive.end(); i++) {
        Entity* eUnit = *i;
        if (randomDouble(0, 1) <= chance) {
            eUnit->get<CUnit>()->effects.insert(make_pair(238, CAction::Map[238]));
        }
    }

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playTimeDilatation(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(213, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;
    eEnemy->get<CArmy>()->armyEffects.insert(make_pair(213, CAction::Map[213]));

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playChangeFormation(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(212, e));

    Entity* eEnemy = e->get<CPlayer>()->enemy;
    vector<int> formations;
    for (int i = 0; i < 3; i++) {
        if (eEnemy->get<CArmy>()->formation == i) continue;
        formations.push_back(i);
    }
    eEnemy->get<CArmy>()->armyEffects.erase(eEnemy->get<CArmy>()->armyEffects.find(eEnemy->get<CArmy>()->formation + 300));
    eEnemy->get<CArmy>()->formation = (CArmy::Formation) formations[randomInt(0, formations.size()-1)];
    eEnemy->get<CArmy>()->armyEffects.insert(make_pair(eEnemy->get<CArmy>()->formation + 300, CAction::Map[eEnemy->get<CArmy>()->formation + 300]));

    notify(START_ANIMATION, eOut);
}

void ActionSystem::playTarot(Entity* e) {
    Entity* eOut = eManager->createEntity();
    eOut->add(new CActionOutcome(227, e));

    int card = randomInt(1, 3);
    int nEnemies = 0;
    int nAllies = 0;
    int nTargets = 5;
    eOut->get<CActionOutcome>()->iValue = card;
    switch (card) {
    case 1:
        nEnemies = nTargets;
        break;
    case 2:
        nEnemies = nTargets;
        nAllies = nTargets;
        break;
    case 3:
        nAllies = nTargets;
        break;
    default:
        break;
    }

    EntityList aliveAllies = getAliveUnits(e);
    EntityList aliveEnemies = getAliveUnits(e->get<CPlayer>()->enemy);

    pushBack(eOut->get<CActionOutcome>()->targets, pickNRandom(aliveAllies, nAllies));
    pushBack(eOut->get<CActionOutcome>()->targets, pickNRandom(aliveEnemies, nEnemies));

    notify(START_ANIMATION, eOut);
}

void ActionSystem::preprocessCoinThrow(ActionOutcome& outcome, Entity* actor) {
    outcome.idWinner = randomInt(1, 2);
    outcome.idLoser = outcome.idWinner == 1 ? 2:1;
}

void ActionSystem::preprocessIntimidation(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    EntityList alive = getAliveUnits(eEnemy);

    double bailChance = 0.1;
    for (EntityListIt i = alive.begin(); i != alive.end(); i++) {
        if (randomInt(0, 100) <= bailChance*100) {
            outcome.idTargets.push_back((*i)->get<CUnit>()->armyID);
        }
    }
}

void ActionSystem::preprocessArmyComposition(ActionOutcome& outcome, Entity* e) {
    map<CUnit::ID, CUnit>& units = CUnit::Map;
    outcome.armyComposition.clear();

#if 0
    outcome.armyComposition = e->get<CArmy>()->unitCount;
    int nUnits = 0;
    for (auto& i : outcome.armyComposition) {
        nUnits += i.second;
    }

    int nRemaining = war.getMatchConfig().armySize - nUnits;
    list<CUnit::ID>& deck = e->get<CPlayer>()->unitDeck;
    for (int i = 0; i < nRemaining; i += war.getMatchConfig().recruitGroup) {
        CUnit unit = units[getRandom(deck)];

        map<CUnit::ID, int>::iterator it = outcome.armyComposition.find(unit.id);
        if (it == outcome.armyComposition.end()) {
            outcome.armyComposition.insert(make_pair(unit.id, war.getMatchConfig().recruitGroup));
        } else {
            outcome.armyComposition[unit.id] += war.getMatchConfig().recruitGroup;
        }
    }
#else
    if (e->get<CPlayer>()->side == CPlayer::LEFT) {
        outcome.armyComposition.insert(make_pair(13, 20));
    } else {
        //outcome.armyComposition.insert(make_pair(2, 20));
    }
    
    int nUnits = 0;
    for (auto& i : outcome.armyComposition) {
        nUnits += i.second;
    }

    int nRemaining = war.getMatchConfig().armySize - nUnits;
    list<CUnit::ID>& deck = e->get<CPlayer>()->unitDeck;
    for (int i = 0; i < nRemaining; i += war.getMatchConfig().recruitGroup) {
        CUnit unit = units[getRandom(deck)];

        map<CUnit::ID, int>::iterator it = outcome.armyComposition.find(unit.id);
        if (it == outcome.armyComposition.end()) {
            outcome.armyComposition.insert(make_pair(unit.id, war.getMatchConfig().recruitGroup));
        } else {
            outcome.armyComposition[unit.id] += war.getMatchConfig().recruitGroup;
        }
    }
#endif
}

void ActionSystem::preprocessRandomArmy(ActionOutcome& outcome, Entity* e) {
    map<CUnit::ID, CUnit>& units = CUnit::Map;
    outcome.armyComposition.clear();
    outcome.armyComposition = e->get<CArmy>()->unitCount;

    int nUnits = 0;
    for (auto& i : outcome.armyComposition) {
        nUnits += i.second;
    }

    int nRemaining = war.getMatchConfig().armySize - nUnits;
    list<CUnit::ID>& deck = e->get<CPlayer>()->unitDeck;
    for(int i = 0; i < nRemaining; i += war.getMatchConfig().recruitGroup) {
        CUnit unit = units[getRandom(deck)];

        map<CUnit::ID, int>::iterator it = outcome.armyComposition.find(unit.id);
        if (it == outcome.armyComposition.end()) {
            outcome.armyComposition.insert(make_pair(unit.id, war.getMatchConfig().recruitGroup));
        } else {
            outcome.armyComposition[unit.id] += war.getMatchConfig().recruitGroup;
        }
    }
}

void ActionSystem::preprocessSummon(ActionOutcome& outcome, Entity* e) {
    EntityList allies = getAliveUnits(e);
    int armySpace = war.getMatchConfig().armySize - allies.size();
    int n = randomInt(1, 10);
    if (n > armySpace) n = armySpace;

    outcome.unit = 19;
    outcome.iValue = n;
}

void ActionSystem::preprocessSomniferous(ActionOutcome& outcome, Entity* e) {
    double chance = 1.0;
    if (randomDouble(0, 1) <= chance) outcome.bValue = true;
    else outcome.bValue = false;
}

void ActionSystem::preprocessTrueForm(ActionOutcome& outcome, Entity* e) {
    ///no preprocess required?
    outcome.hero = 13;
}

void ActionSystem::preprocessImmortality(ActionOutcome& outcome, Entity* e) {
    double chance = 0.3;
    for (auto& eUnit : e->get<CArmy>()->allUnits) {
        if (!eUnit->get<CUnit>()->dead) {
            if (randomDouble(0, 1) < chance) outcome.idTargets.push_back(eUnit->get<CUnit>()->armyID);
        }
    }
}

void ActionSystem::preprocessBuffDebuff(ActionOutcome& outcome, Entity* e) {
    switch(outcome.action) {
    case 204:
        outcome.iValue = +1;
        outcome.dmgType = CUnit::FIRE;
        break;
    case 205:
        outcome.iValue = +1;
        outcome.dmgType = CUnit::WATER;
        break;
    case 206:
        outcome.iValue = +1;
        outcome.dmgType = CUnit::EARTH;
        break;
    case 207:
        outcome.iValue = -1;
        outcome.dmgType = CUnit::FIRE;
        break;
    case 208:
        outcome.iValue = -1;
        outcome.dmgType = CUnit::WATER;
        break;
    case 209:
        outcome.iValue = -1;
        outcome.dmgType = CUnit::EARTH;
        break;
    case 232:
        outcome.iValue = +1;
        outcome.dmgType = CUnit::AIR;
        break;
    case 233:
        outcome.iValue = -1;
        outcome.dmgType = CUnit::AIR;
        break;
    }
    EntityList alive;
    if (outcome.iValue > 0) {
        alive = getAliveUnits(e);
    } else {
        alive = getAliveUnits(e->get<CPlayer>()->enemy);
    }
    double chance = 0.3;
    for(EntityListIt i = alive.begin(); i != alive.end(); i++) {
        if (randomDouble(0, 1) <= chance) {
            outcome.idTargets.push_back((*i)->get<CUnit>()->armyID);
        }
    }
}

void ActionSystem::preprocessConfusion(ActionOutcome& outcome, Entity* e) {
    double chance = 1.0;
    if (randomDouble(0, 1) <= chance) {
        outcome.bValue = true;
    } else {
        outcome.bValue = false;
    }
}

void ActionSystem::preprocessChangeFormation(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    vector<int> formations;
    for (int i = 0; i < 3; i++) {
        if (eEnemy->get<CArmy>()->formation == i) continue;
        formations.push_back(i);
    }
    outcome.formation = (CArmy::Formation)formations[randomInt(0, formations.size()-1)];
}

void ActionSystem::preprocessHex(ActionOutcome& outcome, Entity* e) {
    double chance = 0.5;
#if 0
    if (randomDouble(0, 1) <= chance) outcome.bValue = true;
    else outcome.bValue = false;
#else
    outcome.bValue = true;
#endif
}

void ActionSystem::preprocessTarot(ActionOutcome& outcome, Entity* e) {
    vector<int> cardWeights = {50, 30, 20};
    list<int> cardPool;
    for (int i = 0; i < 3; i++) {
        for (int w = 0; w < cardWeights[i]; w++) cardPool.push_back(i);
    }
    int card = getRandom(cardPool) + 1;
    int nEnemies = 0;
    int nAllies = 0;
    int nTargets = 5;
    outcome.iValue = card;
    switch (card) {
    case 1:
        nEnemies = nTargets;
        break;
    case 2:
        nEnemies = nTargets;
        nAllies = nTargets;
        break;
    case 3:
        nAllies = nTargets;
        break;
    default:
        break;
    }

    EntityList aliveAllies = getAliveUnits(e);
    EntityList aliveEnemies = getAliveUnits(e->get<CPlayer>()->enemy);

    EntityList targets;
    targets = pickNRandom(aliveAllies, nAllies);
    for (auto& i : targets) outcome.idActors.push_back(i->get<CUnit>()->armyID);
    targets = pickNRandom(aliveEnemies, nEnemies);
    for (auto& i : targets) outcome.idTargets.push_back(i->get<CUnit>()->armyID);
}

void ActionSystem::preprocessPurify(ActionOutcome& outcome, Entity* e) {
    //nothing to preprocess?
}

void ActionSystem::preprocessResurect(ActionOutcome& outcome, Entity* e) {
    int nRes = 5;
    EntityList candidates;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) {
            candidates.push_back(eUnit);
        }
    }

    EntityList targets;
    if ((unsigned)nRes >= candidates.size()) {
        targets = candidates;
    } else {
        targets = pickNRandom(candidates, nRes);
    }

    for(EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        outcome.idTargets.push_back(eUnit->get<CUnit>()->armyID);
    }
}

void ActionSystem::preprocessCurse(ActionOutcome& outcome, Entity* e) {
    double chance = 0.3;
    if (randomDouble(0, 1) <= chance) outcome.bValue = true;
    else outcome.bValue = false;
}

void ActionSystem::preprocessSleep(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    double chance = 0.3;
    for (auto& eUnit : eEnemy->get<CArmy>()->allUnits) {
        if (!eUnit->get<CUnit>()->dead) {
            if (randomDouble(0, 1) < chance) outcome.idTargets.push_back(eUnit->get<CUnit>()->armyID);
        }
    }
}

void ActionSystem::preprocessTimeDilatation(ActionOutcome& outcome, Entity* e) {
    //no preprocess required?
    outcome.iValue = 5;
}

void ActionSystem::preprocessSlavesCall(ActionOutcome& outcome, Entity* e) {
    EntityList allies = getAliveUnits(e);
    int armySpace = war.getMatchConfig().armySize - allies.size();
    int n = randomInt(1, 10);
    if (n > armySpace) n = armySpace;

    outcome.unit = 20;
    outcome.iValue = n;
}

void ActionSystem::preprocessConversion(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    EntityList candidates = getAliveUnits(eEnemy);
    EntityList allies = getAliveUnits(e);
    int nTargets = min(10, (int)candidates.size());
    int armySpace = war.getMatchConfig().armySize - allies.size();
    if (nTargets > armySpace) nTargets = armySpace;

    EntityList convert = pickNRandom(candidates, nTargets);

    for(EntityListIt i = convert.begin(); i != convert.end(); i++) {
        Entity* eUnit = *i;
        outcome.idTargets.push_back(eUnit->get<CUnit>()->armyID);
    }
}

void ActionSystem::preprocessDoom(ActionOutcome& outcome, Entity* e) {
    double chance = 0.05;
    if (randomDouble(0, 1) <= chance) outcome.bValue = true;
    else outcome.bValue = false;
}

void ActionSystem::preprocessSwap(ActionOutcome& outcome, Entity* e) {
    //no preprocess required
}

void ActionSystem::preprocessTeleport(ActionOutcome& outcome, Entity* e) {
    //teleport
}

void ActionSystem::preprocessStampede(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    EntityList alive = getAliveUnits(eEnemy);

    double bailChance = 0.15;
    for(EntityListIt i = alive.begin(); i != alive.end(); i++) {
        if (randomInt(0, 100) <= bailChance*100) {
            outcome.idTargets.push_back((*i)->get<CUnit>()->armyID);
        }
    }
}

void ActionSystem::preprocessBecomeHuman(ActionOutcome& outcome, Entity* e) {
    //no preprocess
    outcome.hero = 10;
}

void ActionSystem::preprocessCroak(ActionOutcome& outcome, Entity* e) {
    double chance = 0.70;
    if (randomDouble(0, 1) < chance) outcome.bValue = true;
    else outcome.bValue = false;
}

void ActionSystem::preprocessHelp(ActionOutcome& outcome, Entity* e) {
    list<int> heroes = e->get<CPlayer>()->heroDeck;
    heroes.remove(e->get<CArmy>()->captain->get<CCaptain>()->id);
    outcome.hero = (CCaptain::ID)getRandom(heroes);
    Entity* eHelper = e->get<CArmy>()->captains[outcome.hero];
    outcome.iValue = getRandom(eHelper->get<CCaptain>()->actions);
}

void ActionSystem::preprocessUnitAttack(ActionOutcome& outcome, Entity* e) {
    //unused:
    //Entity* eOutcome = eManager->createEntity();

    Entity* eAttacker = e;
    Entity* eDefender = e->get<CPlayer>()->enemy;

    EntityList aliveAtk = getAliveUnits(eAttacker);
    EntityList aliveDef = getAliveUnits(eDefender);

    EntityList unitsAtk;
    EntityList unitsDef;

    list<TargetOutcome> outcomes;
    CUnit::DamageType dmg;
    CUnit::ID uID = outcome.action;

    for(EntityListIt i = aliveAtk.begin(); i != aliveAtk.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->id == uID) {
            unitsAtk.push_back(eUnit);
            dmg = eUnit->get<CUnit>()->damage;
        }
    }

    orderTargets(aliveDef, eAttacker->get<CArmy>()->armyEffects, dmg);
    int nCount = 0;
    int nAtk = unitsAtk.size();
    while(nCount < nAtk) {
        for(EntityListIt i = aliveDef.begin(); i != aliveDef.end(); i++) {
            unitsDef.push_back(*i);
            if (++nCount >= nAtk) break;
        }
    }

    for(EntityListIt i = unitsDef.begin(); i != unitsDef.end(); i++) {
        Entity* eUnit = *i;
        Entity* eCauser = pickRandom(unitsAtk);
        Entity* eTarget = eUnit;
        UnitActionOutcome uout;
        unitsAtk.remove(eCauser);
        uout.idCauser = eCauser->get<CUnit>()->armyID;
        uout.idTarget = eTarget->get<CUnit>()->armyID;

        int resPoints = eUnit->get<CUnit>()->realResist[eCauser->get<CUnit>()->damage];
        double resChance = resPoints*war.getMatchConfig().uBlockChance + war.getMatchConfig().baseBlockChance;
        if (randomDouble(0,1) <= resChance) {
            uout.id = uout.BLOCKED;
        } else {
            uout.id = uout.DIED;
        }
        outcome.unitActionOutcomes.push_back(uout);
        outcome.idActors.push_back(uout.idCauser);
    }

    for(Entity* eDef : unitsDef) {
        if (!::contains(outcome.idTargets, eDef->get<CUnit>()->armyID)) {
            outcome.idTargets.push_back(eDef->get<CUnit>()->armyID);
        }
    }

    outcome.dmgType = dmg;
}

void ActionSystem::preprocessArmyVsArmy(ActionOutcome& outcome, Entity* e) {
    Entity* P1 = war.getPlayer(1);
    Entity* P2 = war.getPlayer(2);

    Entity* eWin;
    Entity* eLos;

    EntityList P1alive = getAliveUnits(P1);
    EntityList P2alive = getAliveUnits(P2);

    int nP1 = P1alive.size();
    int nP2 = P2alive.size();
    int total = nP1 + nP2;

    Entity* eLargest;
    Entity* eSmallest;
    if (nP1 >= nP2) {
        eLargest = P1;
        eSmallest = P2;
    } else {
        eLargest = P2;
        eSmallest = P1;
    }

    double fixedChance = 0.8;
    double remainingChance = 1.f-fixedChance;
    double bonusChance = ((double)eLargest->get<CArmy>()->nAlive/(double)total)*remainingChance;
    double chance = fixedChance + bonusChance;

    double result = randomDouble(0.0, 1.0);

    if (result < chance) {
        eWin = eLargest;
        eLos = eSmallest;
    } else {
        eLos = eLargest;
        eWin = eSmallest;
    }

    int nCasualties = 0;
    if (eWin->get<CArmy>()->nAlive - eLos->get<CArmy>()->nAlive > 1) {
        nCasualties = randomInt(1, eWin->get<CArmy>()->nAlive - eLos->get<CArmy>()->nAlive);
    }

    EntityList& alive = eWin == P1 ? P1alive : P2alive;
    EntityList casualties = pickNRandom(alive, nCasualties);

    for (auto& eUnit : casualties) {
        outcome.idTargets.push_back(eUnit->get<CUnit>()->armyID);
    }

    outcome.idWinner = eWin->get<CPlayer>()->id;
    outcome.idLoser = eLos->get<CPlayer>()->id;
}
void ActionSystem::preprocessManVsMan(ActionOutcome& outcome, Entity* e) {
    Entity* P1 = war.getPlayer(1);
    Entity* P2 = war.getPlayer(2);

    EntityList P1alive = getAliveUnits(P1);
    EntityList P2alive = getAliveUnits(P2);

    Entity* eP1Fighter = pickRandom(P1alive);
    Entity* eP2Fighter = pickRandom(P2alive);

    outcome.idActors.push_back(eP1Fighter->get<CUnit>()->armyID);
    outcome.idActors.push_back(eP2Fighter->get<CUnit>()->armyID);

    CUnit::DamageType P1damage = eP1Fighter->get<CUnit>()->damage;
    CUnit::DamageType P2damage = eP2Fighter->get<CUnit>()->damage;

    double P1power = eP1Fighter->get<CUnit>()->resistance[P2damage];
    double P2power = eP2Fighter->get<CUnit>()->resistance[P1damage];

    double randomNumber = randomDouble(0.0, P1power + P2power);
    if (randomNumber <= P1power) {
        outcome.idWinner = 1;
        outcome.idLoser = 2;
    } else {
        outcome.idWinner = 2;
        outcome.idLoser = 1;
    }
}

void ActionSystem::preprocessEnslave(ActionOutcome& outcome, Entity* e) {
    Entity* eWin = e;
    Entity* eLos = e->get<CPlayer>()->enemy;

    outcome.idWinner = eWin->get<CPlayer>()->id;
    outcome.idLoser = eLos->get<CPlayer>()->id;
}

void ActionSystem::preprocessMercy(ActionOutcome& outcome, Entity* e) {
    Entity* eWin = e;
    Entity* eLos = e->get<CPlayer>()->enemy;

    outcome.idWinner = eWin->get<CPlayer>()->id;
    outcome.idLoser = eLos->get<CPlayer>()->id;
}

void ActionSystem::playBuffDebuff(Entity* e, CUnit::DamageType dmg, bool buff) {
    Entity* eOut = eManager->createEntity();
    CAction::ID actID;
    if      (dmg == CUnit::FIRE && buff) actID = 204;
    else if (dmg == CUnit::WATER && buff) actID = 205;
    else if (dmg == CUnit::EARTH && buff) actID = 206;
    else if (dmg == CUnit::AIR && buff) actID = 232;
    else if (dmg == CUnit::FIRE && !buff) actID = 207;
    else if (dmg == CUnit::WATER && !buff) actID = 208;
    else if (dmg == CUnit::EARTH && !buff) actID = 209;
    else if (dmg == CUnit::AIR && !buff) actID = 233;
    eOut->add(new CActionOutcome(actID, e));

    EntityList alive = getAliveUnits(e);
    double chance = 0.3;
    for(EntityListIt i = alive.begin(); i != alive.end(); i++) {
        Entity* eUnit = *i;
        if (randomDouble(0, 1) <= chance) {
            eUnit->get<CUnit>()->effects.insert(make_pair(actID, CAction::Map[actID]));
            eOut->get<CActionOutcome>()->targets.push_back(eUnit);
        }
    }

    notify(START_ANIMATION, eOut);
}


void ActionSystem::doNothing(Entity* e) {
    Entity* eOut = eManager->createEntity();
    CAction::ID actID = -1;
    eOut->add(new CActionOutcome(actID, e));
    notify(SCENE_ENDED);
}

void ActionSystem::orderTargets(EntityList& targets, map<CAction::ID, CAction>& effects, CUnit::DamageType dmg) {
    if (mapContains(effects, 301)) {
        switch(dmg) {
        case CUnit::FIRE:
            targets.sort(lowerFireResistance);
            break;
        case CUnit::WATER:
            targets.sort(lowerWaterResistance);
            break;
        case CUnit::EARTH:
            targets.sort(lowerEarthResistance);
            break;
        case CUnit::AIR:
            targets.sort(lowerAirResistance);
            break;
        default:
            break;
        }
    } else if (mapContains(effects, 302)) {
        switch(dmg) {
        case CUnit::FIRE:
            targets.sort(higherFireResistance);
            break;
        case CUnit::WATER:
            targets.sort(higherWaterResistance);
            break;
        case CUnit::EARTH:
            targets.sort(higherEarthResistance);
            break;
        case CUnit::AIR:
            targets.sort(higherAirResistance);
            break;
        default:
            break;
        }
    } else {
        EntityList ordered = pickNRandom(targets, targets.size());
        targets = ordered;
    }
    /*
        if (mapContains(effects, 210)) targets.sort(prioritizeFire);
        if (mapContains(effects, 211)) targets.sort(prioritizeWater);
        if (mapContains(effects, 212)) targets.sort(prioritizeEarth);
        if (mapContains(effects, 234)) targets.sort(prioritizeAir);
    */
}

bool ActionSystem::mapContains(map<CAction::ID, CAction>& m, CAction::ID id) {
    map<CAction::ID, CAction>::iterator it;
    it = m.find(id);
    return it != m.end();
}

void ActionSystem::throwCoin() {
    //unused function:
    /*
        Entity* eFirst = nullptr;
        Entity* eSecon = nullptr;
        if (randomDouble(0,1) <= 0.5){
            eFirst = war.getPlayer1();
            eSecon = war.getPlayer2();
        }else{
            eFirst = war.getPlayer2();
            eSecon = war.getPlayer1();
        }
        e->get<CBattle>()->eFirst = eFirst;

        Entity* eOut = eManager->createEntity();
        eOut->add(new CActionOutcome(505, e));
        eOut->get<CActionOutcome>()->eWinner = eFirst;
        eOut->get<CActionOutcome>()->eLoser = eSecon;
        notify(START_ANIMATION, eOut);
    */
}

