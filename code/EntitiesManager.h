#ifndef ENTITIESMANAGER_H
#define ENTITIESMANAGER_H
#include "Entity.h"
#include "Component.h"

class EntitiesManager {
    public:
        enum Event {
            LIST_UPDATED,
            GAME_SCREEN_UPDATED
        };
        EntitiesManager();
        ~EntitiesManager();
        void addEntity(Entity* e);
        void removeEntity(Entity* e);
        void addModified(Entity* e);
        void clearSystem();
        bool listen(Event ev);
        void notify(Event ev);
        void clearEvents();
        void updateList();
        bool updated();
        void clearActorScript(Entity* e);
        std::list<Entity*> addedEntities;
        std::list<Entity*> removedEntities;
        std::list<Entity*> modifiedEntities;
        Entity* createEntity();
        bool isDead(Entity* e);
        int getCount();
        void clearAll();

    private:
        std::list<Entity*> entities;
        std::list<Event> events;

};

#endif // ENTITIESMANAGER_H
