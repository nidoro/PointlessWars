#include "PanelSystem.h"

PanelSystem::PanelSystem(){
    addSubscription(BEGIN_PANEL);
    addSubscription(END_PANEL);
    addSubscription(SET_PANEL_AS_TOOLTIP);
    addSubscription(SET_PANEL_AS_WINDOW);
    addSubscription(ATTACH_ENTITY_TO_PANEL);
    addSubscription(REMOVE_PANEL);
    addSubscription(CLOSE_PANEL);
    addSubscription(REMOVE_SIBLINGS);
    addSubscription(REMOVE_COUSINS);
    addSubscription(REMOVE_NEPHEWS);
    addSubscription(REMOVE_CHILDREN);
    addSubscription(REMOVE_PARENT);

    addRequirement(Component::PARENT_PANEL);
}

PanelSystem::~PanelSystem(){
    //dtor
}

void PanelSystem::update(){
    /*
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;

        if (!e->has(Component::PARENT_PANEL)) continue;
        if (eManager->isDead(e->get<CParentPanel>()->e)){
            if (!eManager->isDead(e)) eManager->removeEntity(e);
        }else{
            if (eManager->isDead(e) && e->get<CParentPanel>()->e->has(Component::PANEL)){
                e->get<CParentPanel>()->e->get<CPanel>()->objects.remove(e);
            }
        }
    }
    */
}

void PanelSystem::onBeginPanel(Entity* e){
    ePanel = e;
}
void PanelSystem::onEndPanel(Entity* e){
    ePanel = nullptr;
}

void PanelSystem::onSetTooltip(Entity* e){
    ePanel->get<CPanel>()->toolButton = e;
    ePanel->get<CPanel>()->windowButton = nullptr;
    ePanel->get<CPanel>()->panelType = CPanel::TOOLTIP;
}

void PanelSystem::onSetWindow(Entity* e){
    ePanel->get<CPanel>()->toolButton = nullptr;
    ePanel->get<CPanel>()->windowButton = e;
    ePanel->get<CPanel>()->panelType = CPanel::WINDOW;
}

void PanelSystem::onAttachEntityToPanel(Entity* e){
    ePanel->get<CPanel>()->objects.push_back(e);
}

void PanelSystem::onRemovePanel(Entity* e){
    if (e->has(Component::PANEL)){
        for(EntityListIt i = e->get<CPanel>()->objects.begin(); i != e->get<CPanel>()->objects.end(); i++){
            if ((*i)->has(Component::PANEL)){
                notify(REMOVE_PANEL, *i);
            }else{
                eManager->removeEntity(*i);
            }
        }
    }
    eManager->removeEntity(e);
}

void PanelSystem::onClosePanel(Entity* e){
    notify(REMOVE_PANEL, e->get<CParentPanel>()->e);
    notify(UNLOCK_ALL_BUTTONS);
}

void PanelSystem::onRemoveSiblings(Entity* e){
    Entity* eParent = e->get<CParentPanel>()->e;
    for(EntityListIt i = eParent->get<CPanel>()->objects.begin(); i != eParent->get<CPanel>()->objects.end(); i++){
        Entity* eSib = *i;
        if (eSib != e){
            notify(REMOVE_PANEL, eSib);
        }
    }
    eParent->get<CPanel>()->objects.clear();
    eParent->get<CPanel>()->objects.push_back(e);
}

void PanelSystem::onRemoveCousins(Entity* e){
    Entity* eParent = e->get<CParentPanel>()->e;
    Entity* eGrandpa = eParent->get<CParentPanel>()->e;
    for(EntityListIt i = eGrandpa->get<CPanel>()->objects.begin(); i != eGrandpa->get<CPanel>()->objects.end(); i++){
        Entity* eUncle = *i;
        if (eUncle == eParent) continue;
        for(EntityListIt j = eUncle->get<CPanel>()->objects.begin(); j != eUncle->get<CPanel>()->objects.end(); j++){
            notify(REMOVE_PANEL, *i);
        }
        eUncle->get<CPanel>()->objects.clear();
    }
}

void PanelSystem::onRemoveNephews(Entity* e){
    Entity* eParent = e->get<CParentPanel>()->e;
    for(EntityListIt i = eParent->get<CPanel>()->objects.begin(); i != eParent->get<CPanel>()->objects.end(); i++){
        Entity* eSib = *i;
        if (eSib == e) continue;
        for(EntityListIt j = eSib->get<CPanel>()->objects.begin(); j != eSib->get<CPanel>()->objects.end(); j++){
            notify(REMOVE_PANEL, *j);
        }
        eSib->get<CPanel>()->objects.clear();
    }
}

void PanelSystem::onRemoveChildren(Entity* e){
    for(EntityListIt i = e->get<CPanel>()->objects.begin(); i != e->get<CPanel>()->objects.end(); i++){
        if ((*i)->has(Component::PANEL)){
            notify(REMOVE_PANEL, *i);
        }else{
            eManager->removeEntity(*i);
        }
    }
    e->get<CPanel>()->objects.clear();
}

void PanelSystem::onRemoveParent(Entity* e){
    Entity* eParent = e->get<CParentPanel>()->e;
    notify(REMOVE_CHILDREN, eParent);
    eManager->removeEntity(eParent);
}








