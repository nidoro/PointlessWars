#ifndef TURNTIMERSYSTEM_H
#define TURNTIMERSYSTEM_H

#include "System.h"

class TurnTimerSystem : public System {
    public:
        TurnTimerSystem();
        void update() override;
        
    private:
        void onSystemAction(Entity* e) override;
        
};

#endif /* TURNTIMERSYSTEM_H */
