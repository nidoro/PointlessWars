#include "ElipsoidalMovementSystem.h"

ElipsoidalMovementSystem::ElipsoidalMovementSystem(){
    addRequirement(Component::ELIPSOIDAL_MOVEMENT);
    addRequirement(Component::POSITION);
}

ElipsoidalMovementSystem::~ElipsoidalMovementSystem(){
    //dtor
}

void ElipsoidalMovementSystem::update(){
    for(Entity* e : entities){
        if (!hasRequirements(e)) continue;
        e->get<CElipsoidalMovement>()->angle += e->get<CElipsoidalMovement>()->angularSpeed*delay;
        double angle = e->get<CElipsoidalMovement>()->angle;
        double xOff = e->get<CElipsoidalMovement>()->xAmp*cos(angle*M_RAD);
        double yOff = e->get<CElipsoidalMovement>()->yAmp*sin(angle*M_RAD);

        if (e->get<CElipsoidalMovement>()->setX) e->get<CPosition>()->x = e->get<CElipsoidalMovement>()->cx + xOff;
        if (e->get<CElipsoidalMovement>()->setY) e->get<CPosition>()->y = e->get<CElipsoidalMovement>()->cy + yOff;
    }
}
