#ifndef AISYSTEM_H
#define AISYSTEM_H

#include "System.h"

class AISystem : public System {
    public:
        AISystem();
        ~AISystem();

        void update();

    private:
        void onSystemAction(Entity* e);

        void selectArmyAction(Entity* e);
        void selectHeroAction(Entity* e);
        void selectFormation(Entity* e);
        void selectHero(Entity* e);
        void selectBattleClosure(Entity* e);
        void pickHeroFromPool(Entity* e);
        void assembleArmy(Entity* e);
        void selectRandomUnits(list<CUnit::ID>& output, list<CUnit::ID>& input, int n);

        EntityList getAliveUnits(Entity* e);
        EntityList getDeadUnits(Entity* e);
};

#endif // AISYSTEM_H
