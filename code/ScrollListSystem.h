#ifndef SCROLLLISTSYSTEM_H
#define SCROLLLISTSYSTEM_H

#include "System.h"


class ScrollListSystem : public System {
    public:
        ScrollListSystem();
        ~ScrollListSystem();

    private:
        void onInitializeScrollList(Entity* e);
        void onScroll(Entity* e);
        void createScrollButtons(Entity* e);
        void updateCells(Entity* e);
};

#endif // SCROLLLISTSYSTEM_H
