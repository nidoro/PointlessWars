#ifndef AISYSTEM_H
#define AISYSTEM_H

#include "System.h"

class AISystem : public System{
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

        EntityList getAliveUnits(Entity* e);
        EntityList getDeadUnits(Entity* e);
};

#endif // AISYSTEM_H
