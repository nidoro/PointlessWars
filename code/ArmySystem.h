#ifndef ARMYSYSTEM_H
#define ARMYSYSTEM_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

class ArmySystem : public System {
    public:
        ArmySystem();
        ~ArmySystem();

        void update();

    private:
        void onAssignRandomArmy(Entity* e);
        void onInitializePlayer(Entity* e);
        void onCreateArmy(Entity* e);
        void onRecomposeArmy(Entity* e);
        void onStartBattleState(Entity* e);
        void onSystemAction(Entity* e);
        void onPickHero(Entity* e);

        void composeArmy(Entity* e, EntityList eOptions);
        void recomposeArmy(Entity* e);
        void recordArmy(Entity* e);
        void createArmy(Entity* e);
        void updateUnitCount(Entity* e);

        void createRandomArmy(Entity* e);
        void clearArmy(Entity* e);

        EntityList getAliveUnits(Entity* e);
        void removeFormationEffect(Entity* e);
};

#endif // ARMYSYSTEM_H
