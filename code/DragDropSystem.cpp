#include "DragDropSystem.h"

DragDropSystem::DragDropSystem(){
    addSubscription(DRAG);
    addSubscription(DROP);
    addSubscription(KEY_PRESSED);
    addSubscription(MOUSE_BUTTON_RELEASED);

    eDragging = nullptr;
    candidates.clear();
}

DragDropSystem::~DragDropSystem(){
    //dtor
}

void DragDropSystem::update(){
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
    sf::Vector2f cursorPosition = window->mapPixelToCoords(mousePosition);

    if (!candidates.empty()){
        double d = INFINITY;
        for(EntityListIt i = candidates.begin(); i != candidates.end(); i++){
            Entity* e = *i;
            if (getDistance(e->get<CPosition>()->x, e->get<CPosition>()->y, cursorPosition.x, cursorPosition.y) < d){
                eDragging = e;
                d = getDistance(e->get<CPosition>()->x, e->get<CPosition>()->y, cursorPosition.x, cursorPosition.y);
            }
        }
        candidates.clear();
    }

    if (eDragging != nullptr){
        if (eManager->isDead(eDragging)) return;
        eDragging->get<CDragDrop>()->active = true;
        eDragging->get<CPosition>()->x = cursorPosition.x;
        eDragging->get<CPosition>()->y = cursorPosition.y;
    }
}

void DragDropSystem::onDrag(Entity* e){
    if (eDragging != nullptr) return;
    candidates.push_back(e);
}

void DragDropSystem::onDrop(Entity* e){
    eDragging->get<CDragDrop>()->active = false;
    eDragging = nullptr;
}

void DragDropSystem::onKeyPressed(Entity* e){
    if (eDragging != nullptr){
        if (e->get<CKeyboardInput>()->code == sf::Keyboard::Left || e->get<CKeyboardInput>()->code == sf::Keyboard::Right){
            eDragging->get<CAnimation>()->hFlip = !oddNumber(eDragging->get<CAnimation>()->hFlip);
        }else if (e->get<CKeyboardInput>()->code == sf::Keyboard::Up || e->get<CKeyboardInput>()->code == sf::Keyboard::Down){
            eDragging->get<CAnimation>()->vFlip = !oddNumber(eDragging->get<CAnimation>()->vFlip);
        }else if(e->get<CKeyboardInput>()->code == sf::Keyboard::Equal){
            double dw = 1;
            double dh = eDragging->get<CDimensions>()->width/eDragging->get<CDimensions>()->height * dw;
            eDragging->get<CDimensions>()->width += dw;
            eDragging->get<CDimensions>()->height += dh;
            eDragging->get<CButtonHitbox>()->width = eDragging->get<CDimensions>()->width;
            eDragging->get<CButtonHitbox>()->height = eDragging->get<CDimensions>()->height;
        }else if(e->get<CKeyboardInput>()->code == sf::Keyboard::Dash){
            double dw = 1;
            double dh = eDragging->get<CDimensions>()->width/eDragging->get<CDimensions>()->height * dw;
            eDragging->get<CDimensions>()->width -= dw;
            eDragging->get<CDimensions>()->height -= dh;
            eDragging->get<CButtonHitbox>()->width = eDragging->get<CDimensions>()->width;
            eDragging->get<CButtonHitbox>()->height = eDragging->get<CDimensions>()->height;
        }
    }
}

void DragDropSystem::onMouseButtonReleased(Entity* e){
    if (e->get<CMouseInput>()->buttonReleased == sf::Mouse::Left && eDragging != nullptr){
        notify(DROP, eDragging);
    }
}
