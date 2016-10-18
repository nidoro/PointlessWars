#ifndef TIMERSYSTEM_H
#define TIMERSYSTEM_H

#include "System.h"


class TimerSystem : public System{
    public:
        TimerSystem();
        ~TimerSystem();

        void update();

    private:
        void onRemoveEntity(Entity* e);
        //void onLockButton(Entity* e);
};

#endif // TIMERSYSTEM_H
