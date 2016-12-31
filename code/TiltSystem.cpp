#include "TiltSystem.h"

TiltSystem::TiltSystem(){
    addRequirement(Component::ROTATION);
    addRequirement(Component::TILT);
}

void TiltSystem::update(){
    for(Entity* e : entities){
        if (!hasRequirements(e)) continue;
        e->get<CTilt>()->angle += e->get<CTilt>()->tiltSpeed*delay;
        double angle = e->get<CTilt>()->angle;
        double angleOff = e->get<CTilt>()->amplitude*cos(angle*M_RAD);
        e->get<CRotation>()->angle = e->get<CTilt>()->centralAngle + angleOff;
    }
}
