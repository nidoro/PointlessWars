#include "TimerSystem.h"

TimerSystem::TimerSystem(){
    addRequirement(Component::TIMER);
    addSubscription(REMOVE_ENTITY);
}

TimerSystem::~TimerSystem(){
    //dtor
}

void TimerSystem::update(){
    for (EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        if (eManager->isDead(e)) continue;
        if (e->get<CTimer>()->clock.getElapsedTime().asSeconds() >= e->get<CTimer>()->target){
            notify(e->get<CTimer>()->message, e);
            if (e->get<CTimer>()->removeEntity){
                eManager->removeEntity(e);
            }else{
                e->remove(Component::TIMER);
                eManager->addModified(e);
            }
        }
    }
}

void TimerSystem::onRemoveEntity(Entity* e){
    eManager->removeEntity(e);
}
