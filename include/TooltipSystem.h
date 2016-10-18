#ifndef TOOLTIPSYSTEM_H
#define TOOLTIPSYSTEM_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

class TooltipSystem : public System{
    public:
        TooltipSystem();
        ~TooltipSystem();

        void update();

    private:

        void onButtonGainedFocus(Entity* e);
        void onButtonLostFocus(Entity* e);
        void onUpdateTooltip(Entity* e);

        void createSectorTooltip(Entity* e);
        void createUnitGroupTooltip(Entity* e);
        void createUnitTooltip(Entity* e);
        void createActionTooltip(Entity* e);
        void createUnitActionTooltip(Entity* e);
        void createCaptainActionTooltip(Entity* e);
        void createEffectTooltip(Entity* e);
        void createFormationTooltip(Entity* e);
        void createBattleClosureTooltip(Entity* e);
        void createCaptainTooltip(Entity* e);
        void createCaptainTooltip2(Entity* e);
        void createTextTooltip1(Entity* e);

        void createTooltip(Entity* e);
};

#endif // TOOLTIPSYSTEM_H
