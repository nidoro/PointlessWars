#include "TurnTimerSystem.h"

TurnTimerSystem::TurnTimerSystem() {
    addSubscription(SYSTEM_ACTION);
    addSubscription(SELECT_ACTION);
    addSubscription(ACTION_SELECTED);
    
    addRequirement(Component::TURN_TIMER);
}

void TurnTimerSystem::update() {
    for (Entity* e : entities) {
        e->get<CTurnTimer>()->remaining -= delay;
        if (e->get<CTurnTimer>()->remaining <= 0) {
            notify(PLAYER_TURN_TIME_EXPIRED);
            eManager->removeEntity(e);
        } else {
            e->get<CTextbox2>()->content.setString(int2str((int) e->get<CTurnTimer>()->remaining));
        }
    }
}

void TurnTimerSystem::createTimer(float length) {
    Entity* e = eManager->createEntity();
    e->add(new CPosition(cxWindow, 200));
    e->add(new CDraw(CDraw::GUI3));
    e->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::Black));
    e->add(new CTurnTimer(length));
}

void TurnTimerSystem::onSystemAction(Entity* e) {
    Entity* ePlayer = war.getActor();

    if (war.getSystemAction() == war.ASK_HERO_PICK) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_FORMATION) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_CAPTAIN_ACTION) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_ARMY_ACTION) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_BATTLE_CLOSURE) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_ARMY_ASSEMBLE) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_CAPTAIN_SELECTION) {
        createTimer(10);
    }
}

void TurnTimerSystem::onSelectAction(Entity* e) {
    removeTimer();
}

void TurnTimerSystem::onActionSelected(Entity* e) {
    //@cleanup
    printf("Action selected\n");
    removeTimer();
}

void TurnTimerSystem::removeTimer() {
    if (!entities.empty() && !eManager->isDead(entities.front())) {
        eManager->removeEntity(entities.front());
    }
}
