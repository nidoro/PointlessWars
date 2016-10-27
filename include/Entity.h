#ifndef ENTITY_H
#define ENTITY_H
#include "Component.h"
#include <vector>
#include <cstdlib>

typedef std::list<Entity*> EntityList;
typedef std::list<Entity*>::iterator EntityListIt;

class Entity{
    public:
        Entity();
        ~Entity();
        enum Status{UNKNOWN, ALIVE, DEAD};

        bool has(Component::Type component);

        template <typename T>
        T* get(){
            Component::Type c = T::getType();
            if (has(c)){
                return static_cast<T*> (components[c]);
            }else{
                return nullptr;
            }
        }

        template <typename T>
        void add(T* component){
            if (has(component->getType())) remove(component->getType());
            components[component->getType()] = component;
        }

        void add(Component* component, Component::Type type){
            if (has(type)) remove(type);
            components[type] = component;
        }

        void remove(Component::Type component);
        bool isEmpty();
        Status status;

        void attachEmployee(Entity* e);
        void detachEmployee(Entity* e);
        void detachAllEmployees();

        void attachEmployer(Entity* e);
        void detachEmployer(Entity* e);
        void detachAllEmployers();

        EntityList& getEmployees();
        EntityList& getEmployers();

        Entity* getObservedEntity(string id);
        EntityList getObservedEntities();
        void addObservedEntity(string id, Entity* e);
        void addObservedEntity(Entity* e);
        void removeObservedEntity(string id);
        void removeObservedEntity(Entity* e);

    private:
        std::vector<Component*> components;
        list<Entity*> employees;
        list<Entity*> employers;

        map<string, Entity*> observedEntities;
        int nextObservedID;
};

Entity* pickRandom(EntityList& L);
EntityList pickNRandom(EntityList& L, int n);
bool contains(EntityList& L, Entity* value);
void pushBack(EntityList& L1, EntityList L2);


#endif // ENTITY_H
