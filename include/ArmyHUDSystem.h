#ifndef ARMYHUDSYSTEM_H
#define ARMYHUDSYSTEM_H

#include "System.h"
#include "Assets.h"


class ArmyHUDSystem : public System{
    public:
        ArmyHUDSystem();
        ~ArmyHUDSystem();

        void update();

    private:
        void onHighlightUnits(Entity* e);
        void onHighlightUnitsOff(Entity* e);
        void onScoreUpdated(Entity* e);

        void updateUnits(Entity* e);
        void updateEffects(Entity* e);
        void updateCaptain(Entity* e);
        void updateScore(Entity* e);
        void updateNAlive(Entity* e);
        void updateResistBars(Entity* e);
        void updateCoin(Entity* e);

        void createResistanceHighlighters(Entity* e);
        bool mapContains(map<CAction::ID, CAction>& m, CAction::ID id);
        double getAverageResistance(Entity* e, CUnit::DamageType dmgType);
        int getIndex(map<CUnit::ID, IconDisplayerPair>& m, CUnit::ID id);

        void animateUnitIn(IconDisplayerPair& p);
        void animateUnitOut(IconDisplayerPair& p);
        void arrangeUnits(Entity* e);
        void arrangeEffects(Entity* e);
        void animateAliveCounterIn(Entity* e);
        void animateAliveCounterOut(Entity* e);
        void animateButtonInPuff(Entity* e, double after, bool sound = false);
        void animateButtonOutPuff(Entity* e, double after, bool sound = false);

        EntityList eResists;
};

#endif // ARMYHUDSYSTEM_H
