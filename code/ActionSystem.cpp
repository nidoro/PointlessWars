#include "ActionSystem.h"

ActionSystem::ActionSystem() {
    addSubscription(SCENE_ENDED);
}

void ActionSystem::update() {
    if (!war.getPlaying()) return;
    for(int i = 0; i < 3; i++) {
        if (!war.getNextActionOutcome(i).ready && war.getNextActionOutcome(i).action >= 0 && !war.getRemotelyControled(i)) {
            if (i > 0 && war.getPlayer(i)->has(Component::AI)) {
                if (war.getPlayer(i)->get<CAInt>()->actionSelected) {
                    // @note: PREPROCESS ACTION
                    preprocessAction(i);
                    // @note: ACTION READY TO BE PLAYED
                    war.getNextActionOutcome(i).ready = true;
                    war.getPlayer(i)->get<CAInt>()->actionSelected = false;
                }
            } else {
                // @note: PREPROCESS ACTION
                preprocessAction(i);
                // @note: ACTION READY TO BE PLAYED
                war.getNextActionOutcome(i).ready = true;
            }
        }

        // @note: IF ACTION IS READY TO BE PLAYED AND PLAYER IS READY TO PLAY
        if (war.getNextActionOutcome(i).ready && war.getPlayerReady(i)) {
            // @note: EXECUTE ACTION
            executeAction(i);
            /// @note: set action completed and player is not ready to act again until next state
            war.setActionCompleted(i, true);
            war.setPlayerReady(i, false);
        }
    }
}

void ActionSystem::preprocessAction(CPlayer::ID id) {
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
    case 201:
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
        //playPresentArmy(war.getPlayer(id));
        break;
    case 502:
        //playAdvanceArmy(war.getPlayer(id));
        break;
    case 503:
        //playReturnArmy(war.getPlayer(id));
        break;
    case 504:
        //playRemoveDead(war.getPlayer(id));
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

    // @cleanup: temporary way of avoiding random composition
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
    for (EntityListIt i = alive.begin(); i != alive.end(); i++) {
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
    // @cleanup: temporary way of avoiding randomness
#if 1
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
    int nRes = randomInt(1, 5);
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

    for (EntityListIt i = targets.begin(); i != targets.end(); i++) {
        Entity* eUnit = *i;
        outcome.idTargets.push_back(eUnit->get<CUnit>()->armyID);
    }
}

void ActionSystem::preprocessCurse(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    EntityList candidates = getAliveUnits(eEnemy);
    int maxTargets = min(20, (int) candidates.size());
    int nTargets = randomInt(1, maxTargets);
    for (int i = 0; i < nTargets; i++) {
        Entity* eUnit = pickRandom(candidates);
        candidates.remove(eUnit);
        outcome.idTargets.push_back(eUnit->get<CUnit>()->armyID);
    }
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
    // @todo: no preprocess required?
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
    // @todo: no preprocess required
}

void ActionSystem::preprocessTeleport(ActionOutcome& outcome, Entity* e) {
    // @todo: teleport
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
    // @todo: no preprocess
    outcome.hero = 10;
}

void ActionSystem::preprocessCroak(ActionOutcome& outcome, Entity* e) {
    // @clenaup
#if 1
    double chance = 0.50;
#else
    double chance = 0.f;
#endif
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
    CUnit::DamageType dmg = CUnit::DamageType::N_DAMAGE_TYPES;
    CUnit::ID uID = outcome.action;

    for (EntityListIt i = aliveAtk.begin(); i != aliveAtk.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->id == uID) {
            unitsAtk.push_back(eUnit);
            dmg = eUnit->get<CUnit>()->damage;
        }
    }

    //Backfire due to curse
    float curseSuccessChance = 1.0;
    for (auto eUnit : unitsAtk) {
        if (isCursed(eUnit)) {
            if (randomDouble(0, 1) < curseSuccessChance) {
                outcome.idBackfire.push_back(eUnit->get<CUnit>()->armyID);
                unitsAtk.remove(eUnit);
            }
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

    for (EntityListIt i = unitsDef.begin(); i != unitsDef.end(); i++) {
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

bool ActionSystem::isCursed(Entity* e) {
    auto it = e->get<CUnit>()->effects.find(222);
    return it != e->get<CUnit>()->effects.end();
}
