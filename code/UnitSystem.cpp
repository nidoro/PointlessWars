#include "UnitSystem.h"

UnitSystem::UnitSystem() {
    addRequirement(Component::UNIT);
    addRequirement(Component::OWNER);
}

UnitSystem::~UnitSystem() {
}

void UnitSystem::update() {
    for (EntityListIt i = entities.begin(); i != entities.end(); i++) {
        Entity* e = *i;
        for(int j = 0; j < CUnit::N_DAMAGE_TYPES; j++) {
            e->get<CUnit>()->realResist[j] = getResistance(e, (CUnit::DamageType)j);
        }
    }
}

int UnitSystem::getResistance(Entity* e, CUnit::DamageType type) {
    int r = e->get<CUnit>()->resistance[type];
    if (type == CUnit::FIRE && affectedBy(e, 204)) r += 1;
    if (type == CUnit::FIRE && affectedBy(e, 207)) r -= 1;
    if (type == CUnit::WATER && affectedBy(e, 205)) r += 1;
    if (type == CUnit::WATER && affectedBy(e, 208)) r -= 1;
    if (type == CUnit::EARTH && affectedBy(e, 206)) r += 1;
    if (type == CUnit::EARTH && affectedBy(e, 209)) r -= 1;
    if (type == CUnit::AIR && affectedBy(e, 232)) r += 1;
    if (type == CUnit::AIR && affectedBy(e, 233)) r -= 1;
    if (affectedBy(e, 302)) {
        r += 1;
    }
    if (affectedBy(e, 301)) {
        r -= 1;
    }

    r = min(r, war.getMatchConfig().maxResistance);
    r = max(r, 0);

    return r;
}

bool UnitSystem::mapContains(map<CAction::ID, CAction>& m, CAction::ID id) {
    map<CAction::ID, CAction>::iterator it;
    it = m.find(id);
    return it != m.end();
}

bool UnitSystem::affectedBy(Entity* e, CAction::ID id) {
    Entity* eArmy = e->get<COwner>()->e;
    if (mapContains(e->get<CUnit>()->effects, id) || mapContains(eArmy->get<CArmy>()->armyEffects, id)) {
        return true;
    } else {
        return false;
    }
}
