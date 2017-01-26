#ifndef SLIDERSYSTEM_H
#define SLIDERSYSTEM_H

#include "System.h"


class SliderSystem : public System {
    public:
        SliderSystem();
        ~SliderSystem();

        void update();
    private:
        void updateValue(Entity* e);
        void updateVariable(Entity* e);

        void onStartSliding(Entity* e);
        void onMouseButtonReleased(Entity* e);
};

#endif // SLIDERSYSTEM_H
