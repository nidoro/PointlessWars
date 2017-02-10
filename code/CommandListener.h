#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

class CommandListener : public System {
    public:
        CommandListener();

        void update();

    private:
        void onAcceptArmy(Entity*);
        void onRandomizeArmy(Entity*);
        void onSystemAction(Entity* e);
        void onSelectAction(Entity* e);
        void onShowHeroPool(Entity* e);
        void onEndHeroPool(Entity* e);
        void endHeroPool();
        
        // @note: display option buttons
        void showCaptainOptions(Entity* e);
        void showFormationOptions(Entity* e);
        void showCaptainActionOptions(Entity* e);
        void showArmyActionOptions(Entity* e);
        void showBattleClosureOptions(Entity* e);
        void showUnitOptions(Entity* e);
        void askHeroPick(Entity* e);
        
        // @note: button animations
        void animateButtonInPuff(Entity* e, double after = 0.0, bool sound = true);
        void animateButtonOutPuff(Entity* e, double after = 0.0, bool sound = true);
        void animateUnitOptionIn(Entity* e);
        void animateUnitOptionOut(Entity* e, bool sound);
        
        // @note: helpers
        int getArmyCompositionSize(Entity* e);
        void selectRandomUnits(list<CUnit::ID>& output, list<CUnit::ID>& input, int n = 5);

        Entity* eListener;
        bool listening;
};

#endif // COMMANDLISTENER_H
