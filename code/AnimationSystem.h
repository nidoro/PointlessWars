#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "System.h"
#include "Assets.h"

class AnimationSystem : public System {
    public:
        AnimationSystem();
        ~AnimationSystem();

        void update();

    private:
        void onSwitchCharAnimation(Entity* e);
        void updateFrame(Entity* e);
};

#endif // ANIMATIONSYSTEM_H
