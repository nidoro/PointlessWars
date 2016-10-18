#ifndef UNITSYSTEM_H
#define UNITSYSTEM_H

#include "System.h"
#include "Standard.h"


class UnitSystem : public System{
    public:
        UnitSystem();
        ~UnitSystem();

        void update();
    private:

        int getResistance(Entity* e, CUnit::DamageType type);
        bool mapContains(map<CAction::ID, CAction>& m, CAction::ID id);
        bool affectedBy(Entity* e, CAction::ID id);

};

#endif // UNITSYSTEM_H
