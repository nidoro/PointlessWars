#ifndef DISPLAYERSYSTEM_H
#define DISPLAYERSYSTEM_H

#include "System.h"
#include "Standard.h"

class DisplayerSystem : public System {
    public:
        DisplayerSystem();
        ~DisplayerSystem();

        void update();

    private:

        string getGameStateString();
        int getUnitCount(Entity* e);

};

#endif // DISPLAYERSYSTEM_H
