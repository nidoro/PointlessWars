#ifndef TURNTIMERSYSTEM_H
#define TURNTIMERSYSTEM_H

#include "System.h"
#include "Assets.h"

class TurnTimerSystem : public System {
    public:
        TurnTimerSystem();
        void update() override;
        
    private:
        void onSystemAction(Entity* e) override;
        void onSelectAction(Entity* e) override;
        void onActionSelected(Entity* e) override;
        
        void createTimer(float length);
        void removeTimer();
};

#endif /* TURNTIMERSYSTEM_H */
