#ifndef DROPLISTSYSTEM_H
#define DROPLISTSYSTEM_H

#include "System.h"
#include "Assets.h"

class DropListSystem : public System {
    public:
        DropListSystem();
        ~DropListSystem();

        void update();

    private:
        void onExpandDropList(Entity* e);
        void onCollapseDropList(Entity* e);
        void onUpdateDropList(Entity* e);
        void onMouseButtonReleased(Entity* e);

        Entity* eDropOn;
        bool active;
};

#endif // DROPLISTSYSTEM_H
