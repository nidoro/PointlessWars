#include "AISystem.h"

AISystem::AISystem(){
    addSubscription(START_BATTLE_STATE);
    addSubscription(SYSTEM_ACTION);
    addRequirement(Component::AI);
}

AISystem::~AISystem(){
    //dtor
}

void AISystem::update(){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        if (e->get<CAInt>()->waitingOnBrain){
            if (e->get<CAInt>()->brainClock.getElapsedTime().asSeconds() >= e->get<CAInt>()->brainDelay){
                e->get<CAInt>()->actionSelected = true;
                e->get<CAInt>()->waitingOnBrain = false;
            }
        }
        
        if (e->get<CAInt>()->actionSelected){
            notify(ACTION_SELECTED);
            //e->get<CAInt>()->actionSelected = false;
        }
    }
}

void AISystem::onSystemAction(Entity* e){
    /*
    if (war.getSystemAction() == War::SHOW_HERO_POOL){
        showHeroPool();
    }else if (war.getSystemAction() == War::END_HERO_POOL){
        endHeroPool();
    }
    */

    Entity* eActor = war.getActor();
    if (!eActor) return;
    if (!eActor->has(Component::AI))return;

    if (war.getSystemAction() == War::ASK_ARMY_ASSEMBLE){
        assembleArmy(eActor);
        eActor->get<CAInt>()->brainDelay = randomDouble(1.0, 2.0);
    }else if (war.getSystemAction() == War::ASK_HERO_PICK){
        pickHeroFromPool(eActor);
        eActor->get<CAInt>()->brainDelay = randomDouble(1.0, 2.0);
    }else if (war.getSystemAction() == War::ASK_CAPTAIN_SELECTION){
        selectHero(eActor);
        eActor->get<CAInt>()->brainDelay = 0.f;
    }else if (war.getSystemAction() == War::ASK_FORMATION){
        selectFormation(eActor);
        eActor->get<CAInt>()->brainDelay = 0.f;
    }else if (war.getSystemAction() == War::ASK_CAPTAIN_ACTION){
        selectHeroAction(eActor);
        eActor->get<CAInt>()->brainDelay = randomDouble(0.0, 2.0);
    }else if (war.getSystemAction() == War::ASK_ARMY_ACTION){
        selectArmyAction(eActor);
        eActor->get<CAInt>()->brainDelay = randomDouble(0.0, 2.0);
    }else if (war.getSystemAction() == War::ASK_BATTLE_CLOSURE){
        selectBattleClosure(eActor);
        eActor->get<CAInt>()->brainDelay = randomDouble(0.5, 2.0);
    }else if (war.getSystemAction() == War::PRESENT_ARMY){
        //presentArmy(eActor);
    }else if (war.getSystemAction() == War::ADVANCE_ARMY){
        //advanceArmy(eActor);
    }else if (war.getSystemAction() == War::FIELD_CLEANUP){
        //cleanupField(eActor);
    }else if (war.getSystemAction() == War::SHOW_HERO_POOL){
        //showHeroPool();
    }else if (war.getSystemAction() == War::ASK_HERO_PICK){
        //askHeroPick(eActor);
    }else if (war.getSystemAction() == War::ASSIGN_RANDOM_ARMY){
        eActor->get<CAInt>()->brainDelay = 0.f;
    }
    
    eActor->get<CAInt>()->actionSelected = false;
    eActor->get<CAInt>()->brainClock.restart();
    eActor->get<CAInt>()->waitingOnBrain = true;
    
}

void AISystem::selectBattleClosure(Entity* e){
    vector<CAction::ID> actions;
    Entity* eCap = e->get<CArmy>()->captain;

    for(int i = 400; i <= 403; i++){
        actions.push_back(i);
    }

    //war.getNextActionOutcome(e->get<CPlayer>()->id).action = actions[randomInt(0, actions.size()-1)];
    war.getNextActionOutcome(e->get<CPlayer>()->id).action = 400;
    e->get<CAInt>()->actionSelected = true;
}

void AISystem::selectHero(Entity* e){
    vector<CAction::ID> actions;
    for(map<CCaptain::ID, Entity*>::iterator i = e->get<CArmy>()->captains.begin(); i != e->get<CArmy>()->captains.end(); i++){
        if (i->second->get<CCaptain>()->isConfined) continue;
        actions.push_back(i->first+100);
    }

    //e->get<CArmy>()->nextAction = actions[randomInt(0, actions.size()-1)];
    war.getNextActionOutcome(e->get<CPlayer>()->id).action = actions[randomInt(0, actions.size()-1)];
    e->get<CAInt>()->actionSelected = true;
}

void AISystem::selectFormation(Entity* e){
    vector<CAction::ID> actions;
    Entity* eCap = e->get<CArmy>()->captain;

    for(int i = 300; i <= 302; i++){
        if (e->get<CArmy>()->prohibitedFormation == i-300) continue;
        actions.push_back(i);
    }

    if (contains(eCap->get<CCaptain>()->actions, 226)) actions.push_back(303);

    war.getNextActionOutcome(e->get<CPlayer>()->id).action = actions[randomInt(0, actions.size()-1)];
    e->get<CAInt>()->actionSelected = true;
}

void AISystem::selectHeroAction(Entity* e){
    vector<CAction::ID> actions;
    Entity* eCap = e->get<CArmy>()->captain;

    for(list<CAction::ID>::iterator i = eCap->get<CCaptain>()->actions.begin(); i != eCap->get<CCaptain>()->actions.end(); i++){
        if (*i == 226) continue;
        actions.push_back(*i);
    }

    war.getNextActionOutcome(e->get<CPlayer>()->id).action = actions[randomInt(0, actions.size()-1)];
    e->get<CAInt>()->actionSelected = true;
}

void AISystem::selectArmyAction(Entity* e){
    vector<CAction::ID> actions;
    EntityList alive = getAliveUnits(e);
    for (EntityListIt i = alive.begin(); i != alive.end(); i++){
        Entity* eUnit = *i;
        if (!contains(actions, eUnit->get<CUnit>()->action) && eUnit->get<CUnit>()->action != -1){
            actions.push_back(eUnit->get<CUnit>()->action);
        }
    }
    if (actions.empty())  e->get<CArmy>()->nextAction = -1;
    war.getNextActionOutcome(e->get<CPlayer>()->id).action = actions[randomInt(0, actions.size()-1)];
    e->get<CAInt>()->actionSelected = true;
}

void AISystem::assembleArmy(Entity* e){
    if (!war.getMatchConfig().randomArmy){
        list<CUnit::ID> options;
        Entity* eOptions = eManager->createEntity();
        list<CUnit::ID> unitDeck;
        for (list<CUnit::ID>::iterator it = e->get<CPlayer>()->unitDeck.begin(); it != e->get<CPlayer>()->unitDeck.end(); it++){
            if (CUnit::Map[*it].damage != e->get<CArmy>()->prohibitedDamageType){
                //unitDeck.remove(*it);
                unitDeck.push_back(*it);
            }else{
                //it++;
            }
        }
        selectRandomUnits(options, unitDeck, war.getMatchConfig().nUnitOpt);

        e->get<CArmy>()->unitCount.clear();
        map<CUnit::ID, CUnit>& units = CUnit::Map;
        
        //choose randomly
        int nUnits = 0;
        while (nUnits < war.getMatchConfig().armySize){
            CUnit::ID uID = getRandom(options);
            map<CUnit::ID, int>::iterator it = e->get<CArmy>()->unitCount.find(uID);
            if (it != e->get<CArmy>()->unitCount.end()){
                it->second += war.getMatchConfig().recruitGroup;
            }else{
                e->get<CArmy>()->unitCount[uID] = war.getMatchConfig().recruitGroup;
            }
            nUnits += war.getMatchConfig().recruitGroup;
        }
        
        war.getNextActionOutcome(e->get<CPlayer>()->id).action = 511;
    }
}

void AISystem::selectRandomUnits(list<CUnit::ID>& output, list<CUnit::ID>& input, int n){
    output.clear();
    list<CUnit::ID> deck = input;
    n = min(n, (int) deck.size());
    for(int i = 0; i < n; i++){
        output.push_back(popRandom(deck));
    }
}

void AISystem::pickHeroFromPool(Entity* e){
    war.getNextActionOutcome(e->get<CPlayer>()->id).hero = popRandom(e->get<CPlayer>()->heroPool);
    war.getNextActionOutcome(e->get<CPlayer>()->id).action = war.getNextActionOutcome(e->get<CPlayer>()->id).hero + 600;
    e->get<CAInt>()->actionSelected = true;
}

EntityList AISystem::getAliveUnits(Entity* e){
    EntityList l;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++){
        Entity* eUnit = *i;
        if (!eUnit->get<CUnit>()->dead) l.push_back(eUnit);
    }
    return l;
}

EntityList AISystem::getDeadUnits(Entity* e){
    EntityList l;
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++){
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) l.push_back(eUnit);
    }
    return l;
}
