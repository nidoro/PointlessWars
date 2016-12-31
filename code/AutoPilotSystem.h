#ifndef AUTOPILOTSYSTEM_H
#define AUTOPILOTSYSTEM_H

#include "System.h"


class AutoPilotSystem : public System{
    public:
        AutoPilotSystem();
        ~AutoPilotSystem();

        void update();
    private:
};

#endif // AUTOPILOTSYSTEM_H
