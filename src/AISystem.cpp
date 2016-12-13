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
        if (e->get<CAInt>()->actionSelected){
            notify(ACTION_SELECTED);
            e->get<CAInt>()->actionSelected = false;
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
        //showUnitOptions(eActor);
    }else if (war.getSystemAction() == War::ASK_CAPTAIN_SELECTION){
        selectHero(eActor);
    }else if (war.getSystemAction() == War::ASK_FORMATION){
        selectFormation(eActor);
    }else if (war.getSystemAction() == War::ASK_CAPTAIN_ACTION){
        selectHeroAction(eActor);
    }else if (war.getSystemAction() == War::ASK_ARMY_ACTION){
        selectArmyAction(eActor);
    }else if (war.getSystemAction() == War::ASK_BATTLE_CLOSURE){
        selectBattleClosure(eActor);
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
    }
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
