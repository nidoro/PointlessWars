#ifndef ELIPSOIDALMOVEMENTSYSTEM_H
#define ELIPSOIDALMOVEMENTSYSTEM_H

#include "System.h"

class ElipsoidalMovementSystem : public System {
    public:
        ElipsoidalMovementSystem();
        ~ElipsoidalMovementSystem();

        void update();

    private:
};

#endif // ELIPSOIDALMOVEMENTSYSTEM_H
