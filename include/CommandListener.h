#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

class CommandListener : public System{
    public:
        CommandListener();
        ~CommandListener();

        void update();

    private:
        void onStartBattleState(Entity* e);
        void onAcceptArmy(Entity*);
        void onRandomizeArmy(Entity*);
        void onSystemAction(Entity* e);
        void onPickHero(Entity* e);
        void onSelectAction(Entity* e);
        void onShowHeroPool(Entity* e);
        void onRemoveFromHeroPool(Entity* e);

        void showCaptainsPool(Entity* e);
        void showCaptainOptions(Entity* e);
        void showFormationOptions(Entity* e);
        void showCaptainActionOptions(Entity* e);
        void showArmyActionOptions(Entity* e);
        void showBattleClosureOptions(Entity* e);
        void showUnitOptions(Entity* e);
        void showHeroPool();
        void playMeleeBattle(Entity* e);
        void selectRandomUnits(list<CUnit::ID>& output, list<CUnit::ID>& input, int n = 5);
        void clearArmy(Entity* e);
        void recomposeArmy(Entity* e);
        void reassembleArmies(Entity* e);
        void presentArmy(Entity* e);
        void advanceArmy(Entity* e);
        void cleanupField(Entity* e);
        void askHeroPick(Entity* e);
        void endHeroPool();
        void onEndHeroPool(Entity* e);

        bool checkArmySize(Entity* e);

        void animateButtonInPuff(Entity* e, double after = 0.0, bool sound = true);
        void animateButtonOutPuff(Entity* e, double after = 0.0, bool sound = true);
        void animateUnitOptionIn(Entity* e);
        void animateUnitOptionOut(Entity* e, bool sound);

        int getArmyCompositionSize(Entity* e);
        
        Entity* eListener;
        bool listening;
};

#endif // COMMANDLISTENER_H
