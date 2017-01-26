#include "Entity.h"

Entity::Entity() {
    components.resize(Component::NUM_COMPONENTS, nullptr);
    status = UNKNOWN;
    nextObservedID = 0;
}

Entity::~Entity() {
    for (int i = 0; i < Component::NUM_COMPONENTS; i++) {
        if (components[i] != nullptr) {
            remove((Component::Type)i);
        }
    }
}

bool Entity::has(Component::Type c) {
    return (components[c] != nullptr);
}

void Entity::remove(Component::Type component) {
    delete components[component];
    components[component] = nullptr;
}

bool Entity::isEmpty() {
    for(int i = 0; i < Component::NUM_COMPONENTS; i++) {
        if (has((Component::Type)i)) {
            return false;
        }
    }
    return true;
}

Entity* pickRandom(list<Entity*>& L) {
    if (L.empty()) return nullptr;
    list<Entity*>::iterator it = L.begin();
    int r = randomInt(0, L.size()-1);
    std::advance(it, r);
    return *it;
}

EntityList pickNRandom(EntityList& L, int n) {
    if (L.empty()) return L;
    EntityList LC = L;
    EntityList LL;
    int c = 0;
    while(++c <= n) {
        if ((unsigned) c > L.size()) {
            LC = L;
        }
        Entity* el = pickRandom(LC);
        LL.push_back(el);
        LC.remove(el);
    }
    return LL;
}

bool contains(list<Entity*>& L, Entity* value) {
    for(list<Entity*>::iterator it = L.begin(); it != L.end(); it++) {
        if (*it == value) return true;
    }
    return false;
}

void pushBack(list<Entity*>& L1, EntityList L2) {
    for(EntityListIt i = L2.begin(); i != L2.end(); i++) {
        L1.push_back(*i);
    }
}

void Entity::attachEmployee(Entity* e) {
    if (!contains(employees, e)) employees.push_back(e);
    if (!contains(e->getEmployers(), this)) e->attachEmployer(this);
}
void Entity::detachEmployee(Entity* e) {
    if (contains(employees, e)) employees.remove(e);
    if (contains(e->getEmployers(), this)) e->detachEmployer(this);
}
void Entity::detachAllEmployees() {
    employees.clear();
}

void Entity::attachEmployer(Entity* e) {
    if (!contains(employers, e)) employers.push_back(e);
    if (!contains(e->getEmployees(), this)) e->attachEmployee(this);
}
void Entity::detachEmployer(Entity* e) {
    if (contains(employers, e)) employers.remove(e);
    if (contains(e->getEmployees(), this)) e->detachEmployee(this);
}
void Entity::detachAllEmployers() {
    employers.clear();
}

EntityList& Entity::getEmployees() {
    return employees;
}

EntityList& Entity::getEmployers() {
    return employers;
}

Entity* Entity::getObservedEntity(string id) {
    map<string, Entity*>::iterator it = observedEntities.find(id);
    if (it != observedEntities.end()) {
        return it->second;
    } else {
        //WARNING_MESSAGE("Observed entity not found: " + id);
        return nullptr;
    }
}
EntityList Entity::getObservedEntities(string prefix) {
    EntityList l;
    for (auto& i : observedEntities) {
        string id = i.first;
        if (prefix.empty() || hasBegining(id, prefix)) l.push_back(i.second);
    }
    if (l.empty()) {
        //WARNING_MESSAGE("Observed entities not found: " + prefix);
    }
    return l;
}
void Entity::addObservedEntity(string id, Entity* e) {
    observedEntities[id] = e;
}
void Entity::addObservedEntity(Entity* e) {
    observedEntities[int2str(nextObservedID++)] = e;
}

void Entity::removeObservedEntity(string id) {
    map<string, Entity*>::iterator it = observedEntities.find(id);
    if (it != observedEntities.end()) observedEntities.erase(it);
}

void Entity::removeObservedEntity(Entity* e) {
    map<string, Entity*>::iterator it = observedEntities.begin();
    for(    ; it != observedEntities.end(); it++) {
        if (it->second == e) break;
    }
    if (it != observedEntities.end()) observedEntities.erase(it);
}
