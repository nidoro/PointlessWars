#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

class CommandListener : public System {
    public:
        CommandListener();

        void update() override;

    private:
        void onPlayerTurnTimeExpired(Entity* e) override;
        void onAcceptArmy(Entity*) override;
        void onRandomizeArmy(Entity*) override;
        void onSystemAction(Entity* e) override;
        void onSelectAction(Entity* e) override;
        void onShowHeroPool(Entity* e) override;
        void onEndHeroPool(Entity* e) override;
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
