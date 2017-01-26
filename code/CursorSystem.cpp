#include "CursorSystem.h"

CursorSystem::CursorSystem() {
    addRequirement(Component::CURSOR);
    addSubscription(GAME_STARTED);
}

CursorSystem::~CursorSystem() {
    //dtor
}

void CursorSystem::update() {
    for (EntityListIt i = entities.begin(); i != entities.end(); i++) {
        Entity* e = *i;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        sf::Vector2f cursorPosition = window->mapPixelToCoords(mousePosition);
        e->get<CPosition>()->x = cursorPosition.x;
        e->get<CPosition>()->y = cursorPosition.y;

        if (e->has(Component::PARTICLE_EMMITER)) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                e->get<CParticleEmmiter>()->on = true;
            } else {
                e->get<CParticleEmmiter>()->on = false;
            }
        }
    }
}

void CursorSystem::onGameStarted(Entity* e) {
    Entity* cursor = eManager->createEntity();
    cursor->add(new CTexture("cursor.png"));
    cursor->add(new CPosition());
    cursor->add(new CDraw(CDraw::CURSOR));
    cursor->add(new CCursor());
    cursor->add(new CSystem());

    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Red);
    rect.setSize(sf::Vector2f(5,5));
    //cursor->add(new CParticleEmmiter(60, rect, 0.2, 50, 100, -90, 360, 5));
}

