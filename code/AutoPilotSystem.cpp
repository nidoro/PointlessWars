#include "AutoPilotSystem.h"

AutoPilotSystem::AutoPilotSystem() {
    addRequirement(Component::AUTO_PILOT);
}

AutoPilotSystem::~AutoPilotSystem() {
    //dtor
}

void AutoPilotSystem::update() {
    for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
        Entity* e = *i;
        if (eManager->isDead(e)) continue;

        if (e->has(Component::ACCELERATION) && e->has(Component::VELOCITY)) {
            e->get<CVelocity>()->x += e->get<CAcceleration>()->x*delay;
            e->get<CVelocity>()->y += e->get<CAcceleration>()->y*delay;
        }

        if (e->has(Component::VELOCITY) && e->has(Component::POSITION) && !e->has(Component::PATH)) {
            e->get<CPosition>()->x += e->get<CVelocity>()->x*delay;
            e->get<CPosition>()->y += e->get<CVelocity>()->y*delay;
        }

        if (e->has(Component::ROTATION) && e->has(Component::VELOCITY)) {
            if (e->get<CRotation>()->followVelocity) {
                sf::Vector2f v(e->get<CVelocity>()->x, e->get<CVelocity>()->y);
                e->get<CRotation>()->angle = angleBetweenPoints(0,0,v.x,v.y);
            }
        }

        if (e->has(Component::ROTATION)) {
            e->get<CRotation>()->angle += e->get<CRotation>()->velocity*delay;
        }

        if (e->has(Component::ZOOM) && e->has(Component::DIMENSIONS)) {
            //e->get<CDimensions>()->width += e->get<CZoom>()->dx * delay;
            //e->get<CDimensions>()->height += e->get<CZoom>()->dy * delay;

            double wNew = e->get<CDimensions>()->width + e->get<CZoom>()->dx * delay;
            double wOld = e->get<CDimensions>()->width;
            double wTarget = e->get<CZoom>()->wTarget;
            if (e->get<CDimensions>()->width != wTarget) {
                if ((wNew < wTarget) != (wOld < wTarget)) {
                    wNew = wTarget;
                }
            }
            e->get<CDimensions>()->width = wNew;

            double hNew = e->get<CDimensions>()->height + e->get<CZoom>()->dy * delay;
            double hOld = e->get<CDimensions>()->height;
            double hTarget = e->get<CZoom>()->hTarget;
            if (e->get<CDimensions>()->height != hTarget) {
                if ((hNew < hTarget) != (hOld < hTarget)) {
                    hNew = hTarget;
                }
            }
            e->get<CDimensions>()->height = hNew;
            //e->get<CDimensions>()->width = wNew;
            //e->get<CDimensions>()->height = hNew;

            if ((e->get<CDimensions>()->width == wTarget && e->get<CDimensions>()->height == hTarget)
                    ||  (e->get<CDimensions>()->width == wTarget && e->get<CZoom>()->dy == 0.0)
                    ||  (e->get<CDimensions>()->height == hTarget && e->get<CZoom>()->dx == 0.0)) {
                e->remove(Component::ZOOM);
                eManager->addModified(e);
            }
        }
    }
}
