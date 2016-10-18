#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(){
    addRequirement(Component::ANIMATION);
    addSubscription(START_ANIMATION_ACTION);
    addSubscription(START_ANIMATION_DEATH);
    addSubscription(START_ANIMATION_RAISE_SHIELD);
    addSubscription(END_ANIMATION_DEATH);
    addSubscription(END_ANIMATION_RAISE_SHIELD);
    addSubscription(SWITCH_CHAR_ANIMATION);
}

AnimationSystem::~AnimationSystem(){
    //dtor
}

void AnimationSystem::update(){
    for (EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        if (eManager->isDead(e)) continue;
        if (!e->get<CAnimation>()->freeze){
            updateFrame(e);
        }
    }
}
void AnimationSystem::updateFrame(Entity* e){
    Animation animation = Assets::getAnimation(e->get<CAnimation>()->current);
    if (e->get<CAnimation>()->clock.getElapsedTime().asSeconds() >= animation.rate || e->get<CAnimation>()->update){
        if (!e->get<CAnimation>()->update){
            e->get<CAnimation>()->frame = (e->get<CAnimation>()->frame+1) % animation.nFrames;
        }else{
            e->get<CAnimation>()->frame = 0;
        }
        e->get<CAnimation>()->sprite = animation.frames[e->get<CAnimation>()->frame];
        e->get<CAnimation>()->clock.restart();
        e->get<CAnimation>()->update = false;
    }
}

void AnimationSystem::onSwitchCharAnimation(Entity* e){
    string anim;
    if (e->get<CAnimation>()->current == e->get<CChar>()->aIdle){
        anim = e->get<CChar>()->aWalk;
    }else{
        anim = e->get<CChar>()->aIdle;
    }
    e->get<CAnimation>()->current = anim;
    e->get<CAnimation>()->frame = 0;
    e->get<CAnimation>()->sprite = Assets::getAnimation(anim).frames[e->get<CAnimation>()->frame];
    e->get<CAnimation>()->clock.restart();
}
