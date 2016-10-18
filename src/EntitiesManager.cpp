#include "EntitiesManager.h"

EntitiesManager::EntitiesManager(){

}

EntitiesManager::~EntitiesManager(){
    //dtor
}

void EntitiesManager::addEntity(Entity* e){
    if (contains(removedEntities, e)) return;
    addedEntities.push_back(e);
    e->status = Entity::ALIVE;
    notify(LIST_UPDATED);
}

Entity* EntitiesManager::createEntity(){
    Entity* e = new Entity();
    addEntity(e);
    return e;
}

void EntitiesManager::removeEntity(Entity* e){
    if (isDead(e)) return;
    if (contains(addedEntities, e)) addedEntities.remove(e);

    e->status = Entity::DEAD;

    EntityList eEmployers = e->getEmployers();
    for (Entity* eEmployer : eEmployers){
        e->detachEmployer(eEmployer);
    }

    EntityList eEmployees = e->getEmployees();
    for (Entity* eEmployee : eEmployees){
        removeEntity(eEmployee);
    }

    e->detachAllEmployees();
    removedEntities.push_back(e);
    notify(LIST_UPDATED);
}

void EntitiesManager::clearSystem(){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        if (isDead(e)) continue;
        if (!e->has(Component::SYSTEM)){
            removeEntity(e);
        }
    }
    notify(LIST_UPDATED);
}
void EntitiesManager::addModified(Entity* e){
    for(EntityListIt i = addedEntities.begin(); i != addedEntities.end(); i++){
        if (*i == e) return;
    }
    if (contains(removedEntities, e)) return;

    modifiedEntities.push_back(e);
    notify(LIST_UPDATED);
}

bool EntitiesManager::listen(Event ev){
    std::list<Event>::iterator it;
    for (it = events.begin(); it != events.end(); it++){
        if (*it == ev) return true;
    }
    return false;
}

void EntitiesManager::notify(Event ev){
    events.push_back(ev);
}
void EntitiesManager::clearEvents(){
    events.clear();
}

void EntitiesManager::updateList(){
    for(Entity* e : entities){
        if (e->status == Entity::UNKNOWN) printf("Warning: there's an UNKOWN entity in the system\n");
        if (e == nullptr) printf("Warning: there's a nullptr entity in the system!\n");
    }

    std::list<Entity*>::iterator it;
    for (it = removedEntities.begin(); it != removedEntities.end(); it++){
        Entity* e = *it;
        clearActorScript(e);
        entities.remove(e);
        if (e != nullptr) delete e;
        e = nullptr;
    }
    for (it = addedEntities.begin(); it != addedEntities.end(); it++){
        entities.push_back(*it);
    }

    addedEntities.clear();
    removedEntities.clear();
    modifiedEntities.clear();
}

bool EntitiesManager::updated(){
    return (!addedEntities.empty() || !removedEntities.empty() || !modifiedEntities.empty());
}

bool EntitiesManager::isDead(Entity* e){
    if (e == nullptr) return true;
    if (e->status == Entity::DEAD) return true;
    if (e->status == Entity::ALIVE) return false;
    for (EntityListIt i = addedEntities.begin(); i != addedEntities.end(); i++){
        if (e == *i) return false;
    }
    for (EntityListIt i = removedEntities.begin(); i != removedEntities.end(); i++){
        if (e == *i) return true;
    }
    bool dead = true;
    for (EntityListIt i = entities.begin(); i != entities.end(); i++){
        if (e == *i){
            dead = false;
            break;
        }
    }
    return dead;
}

int EntitiesManager::getCount(){
    return entities.size();
}

void EntitiesManager::clearAll(){
    for(Entity* e : entities){
        if (!isDead(e)) removeEntity(e);
    }
}

void EntitiesManager::clearActorScript(Entity* e){
    if (e->has(Component::ACTOR)){
        for (list<AnimationNode*>::iterator it = e->get<CActor>()->timeline.begin(); it != e->get<CActor>()->timeline.end(); it++){
            delete *it;
        }
        e->get<CActor>()->timeline.clear();
    }
}


