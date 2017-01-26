#include "AnchorSystem.h"

AnchorSystem::AnchorSystem() {
    addRequirement(Component::ANCHOR);
    addRequirement(Component::POSITION);
}

AnchorSystem::~AnchorSystem() {
    //dtor
}
void AnchorSystem::update() {
    std::list<Entity*>::iterator it;
    for (it = entities.begin(); it != entities.end(); it++) {
        Entity* e = *it;
        Entity* eAnchor = e->getObservedEntity("Anchor");
        if (!eAnchor) continue;
        if (eAnchor->has(Component::ROTATION)) {
            double angle = eAnchor->get<CRotation>()->angle;
            e->get<CPosition>()->x = eAnchor->get<CPosition>()->x + cos(angle*M_RAD)*e->get<CAnchor>()->xOff;
            e->get<CPosition>()->y = eAnchor->get<CPosition>()->y + sin(angle*M_RAD)*e->get<CAnchor>()->xOff;
        } else if (eAnchor->has(Component::POSITION)) {
            e->get<CPosition>()->x = eAnchor->get<CPosition>()->x + e->get<CAnchor>()->xOff;
            e->get<CPosition>()->y = eAnchor->get<CPosition>()->y + e->get<CAnchor>()->yOff;
        }
    }
}
