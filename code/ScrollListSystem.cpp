#include "ScrollListSystem.h"

ScrollListSystem::ScrollListSystem(){
    addSubscription(INITIALIZE_SCROLL_LIST);
    addSubscription(SCROLL);
}

ScrollListSystem::~ScrollListSystem(){
    //dtor
}

void ScrollListSystem::onInitializeScrollList(Entity* e){

    e->get<CScrollList>()->wCell = e->get<CScrollList>()->cells[0]->get<CDimensions>()->width;
    e->get<CScrollList>()->hCell = e->get<CScrollList>()->cells[0]->get<CDimensions>()->height;

    createScrollButtons(e);
    updateCells(e);
}

void ScrollListSystem::createScrollButtons(Entity* e){
    Entity* eButton1 = eManager->createEntity();
    Entity* eButton2 = eManager->createEntity();

    double w = 32;
    double h = 32;
    double x = e->get<CPosition>()->x;
    double y = e->get<CPosition>()->y;

    eButton1->add(new CTexture());
    eButton1->add(new CPosition(x, y));
    eButton1->add(new CDimensions(w, h));
    eButton1->add(new CButtonHitbox(w, h));
    eButton1->add(new CButtonState());
    eButton1->add(new CButtonTrigger(SCROLL));
    eButton1->add(new CDraw(CDraw::GUI1));
    eButton1->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    eButton1->add(new CScrollButton(e, CScrollButton::BACKWARD));

    if (e->get<CScrollList>()->orientation == CScrollList::HORIZONTAL){
        x = e->get<CPosition>()->x + w/2
            + (e->get<CScrollList>()->spacing+e->get<CScrollList>()->wCell)*e->get<CScrollList>()->size
            + e->get<CScrollList>()->spacing + w/2;
        eButton1->add(new CDefaultTexture("bt-left-arrow.png"));
        eButton1->add(new CHoverTexture("bt-left-arrow.png"));
        eButton1->add(new CActiveTexture("bt-left-arrow.png"));
    }else{
        y = e->get<CPosition>()->y + h/2
            + (e->get<CScrollList>()->spacing+e->get<CScrollList>()->hCell)*e->get<CScrollList>()->size
            + e->get<CScrollList>()->spacing + h/2;
        eButton1->add(new CDefaultTexture("bt-up-arrow.png"));
        eButton1->add(new CHoverTexture("bt-up-arrow.png"));
        eButton1->add(new CActiveTexture("bt-up-arrow.png"));
    }

    eButton2->add(new CTexture());
    eButton2->add(new CPosition(x, y));
    eButton2->add(new CDimensions(w, h));
    eButton2->add(new CButtonHitbox(w, h));
    eButton2->add(new CButtonState());
    eButton2->add(new CButtonTrigger(SCROLL));
    eButton2->add(new CDraw(CDraw::GUI1));
    eButton2->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    eButton2->add(new CScrollButton(e, CScrollButton::FORWARD));


    if (e->get<CScrollList>()->orientation == CScrollList::HORIZONTAL){
        eButton2->add(new CDefaultTexture("bt-right-arrow.png"));
        eButton2->add(new CHoverTexture("bt-right-arrow.png"));
        eButton2->add(new CActiveTexture("bt-right-arrow.png"));
    }else{
        eButton2->add(new CDefaultTexture("bt-down-arrow.png"));
        eButton2->add(new CHoverTexture("bt-down-arrow.png"));
        eButton2->add(new CActiveTexture("bt-down-arrow.png"));
    }
}

void ScrollListSystem::updateCells(Entity* e){
    double w = 32;
    double h = 32;
    double x, y;
    if (e->get<CScrollList>()->orientation == CScrollList::HORIZONTAL){
        x = e->get<CPosition>()->x + w/2 + e->get<CScrollList>()->spacing + e->get<CScrollList>()->wCell/2;
        y = e->get<CPosition>()->y;
    }else{
        y = e->get<CPosition>()->y + h/2 + e->get<CScrollList>()->spacing + e->get<CScrollList>()->hCell/2;
        x = e->get<CPosition>()->x;
    }

    for(unsigned int i = 0; i < e->get<CScrollList>()->cells.size(); i++){
        Entity* eCell = e->get<CScrollList>()->cells[i];
        eCell->get<CDraw>()->isHidden = true;
        eCell->get<CButtonState>()->state = CButtonState::LOCKED;
    }

    int c = e->get<CScrollList>()->front;
    for(int i = 0; i < e->get<CScrollList>()->size; i++){
        if ((unsigned) i >= e->get<CScrollList>()->cells.size()) break;
        Entity* eCell = e->get<CScrollList>()->cells[c];
        eCell->get<CDraw>()->isHidden = false;
        eCell->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
        eCell->get<CPosition>()->x = x;
        eCell->get<CPosition>()->y = y;
        c = (c+1)%(e->get<CScrollList>()->cells.size());

        if (e->get<CScrollList>()->orientation == CScrollList::HORIZONTAL){
            x += e->get<CScrollList>()->spacing + e->get<CScrollList>()->wCell;
        }else{
            y += e->get<CScrollList>()->spacing + e->get<CScrollList>()->hCell;
        }
    }
}

void ScrollListSystem::onScroll(Entity* e){
    Entity* eList = e->get<CScrollButton>()->eList;
    int step = eList->get<CScrollList>()->step;
    int nOptions = eList->get<CScrollList>()->cells.size();
    if (e->get<CScrollButton>()->move == CScrollButton::BACKWARD){
        eList->get<CScrollList>()->front -= step;
        if (eList->get<CScrollList>()->front < 0) eList->get<CScrollList>()->front = nOptions + eList->get<CScrollList>()->front;
    }else{
        eList->get<CScrollList>()->front = (eList->get<CScrollList>()->front+step)%(nOptions);
    }

    updateCells(eList);
}


