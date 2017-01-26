#ifndef MASTERSYSTEM_H
#define MASTERSYSTEM_H

#include "System.h"

class MasterSystem : public System {
    public:
        MasterSystem();
        ~MasterSystem();
        void update();

    private:
        void onUpdatePlayerNicknameWithInputTextbox(Entity* e);

};

#endif // MASTERSYSTEM_H
