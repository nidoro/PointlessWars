#ifndef CURSORSYSTEM_H
#define CURSORSYSTEM_H

#include "System.h"

class CursorSystem : public System {
    public:
        CursorSystem();
        ~CursorSystem();

        void update();

    private:
        void onGameStarted(Entity* e);
};

#endif // CURSORSYSTEM_H
