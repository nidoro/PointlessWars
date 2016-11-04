#include "DropListSystem.h"

DropListSystem::DropListSystem(){
    addSubscription(EXPAND_DROP_LIST);
    addSubscription(COLLAPSE_DROP_LIST);
    addSubscription(UPDATE_DROP_LIST);
    addSubscription(MOUSE_BUTTON_RELEASED);
    eDropOn = nullptr;
    active = false;
}

DropListSystem::~DropListSystem(){

}

void DropListSystem::update(){

}

void DropListSystem::onExpandDropList(Entity* e){
    //notify(LOCK_ALL_BUTTONS);

    CUILayer::Layer uiLayer = (CUILayer::Layer) ((int)e->get<CUILayer>()->layer + 1);

    sf::RectangleShape shape = e->get<CRectShape>()->shape;
    sf::Font* fnt = (sf::Font*) e->get<CTextbox2>()->content.getFont();
    int size = e->get<CTextbox2>()->content.getCharacterSize();
    double w = shape.getSize().x;
    double h = shape.getSize().y;
    double x = e->get<CPosition>()->x;
    double y = e->get<CPosition>()->y;

    double dx, dy;
    switch(e->get<CDropList>()->direction){
        case CDropList::DOWN:   dx = 0; dy = h+2; break;
        case CDropList::UP:     dx = 0; dy = -h; break;
        case CDropList::LEFT:   dx = -w; dy = 0; break;
        case CDropList::RIGHT:  dx = w; dy = 0; break;
        default:                dx = 0; dy = h; break;
    }

    double hTotal = e->get<CDropList>()->values.size() * dy;
    double wTotal = e->get<CDropList>()->values.size() * dx;

    if (e->get<CDropList>()->direction == CDropList::DOWN){
        hTotal = e->get<CDropList>()->values.size() * dy;
        double yLastItem = y + hTotal;
        double hHidden = yLastItem - (window->getView().getCenter().y + window->getView().getSize().y/2);
        if (hHidden > 0) yLastItem -= hHidden;
        y = yLastItem - hTotal;
    }

    for(list<string>::iterator i = e->get<CDropList>()->values.begin(); i != e->get<CDropList>()->values.end(); i++){
        string value = *i;
        Entity* eCell = eManager->createEntity();
        eCell->add(new CPosition(x, y));
        eCell->add(new CRectShape(e->get<CDropList>()->defRect));
        eCell->add(new CTextbox2(value, *fnt, size, e->get<CTextbox2>()->content.getFillColor()));
        eCell->add(new CRectButton(e->get<CDropList>()->defRect, e->get<CDropList>()->hovRect, e->get<CDropList>()->actRect));
        eCell->add(new CDraw(e->get<CDraw>()->tag));
        eCell->add(new CButtonState());
        eCell->add(new CButtonTrigger(UPDATE_DROP_LIST));
        eCell->add(new CButtonHitbox(w, h));
        eCell->addObservedEntity("DropListHead", e);
        eCell->attachEmployer(e);
        eCell->add(new CUILayer(uiLayer));
        //if (e->has(Component::UI_LAYER)){
        //    eCell->add(new CUILayer(*e->get<CUILayer>()));
        //}
        e->get<CDropList>()->cells.insert(make_pair(eCell, value));

        y += dy;
        x += dx;
    }
    e->get<CButtonTrigger>()->setUniqueTrigger(COLLAPSE_DROP_LIST);
    eDropOn = e;
    active = true;

    notify(BRING_UI_LAYER_FORWARD, e->get<CDropList>()->cells.begin()->first);
}

void DropListSystem::onCollapseDropList(Entity* e){
    //notify(UNLOCK_ALL_BUTTONS);
    for(auto& p : e->get<CDropList>()->cells){
        eManager->removeEntity(p.first);
    }
    e->get<CDropList>()->cells.clear();
    e->get<CButtonTrigger>()->setUniqueTrigger(EXPAND_DROP_LIST);
    eDropOn = nullptr;
    active = false;
    notify(BRING_UI_LAYER_FORWARD, e);
}

void DropListSystem::onUpdateDropList(Entity* e){
    Entity* eHead = e->getObservedEntity("DropListHead");
    eHead->get<CDropList>()->value = eHead->get<CDropList>()->cells[e];
    eHead->get<CTextbox2>()->content.setString(eHead->get<CDropList>()->value);
    notify(eHead->get<CDropList>()->msgOnUpdate, eHead);
    notify(COLLAPSE_DROP_LIST, eHead);
}

void DropListSystem::onMouseButtonReleased(Entity* e){
    if (active){
        bool out = true;
        double x = eDropOn->get<CPosition>()->x;
        double y = eDropOn->get<CPosition>()->y;
        double w = eDropOn->get<CButtonHitbox>()->width;
        double h = eDropOn->get<CButtonHitbox>()->height;
        sf::FloatRect rect(x - w/2, y - h/2, w, h);
        sf::Vector2i mousePosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
        sf::Vector2f mp = window->mapPixelToCoords(mousePosition);
        if (rect.contains(mp)){
            out = false;
        }

        for(auto& p : eDropOn->get<CDropList>()->cells){
            Entity* eCell = p.first;
            x = eCell->get<CPosition>()->x;
            y = eCell->get<CPosition>()->y;
            w = eCell->get<CButtonHitbox>()->width;
            h = eCell->get<CButtonHitbox>()->height;
            rect = sf::FloatRect(x - w/2, y - h/2, w, h);
            mousePosition = sf::Vector2i(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
            mp = sf::Vector2f(window->mapPixelToCoords(mousePosition));
            if (rect.contains(mp)){
                out = false;
            }
        }
        if (out){
            notify(COLLAPSE_DROP_LIST, eDropOn);
        }
    }
}
