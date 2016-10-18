#include "FadeSystem.h"

FadeSystem::FadeSystem(){
    addRequirement(Component::FADE);
    addSubscription(START_FADE_IN);
    addSubscription(START_FADE_OUT);
}

FadeSystem::~FadeSystem(){
    //dtor
}

void FadeSystem::update(){
    for (EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        double speed = e->get<CFade>()->alphaSpeed;

        double target = e->get<CFade>()->alphaTarget;
        double alpha = e->get<CDraw>()->alpha;
        e->get<CDraw>()->alpha = alpha + speed*delay;

        if ((target - alpha > 0) != (target - e->get<CDraw>()->alpha > 0)){
            e->get<CDraw>()->alpha = target;
            notify(e->get<CFade>()->endMessage, e);
            e->get<CFade>()->alphaSpeed = 0;
        }
    }
}
