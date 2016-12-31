#include "PathSystem.h"

PathSystem::PathSystem(){
    addRequirement(Component::PATH);
}

PathSystem::~PathSystem(){
    //dtor
}

void PathSystem::update(){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        if (eManager->isDead(e)) continue;
        double angle = angleBetweenPoints(e->get<CPosition>()->x, e->get<CPosition>()->y, e->get<CPath>()->target.x, e->get<CPath>()->target.y);
        double xSpeed = cos(angle*M_RAD) * e->get<CVelocity>()->speed;
        double ySpeed = sin(angle*M_RAD) * e->get<CVelocity>()->speed;

        double xNew = e->get<CPosition>()->x + xSpeed*delay;
        double xOld = e->get<CPosition>()->x;
        double xTarget = e->get<CPath>()->target.x;
        if ((xNew < xTarget) != (xOld < xTarget)){
            xNew = xTarget;
        }

        double yNew = e->get<CPosition>()->y + ySpeed*delay;
        double yOld = e->get<CPosition>()->y;
        double yTarget = e->get<CPath>()->target.y;
        if ((yNew < yTarget) != (yOld < yTarget)){
            yNew = yTarget;
        }
        e->get<CPosition>()->x = xNew;
        e->get<CPosition>()->y = yNew;

        if (e->get<CPosition>()->x == xTarget && e->get<CPosition>()->y == yTarget && !e->get<CPath>()->nodes.empty()){
            e->get<CPath>()->target = pollNextNode(e);
        }else if (e->get<CPosition>()->x == xTarget && e->get<CPosition>()->y == yTarget){
            e->remove(Component::PATH);
            eManager->addModified(e);
        }
    }

}

sf::Vector2f PathSystem::pollNextNode(Entity* e){
    sf::Vector2f node = e->get<CPath>()->nodes.front();
    e->get<CPath>()->nodes.pop_front();
    return node;
}
