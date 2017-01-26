#ifndef DRAGDROPSYSTEM_H
#define DRAGDROPSYSTEM_H

#include "System.h"

class DragDropSystem : public System {
    public:
        DragDropSystem();
        ~DragDropSystem();

        void update();

    private:
        void onDrag(Entity* e);
        void onDrop(Entity* e);
        void onKeyPressed(Entity* e);
        void onMouseButtonReleased(Entity* e);

        EntityList candidates;

        Entity* eDragging;
};

#endif // DRAGDROPSYSTEM_H
