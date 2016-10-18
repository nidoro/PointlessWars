#include "SliderSystem.h"

SliderSystem::SliderSystem(){
    addSubscription(START_SLIDING);
    addSubscription(MOUSE_BUTTON_RELEASED);
    addRequirement(Component::SLIDER);
}

SliderSystem::~SliderSystem(){
    //dtor
}


void SliderSystem::update(){
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
    sf::Vector2f cursorPosition = window->mapPixelToCoords(mousePosition);

    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;

        if (e->get<CSlider>()->sliding){
            double xTarget;
            double yTarget;
            double length = e->get<CSlider>()->length;
            if (e->get<CSlider>()->direction == CSlider::HORIZONTAL){
                xTarget = cursorPosition.x;
                xTarget = max(xTarget, e->get<CSlider>()->x-length/2.f);
                xTarget = min(xTarget, e->get<CSlider>()->x+length/2.f);
                yTarget = e->get<CSlider>()->y;
            }else{
                yTarget = cursorPosition.y;
                yTarget = max(yTarget, e->get<CSlider>()->y-length/2);
                yTarget = min(yTarget, e->get<CSlider>()->y+length/2);
                xTarget = e->get<CSlider>()->x;
            }
            e->get<CPosition>()->x = xTarget;
            e->get<CPosition>()->y = yTarget;

            updateValue(e);
            updateVariable(e);
        }
    }
}

void SliderSystem::updateValue(Entity* e){
    double posMin;
    double pos;
    double length = e->get<CSlider>()->length;
    if (e->get<CSlider>()->direction == CSlider::HORIZONTAL){
        pos = e->get<CPosition>()->x;
        posMin = e->get<CSlider>()->x - length/2;
        e->get<CSlider>()->value = (pos-posMin)/(length);
    }else{
        pos = e->get<CPosition>()->y;
        posMin = e->get<CSlider>()->y - length/2;
        e->get<CSlider>()->value = 1 - (pos-posMin)/(length);
    }
    e->get<CSlider>()->value = min(e->get<CSlider>()->value, 100.0);
    e->get<CSlider>()->value = max(e->get<CSlider>()->value, 0.0);
}

void SliderSystem::updateVariable(Entity* e){
    if (e->get<CSlider>()->variable == "music-volume"){
        config.setMusMaxVolume(e->get<CSlider>()->value*100);
    }else if (e->get<CSlider>()->variable == "sfx-volume"){
        config.setSfxMaxVolume(e->get<CSlider>()->value*100);
    }
}

void SliderSystem::onStartSliding(Entity* e){
    //notify(LOCK_ALL_BUTTONS);
    e->get<CSlider>()->sliding = true;
}

void SliderSystem::onMouseButtonReleased(Entity* e){
    bool sliding = false;
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* eSlider = *i;
        if (eSlider->get<CSlider>()->sliding) sliding = true;
        eSlider->get<CSlider>()->sliding = false;
    }
    if (sliding){
        //notify(UNLOCK_ALL_BUTTONS);
    }
}
