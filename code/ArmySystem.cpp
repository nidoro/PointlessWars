#include "ArmySystem.h"

ArmySystem::ArmySystem() {
    addRequirement(Component::ARMY);

    addSubscription(PLAYER_CREATED);
    addSubscription(INITIALIZE_PLAYER);
    addSubscription(BATTLE_FIELD_CREATED);
    addSubscription(NEXT_BATTLE_PHASE);
    addSubscription(UNIT_DIED);
    addSubscription(START_AFTER_BATTLE_CLEANUP);
    addSubscription(RESTART_BATTLE);
    addSubscription(CREATE_RANDOM_ARMIES);
    addSubscription(ASSIGN_RANDOM_ARMY);
    addSubscription(CREATE_ARMY);
    addSubscription(RECOMPOSE_ARMY);
    addSubscription(START_BATTLE_STATE);
    addSubscription(SYSTEM_ACTION);
    //addSubscription(PICK_HERO);
}

ArmySystem::~ArmySystem() {
    //dtor
}

void ArmySystem::update() {
    for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
        Entity* e = *i;
        updateUnitCount(e);
    }
}

void ArmySystem::onPickHero(Entity* e) {
    Entity* eArmy = e->get<CParentPanel>()->e->get<COwner>()->e;

    CCaptain::ID id = e->get<CCaptain>()->id;
    eArmy->get<CPlayer>()->heroDeck.push_back(id);

    //eManager->removeEntity(e);
    //captainPool.remove(e);

    //eChooser = eChooser->get<CPlayer>()->enemy;
    //eTurnDisplayer->get<CTextbox2>()->content.setString(eChooser->get<CPlayer>()->name + " is choosing a captain");
}

void ArmySystem::onAssignRandomArmy(Entity* e) {
    clearArmy(e);
#if 0
    createRandomArmy(e);
#else
    double xOff = 500;
    e->get<CPosition>()->x = e->get<CPlayer>()->side == CPlayer::LEFT ? xOff : wWindow - xOff;
    e->get<CPosition>()->y = cyWindow;
    e->get<CArmy>()->unitCount.clear();

    if (e->get<CPlayer>()->side == CPlayer::LEFT) {
        e->get<CArmy>()->unitCount.insert(make_pair(1, 50));
    } else {
        e->get<CArmy>()->unitCount.insert(make_pair(2, 50));
    }
#endif
    createArmy(e);
    recordArmy(e);
}

void ArmySystem::onInitializePlayer(Entity* e) {
    e->get<CArmy>()->captains.clear();

    int sign = e->get<CArmy>()->side == CPlayer::LEFT ? -1:1;
    bool flip = e->get<CArmy>()->side == CPlayer::LEFT ? false:true;
    double xSpawn = cxWindow + sign*wWindow/2 + sign*80;
    double ySpawn = cyWindow;

    e->get<CArmy>()->x = 0;

    for(auto& i : e->get<CPlayer>()->heroDeck) {
        CCaptain::ID id = i;
        Entity* eCap = eManager->createEntity();
        eCap->add(new CCaptain(CCaptain::Map[id]));
        eCap->add(new CAnimation(flip, CCaptain::Map[id].aIdle));
        eCap->add(new CDraw(CDraw::WORLD_1));
        eCap->add(new CVelocity());
        eCap->add(new CPosition(xSpawn, ySpawn));
        eCap->add(new CActor());
        eCap->add(new COwner(e));
        eCap->get<CVelocity>()->speed = 80;

        e->get<CArmy>()->captains.insert(make_pair(id, eCap));
    }
}

void ArmySystem::createRandomArmy(Entity* e) {
    double xOff = 500;
    e->get<CPosition>()->x = e->get<CPlayer>()->side == CPlayer::LEFT ? xOff : wWindow - xOff;
    e->get<CPosition>()->y = cyWindow;

    int groupSize = 10;
    int nGroups = war.getMatchConfig().armySize/groupSize;

    e->get<CArmy>()->unitCount.clear();

    map<CUnit::ID, CUnit>& units = CUnit::Map;

    for(int i = 0; i < nGroups; i++) {
        list<CUnit::ID> deck = e->get<CPlayer>()->unitDeck;
        CUnit unit = units[getRandom(deck)];

        map<CUnit::ID, int>::iterator it = e->get<CArmy>()->unitCount.find(unit.id);
        if (it == e->get<CArmy>()->unitCount.end()) {
            e->get<CArmy>()->unitCount.insert(make_pair(unit.id, groupSize));
        } else {
            e->get<CArmy>()->unitCount[unit.id] += groupSize;
        }
    }
}

void ArmySystem::onCreateArmy(Entity* e) {
    Entity* eArmy = e->get<CCommandOption>()->eArmy;
    EntityList eOptions = e->get<CArmyComposition>()->eOptions;
    composeArmy(eArmy, eOptions);
    createArmy(eArmy);
    recordArmy(eArmy);
    e->get<CArmyComposition>()->composed = true;
}

void ArmySystem::onRecomposeArmy(Entity* e) {
    recomposeArmy(e);
    createArmy(e);
    recordArmy(e);
}

void ArmySystem::composeArmy(Entity* eArmy, EntityList eOptions) {
    double xOff = 500;
    eArmy->get<CPosition>()->x = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? xOff : wWindow - xOff;
    eArmy->get<CPosition>()->y = cyWindow;

    eArmy->get<CArmy>()->unitCount.clear();
    map<CUnit::ID, CUnit>& units = CUnit::Map;

    int nUnits = 0;
    for (EntityListIt i = eOptions.begin(); i != eOptions.end(); i++) {
        Entity* eOpt = *i;
        CUnit::ID id = eOpt->get<CUnit>()->id;
        if (eOpt->get<CSpinButton>()->value > 0) {
            eArmy->get<CArmy>()->unitCount.insert(make_pair(id, eOpt->get<CSpinButton>()->value));
            nUnits += eOpt->get<CSpinButton>()->value;
        }
    }

    int nRemaining = war.getMatchConfig().armySize - nUnits;
    for(int i = 0; i < nRemaining; i+=war.getMatchConfig().recruitGroup) {
        //Entity* eUnitGroup;
        list<CUnit::ID>& deck = eArmy->get<CPlayer>()->unitDeck;
        CUnit unit = units[getRandom(deck)];

        map<CUnit::ID, int>::iterator it = eArmy->get<CArmy>()->unitCount.find(unit.id);
        if (it == eArmy->get<CArmy>()->unitCount.end()) {
            eArmy->get<CArmy>()->unitCount.insert(make_pair(unit.id, war.getMatchConfig().recruitGroup));
        } else {
            eArmy->get<CArmy>()->unitCount[unit.id] += war.getMatchConfig().recruitGroup;
        }
    }

}

void ArmySystem::recomposeArmy(Entity* eArmy) {
    eArmy->get<CArmy>()->unitCount = eArmy->get<CArmy>()->lastArmy;
}

void ArmySystem::recordArmy(Entity* e) {
    e->get<CArmy>()->lastArmy = e->get<CArmy>()->unitCount;
}

void ArmySystem::createArmy(Entity* e) {
    e->get<CArmy>()->allUnits.clear();
    int sign = e->get<CArmy>()->side == CPlayer::LEFT ? -1:1;
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
}

void ArmySystem::onStartBattleState(Entity* e) {
    BattleState state = e->get<CBattle>()->state;
    if (state.id == state.BETWEEN_TURNS) {
        for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
            Entity* eArmy = *i;
            ///UNIT EFFECTS
            for(EntityListIt j = eArmy->get<CArmy>()->allUnits.begin(); j != eArmy->get<CArmy>()->allUnits.end(); j++) {
                Entity* eUnit = *j;
                map<CAction::ID, CAction>::iterator it;
                IntList toErase;
                for(it = eUnit->get<CUnit>()->effects.begin(); it != eUnit->get<CUnit>()->effects.end(); it++) {
                    CAction::ID id = it->first;
                    if (eUnit->get<CUnit>()->effects[id].duration > 0) eUnit->get<CUnit>()->effects[id].duration--;
                    if (eUnit->get<CUnit>()->effects[id].duration == 0) toErase.push_back(id);
                }

                for(IntListIt iter = toErase.begin(); iter != toErase.end(); iter++) {
                    eUnit->get<CUnit>()->effects.erase(eUnit->get<CUnit>()->effects.find(*iter));
                }
            }
            ///ARMY EFFECTS
            map<CAction::ID, CAction>::iterator it;
            IntList toErase;
            for(it = eArmy->get<CArmy>()->armyEffects.begin(); it != eArmy->get<CArmy>()->armyEffects.end(); it++) {
                CAction::ID id = it->first;
                if (eArmy->get<CArmy>()->armyEffects[id].duration > 0) eArmy->get<CArmy>()->armyEffects[id].duration--;
                if (eArmy->get<CArmy>()->armyEffects[id].duration == 0) toErase.push_back(id);
            }
            for(IntListIt iter = toErase.begin(); iter != toErase.end(); iter++) {
                eArmy->get<CArmy>()->armyEffects.erase(eArmy->get<CArmy>()->armyEffects.find(*iter));
            }
        }
    } else if (state.id == state.ENDING) {
        for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
            Entity* eArmy = *i;
            ///ARMY EFFECTS
            map<CAction::ID, CAction>::iterator it;
            IntList toErase;
            for(it = eArmy->get<CArmy>()->armyEffects.begin(); it != eArmy->get<CArmy>()->armyEffects.end(); it++) {
                CAction::ID id = it->first;
                if (eArmy->get<CArmy>()->armyEffects[id].duration == -1) toErase.push_back(id);
            }
            for(IntListIt iter = toErase.begin(); iter != toErase.end(); iter++) {
                eArmy->get<CArmy>()->armyEffects.erase(eArmy->get<CArmy>()->armyEffects.find(*iter));
            }
            ///CAPTAIN ACTIONS
            Entity* eCaptain = eArmy->get<CArmy>()->captain;
            CCaptain::ID idCap = eCaptain->get<CCaptain>()->id;
            for(IntListIt j = CCaptain::Map[idCap].actions.begin(); j != CCaptain::Map[idCap].actions.end(); j++) {
                CAction::ID idAct = *j;
                if (CAction::Map[idAct].oneTimeBattle && !contains(eCaptain->get<CCaptain>()->actions, idAct)) {
                    eCaptain->get<CCaptain>()->actions.push_back(idAct);
                }
            }
            eArmy->get<CArmy>()->captain = nullptr;
            eArmy->get<CArmy>()->ready = false;
        }
    } else if (state.id == state.BEGINING) {
        for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
            Entity* eArmy = *i;
            ///ARMY EFFECTS
            map<CAction::ID, CAction>::iterator it;
            IntList toErase;
            for(it = eArmy->get<CArmy>()->armyEffects.begin(); it != eArmy->get<CArmy>()->armyEffects.end(); it++) {
                CAction::ID id = it->first;
                if (eArmy->get<CArmy>()->armyEffects[id].duration == -2) toErase.push_back(id);
            }
            for(IntListIt iter = toErase.begin(); iter != toErase.end(); iter++) {
                eArmy->get<CArmy>()->armyEffects.erase(eArmy->get<CArmy>()->armyEffects.find(*iter));
            }
            eArmy->get<CArmy>()->ready = true;
        }
    }
    for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
        updateUnitCount(*i);
    }
}

void ArmySystem::onSystemAction(Entity* e) {
    if (war.getSystemAction() == war.RECOMPOSE_ARMY) {
        onRecomposeArmy(war.getActor());
    } else if (war.getSystemAction() == war.BETWEEN_TURNS) {
        for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
            Entity* eArmy = *i;
            ///UNIT EFFECTS
            for(EntityListIt j = eArmy->get<CArmy>()->allUnits.begin(); j != eArmy->get<CArmy>()->allUnits.end(); j++) {
                Entity* eUnit = *j;
                map<CAction::ID, CAction>::iterator it;
                IntList toErase;
                for(it = eUnit->get<CUnit>()->effects.begin(); it != eUnit->get<CUnit>()->effects.end(); it++) {
                    CAction::ID id = it->first;
                    if (eUnit->get<CUnit>()->effects[id].duration > 0) eUnit->get<CUnit>()->effects[id].duration--;
                    if (eUnit->get<CUnit>()->effects[id].duration == 0) toErase.push_back(id);
                }

                for(IntListIt iter = toErase.begin(); iter != toErase.end(); iter++) {
                    eUnit->get<CUnit>()->effects.erase(eUnit->get<CUnit>()->effects.find(*iter));
                }
            }
            ///ARMY EFFECTS
            map<CAction::ID, CAction>::iterator it;
            IntList toErase;
            for(it = eArmy->get<CArmy>()->armyEffects.begin(); it != eArmy->get<CArmy>()->armyEffects.end(); it++) {
                CAction::ID id = it->first;
                if (eArmy->get<CArmy>()->armyEffects[id].duration > 0) eArmy->get<CArmy>()->armyEffects[id].duration--;
                if (eArmy->get<CArmy>()->armyEffects[id].duration == 0) toErase.push_back(id);
            }
            for(IntListIt iter = toErase.begin(); iter != toErase.end(); iter++) {
                eArmy->get<CArmy>()->armyEffects.erase(eArmy->get<CArmy>()->armyEffects.find(*iter));
            }
        }
    } else if (war.getSystemAction() == war.ENDING) {
        for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
            Entity* eArmy = *i;
            ///ARMY EFFECTS
            map<CAction::ID, CAction>::iterator it;
            IntList toErase;
            for(it = eArmy->get<CArmy>()->armyEffects.begin(); it != eArmy->get<CArmy>()->armyEffects.end(); it++) {
                CAction::ID id = it->first;
                if (eArmy->get<CArmy>()->armyEffects[id].duration == -1) toErase.push_back(id);
            }
            for(IntListIt iter = toErase.begin(); iter != toErase.end(); iter++) {
                eArmy->get<CArmy>()->armyEffects.erase(eArmy->get<CArmy>()->armyEffects.find(*iter));
            }
            ///CAPTAIN ACTIONS
            Entity* eCaptain = eArmy->get<CArmy>()->captain;
            CCaptain::ID idCap = eCaptain->get<CCaptain>()->id;
            for(IntListIt j = CCaptain::Map[idCap].actions.begin(); j != CCaptain::Map[idCap].actions.end(); j++) {
                CAction::ID idAct = *j;
                if (CAction::Map[idAct].oneTimeBattle && !contains(eCaptain->get<CCaptain>()->actions, idAct)) {
                    eCaptain->get<CCaptain>()->actions.push_back(idAct);
                }
            }

            eArmy->get<CArmy>()->captain = nullptr;
            eArmy->get<CArmy>()->ready = false;
        }
    } else if (war.getSystemAction() == war.BEGINING) {
        for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
            Entity* eArmy = *i;
            ///ARMY EFFECTS
            map<CAction::ID, CAction>::iterator it;
            IntList toErase;
            for(it = eArmy->get<CArmy>()->armyEffects.begin(); it != eArmy->get<CArmy>()->armyEffects.end(); it++) {
                CAction::ID id = it->first;
                if (eArmy->get<CArmy>()->armyEffects[id].duration == -2) toErase.push_back(id);
            }
            for(IntListIt iter = toErase.begin(); iter != toErase.end(); iter++) {
                eArmy->get<CArmy>()->armyEffects.erase(eArmy->get<CArmy>()->armyEffects.find(*iter));
            }
            eArmy->get<CArmy>()->ready = true;
        }
    } else if (war.getSystemAction() == war.UPDATE_SCORE) {
        for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
            Entity* eArmy = *i;
            int score = eArmy->get<CArmy>()->score;
            CPlayer::ID id = eArmy->get<CPlayer>()->id;
            if (war.getBattleClosure() == war.ARMY_VS_ARMY
                    ||  war.getBattleClosure() == war.ALL_KILLED) {
                eArmy->get<CArmy>()->score = id == war.getBattleWinner() ? score+1 : 0;

            } else if (war.getBattleClosure() == war.CONFINE
                       ||  war.getBattleClosure() == war.MAN_VS_MAN) {
                //eArmy->get<CArmy>()->score = id == war.getBattleWinner() ? score : 0;

            } else if (war.getBattleClosure() == war.MERCY) {
                eArmy->get<CArmy>()->score = 0;

            }

            eArmy->get<CArmy>()->hasCoin = id == war.getFirstMover() ? true : false;
            notify(SCORE_UPDATED, eArmy);
        }
    } else if (war.getSystemAction() == war.ANNOUNCE_VICTORY) {
        for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
            (*i)->get<CArmy>()->armyEffects.clear();
        }
    }
    for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
        updateUnitCount(*i);
    }
}

void ArmySystem::clearArmy(Entity* e) {
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (!eManager->isDead(eUnit)) {
            eManager->removeEntity(eUnit);
        }
    }
    e->get<CArmy>()->allUnits.clear();
}

EntityList ArmySystem::getAliveUnits(Entity* e) {
    EntityList l;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (!eUnit->get<CUnit>()->dead) l.push_back(eUnit);
    }
    return l;
}

void ArmySystem::updateUnitCount(Entity* e) {
    e->get<CArmy>()->nAlive = 0;
    if (!e->get<CArmy>()->unitCount.empty()) {
        for(auto& i : e->get<CArmy>()->unitCount) {
            i.second = 0;
        }

        for(Entity* eUnit : e->get<CArmy>()->allUnits) {
            if (eUnit->get<CUnit>()->dead) continue;
            e->get<CArmy>()->unitCount[eUnit->get<CUnit>()->id]++;
            e->get<CArmy>()->nAlive++;
        }
    }
}
