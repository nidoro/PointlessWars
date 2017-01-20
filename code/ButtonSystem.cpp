#include "ButtonSystem.h"

ButtonSystem::ButtonSystem(){
    addRequirement(Component::BUTTON_STATE);
    addRequirement(Component::BUTTON_HITBOX);
    addRequirement(Component::BUTTON_TRIGGER);
    addRequirement(Component::POSITION);

    addSubscription(BUTTON_PRESSED);
    addSubscription(BUTTON_RELEASED);
    addSubscription(BUTTON_GAINED_FOCUS);
    addSubscription(KEY_RELEASED);
    addSubscription(LOCK_ALL_BUTTONS);
    addSubscription(UNLOCK_ALL_BUTTONS);
    addSubscription(NEW_SCREEN);
    addSubscription(TOGGLE_CHECK_BOX);
    addSubscription(BRING_UI_LAYER_FORWARD);
    addSubscription(WINDOW_LOST_FOCUS);
    addSubscription(WINDOW_GAINED_FOCUS);
    addSubscription(DO_TOGGLE_ACTION);

    uiLayers.push(CUILayer::NONE);
    topUILayer = CUILayer::NONE;
    windowFocused = true;

    //addRequirement(Component::TEXTURE);
    //addRequirement(Component::DEFAULT_TEXTURE);
}

ButtonSystem::~ButtonSystem(){
    //dtor
}
void ButtonSystem::update(){
    std::list<Entity*>::iterator it;
    for (it = entities.begin(); it != entities.end(); it++){
        Entity* e = *it;
        if (eManager->isDead(e)) continue;
        updateButtonState(e);
        if (e->has(Component::SIMPLE_BUTTON)){
            updateButtonColor(e);
        }else if (e->has(Component::RECT_BUTTON)){
            updateButtonRect(e);
        }else if (e->has(Component::CIRCLE_BUTTON)){
            updateButtonCircle(e);
        }else if (e->has(Component::DEFAULT_TEXTURE) || e->has(Component::BUTTON_TEXTURES)){
            updateButtonTexture(e);
        }
    }
}

void ButtonSystem::updateButtonState(Entity* e){
    /*
    if (topUILayer != CUILayer::NONE){
        if (!e->has(Component::UI_LAYER)
        || (e->has(Component::UI_LAYER) && e->get<CUILayer>()->layer != topUILayer)){
            setState(e, CButtonState::NON_ACTIVE);
            notify(BUTTON_LOST_FOCUS, e);
            notify(e->get<CButtonState>()->lostFocusMessage, e);
            return;
        }
    }
    */
    if (uiLayers.top() != CUILayer::NONE){
        if (!e->has(Component::UI_LAYER)
        || (e->has(Component::UI_LAYER) && e->get<CUILayer>()->layer != uiLayers.top())){
            setState(e, CButtonState::NON_ACTIVE);
            notify(BUTTON_LOST_FOCUS, e);
            notify(e->get<CButtonState>()->lostFocusMessage, e);
            return;
        }
    }
    if (e->get<CButtonState>()->state == CButtonState::LOCKED){
        notify(BUTTON_LOST_FOCUS, e);
        notify(e->get<CButtonState>()->lostFocusMessage, e);
        return;
    }
    if (e->get<CButtonState>()->isDependent) return;
    CButtonHitbox* cHitbox = e->get<CButtonHitbox>();
    CButtonState* cState = e->get<CButtonState>();
    CPosition* cPosition = e->get<CPosition>();
    sf::FloatRect hitbox(cPosition->x - cHitbox->width/2, cPosition->y - cHitbox->height/2, cHitbox->width, cHitbox->height);
    sf::Vector2i mousePosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
    sf::Vector2f mp = window->mapPixelToCoords(mousePosition);
    if (cHitbox->shape == CButtonHitbox::RECT && hitbox.contains(mp)){
        if (cState->state == CButtonState::NON_ACTIVE){
            notify(BUTTON_GAINED_FOCUS, e);
            notify(e->get<CButtonState>()->gainedFocusMessage, e);
        }

        if (sf::Mouse::isButtonPressed(e->get<CButtonTrigger>()->mouseButton)){
            if (cState->state != CButtonState::ACTIVE){
                notify(BUTTON_PRESSED, e);
            }
            setState(e, CButtonState::ACTIVE);
        }else if (cState->state == CButtonState::ACTIVE){
            notify(BUTTON_RELEASED, e);
            if (cState->state != CButtonState::LOCKED) setState(e, CButtonState::HOVERED);
        }else{
            setState(e, CButtonState::HOVERED);
        }
    }else if (cHitbox->shape == CButtonHitbox::CIRCLE
           && getDistance(mp.x, mp.y, cPosition->x, cPosition->y) <= cHitbox->radius){

        if (cState->state == CButtonState::NON_ACTIVE){
            notify(BUTTON_GAINED_FOCUS, e);
            notify(e->get<CButtonState>()->gainedFocusMessage, e);
        }

        if (sf::Mouse::isButtonPressed(e->get<CButtonTrigger>()->mouseButton)){
            if (cState->state != CButtonState::ACTIVE){
                notify(BUTTON_PRESSED, e);
            }
            setState(e, CButtonState::ACTIVE);
        }else if (cState->state == CButtonState::ACTIVE && e->has(Component::BUTTON_TRIGGER)){
            notify(BUTTON_RELEASED, e);
            if (cState->state != CButtonState::LOCKED) setState(e, CButtonState::HOVERED);
        }else{
            setState(e, CButtonState::HOVERED);
        }
    }else{
        if (cState->state != CButtonState::NON_ACTIVE || eManager->isDead(e)){
            notify(BUTTON_LOST_FOCUS, e);
            notify(e->get<CButtonState>()->lostFocusMessage, e);
        }
        setState(e, CButtonState::NON_ACTIVE);
    }
    if (eManager->isDead(e)){
        notify(BUTTON_LOST_FOCUS, e);
        notify(e->get<CButtonState>()->lostFocusMessage, e);
    }
}

void ButtonSystem::updateButtonTexture(Entity* e){
    if (e->has(Component::BUTTON_TEXTURES)){
        if (e->get<CButtonState>()->state == CButtonState::NON_ACTIVE){
            e->get<CTexture>()->file = e->get<CButtonTextures>()->def;
        }else if (e->get<CButtonState>()->state == CButtonState::HOVERED){
            e->get<CTexture>()->file = e->get<CButtonTextures>()->hov;
        }else if (e->get<CButtonState>()->state == CButtonState::ACTIVE){
            e->get<CTexture>()->file = e->get<CButtonTextures>()->act;
        }else{
            e->get<CTexture>()->file = e->get<CButtonTextures>()->def;
        }
        return;
    }

    CButtonState* cState = e->get<CButtonState>();
    CTexture* cTexture = e->get<CTexture>();
    CDefaultTexture* cDefaultTex = e->get<CDefaultTexture>();
    if (cState->state == CButtonState::NON_ACTIVE){
        cTexture->file = cDefaultTex->file;
    }else if (cState->state == CButtonState::HOVERED && e->has(Component::HOVER_TEXTURE)){
        CHoverTexture* cHoverTex = e->get<CHoverTexture>();
        cTexture->file = cHoverTex->file;
    }else if (cState->state == CButtonState::ACTIVE && e->has(Component::ACTIVE_TEXTURE)){
        CActiveTexture* cActiveTex = e->get<CActiveTexture>();
        cTexture->file = cActiveTex->file;
    }else{
        cTexture->file = cDefaultTex->file;
    }
}

void ButtonSystem::updateButtonColor(Entity* e){
    CButtonState* cState = e->get<CButtonState>();
    //sf::Color color = e->get<CSimpleButton>()->shape.getFillColor();
    //sf::Color defColor = e->get<CSimpleButton>()->defaultColor;
    if (cState->state == CButtonState::NON_ACTIVE){
        e->get<CSimpleButton>()->shape.setFillColor(e->get<CSimpleButton>()->defaultColor);
    }else if (cState->state == CButtonState::HOVERED){
        e->get<CSimpleButton>()->shape.setFillColor(e->get<CSimpleButton>()->hoverColor);
    }else if (cState->state == CButtonState::ACTIVE){
        e->get<CSimpleButton>()->shape.setFillColor(e->get<CSimpleButton>()->activeColor);
    }else{
        e->get<CSimpleButton>()->shape.setFillColor(e->get<CSimpleButton>()->defaultColor);
    }
}

void ButtonSystem::updateButtonRect(Entity* e){
    CButtonState* cState = e->get<CButtonState>();
    if (cState->state == CButtonState::NON_ACTIVE){
        e->get<CRectShape>()->shape = e->get<CRectButton>()->defRect;
    }else if (cState->state == CButtonState::HOVERED){
        e->get<CRectShape>()->shape = e->get<CRectButton>()->hovRect;
    }else if (cState->state == CButtonState::ACTIVE){
        e->get<CRectShape>()->shape = e->get<CRectButton>()->actRect;
    }else{
        e->get<CRectShape>()->shape = e->get<CRectButton>()->defRect;
    }
}

void ButtonSystem::updateButtonCircle(Entity* e){
    CButtonState* cState = e->get<CButtonState>();
    if (cState->state == CButtonState::NON_ACTIVE){
        e->get<CCircleShape>()->shape = e->get<CCircleButton>()->defCircle;
    }else if (cState->state == CButtonState::HOVERED){
        e->get<CCircleShape>()->shape = e->get<CCircleButton>()->hovCircle;
    }else if (cState->state == CButtonState::ACTIVE){
        e->get<CCircleShape>()->shape = e->get<CCircleButton>()->actCircle;
    }else{
        e->get<CCircleShape>()->shape = e->get<CCircleButton>()->defCircle;
    }
}

void ButtonSystem::onButtonPressed(Entity* e){
    if (!windowFocused) return;
    if (e->get<CButtonState>()->state == CButtonState::LOCKED) return;
    if (e->has(Component::BUTTON_TRIGGER) && e->get<CButtonTrigger>()->action == CButtonTrigger::ON_PRESS){
        for (auto& m : e->get<CButtonTrigger>()->msgs){
            notify(m, e);
        }
    }
    if (!e->has(Component::BUTTON_SOUNDS)) return;
    Entity* eSound = eManager->createEntity();
    eSound->add(new CSound(e->get<CButtonSounds>()->pressed, CSound::REMOVE_ENTITY));
    notify(PLAY_SOUND, eSound);
}

void ButtonSystem::onButtonReleased(Entity* e){
    if (!windowFocused) return;
    if (e->get<CButtonState>()->state == CButtonState::LOCKED) return;

    if (e->has(Component::BUTTON_TRIGGER) && e->get<CButtonTrigger>()->action == CButtonTrigger::ON_RELEASE){
        for (auto& m : e->get<CButtonTrigger>()->msgs){
            notify(m, e);
        }
    }
    if (!e->has(Component::BUTTON_SOUNDS)) return;
    Entity* eSound = eManager->createEntity();
    eSound->add(new CSound(e->get<CButtonSounds>()->released, CSound::REMOVE_ENTITY));
    notify(PLAY_SOUND, eSound);
}

void ButtonSystem::onKeyReleased(Entity* e){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* eBt = *i;
        if (eBt->get<CButtonState>()->state == CButtonState::LOCKED) continue;
        /*
        if (topUILayer != CUILayer::NONE && (eBt->has(Component::UI_LAYER) &&  eBt->get<CUILayer>()->layer != topUILayer)
        || (topUILayer != CUILayer::NONE && !eBt->has(Component::UI_LAYER))){
            continue;
        }
        */
        if ((uiLayers.top() != CUILayer::NONE && (eBt->has(Component::UI_LAYER) &&  eBt->get<CUILayer>()->layer != uiLayers.top()))
        || (uiLayers.top() != CUILayer::NONE && !eBt->has(Component::UI_LAYER))){
            continue;
        }
        if (e->get<CKeyboardInput>()->code != sf::Keyboard::Unknown
        &&  e->get<CKeyboardInput>()->code == eBt->get<CButtonTrigger>()->hotkey){
            notify(BUTTON_RELEASED, eBt);
            break;
        }
    }
}

void ButtonSystem::onStartScreenTransition(Entity* e){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* eBt = *i;
        setState(eBt, CButtonState::LOCKED);
    }
}

void ButtonSystem::onLockAllButtons(Entity* e){
    buttonsLocked.push_back(EntityList());
    int lastLayer = buttonsLocked.size()-1;
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* eBt = *i;
        if (eBt->get<CButtonState>()->state != CButtonState::LOCKED){
            setState(eBt, CButtonState::LOCKED);
            buttonsLocked[lastLayer].push_back(eBt);
        }
    }
}

void ButtonSystem::lockButton(Entity* e){
    int lastLayer = buttonsLocked.size()-1;
    buttonsLocked[lastLayer].push_back(e);
}

void ButtonSystem::onUnlockAllButtons(Entity* e){
    int lastLayer = buttonsLocked.size()-1;
    for(EntityListIt i = buttonsLocked[lastLayer].begin(); i != buttonsLocked[lastLayer].end(); i++){
        Entity* eBt = *i;
        if (eManager->isDead(eBt)) continue;
        setState(eBt, CButtonState::NON_ACTIVE);
    }
    buttonsLocked[lastLayer].clear();
    buttonsLocked.pop_back();
}

void ButtonSystem::onNewScreen(Entity* e){
    buttonsLocked.clear();
    uiLayers = std::stack<CUILayer::Layer>();
    uiLayers.push(CUILayer::NONE);
}

void ButtonSystem::onButtonGainedFocus(Entity* e){
    if (!e->has(Component::BUTTON_SOUNDS)) return;

    Entity* eSound = eManager->createEntity();
    eSound->add(new CSound(e->get<CButtonSounds>()->focused, CSound::REMOVE_ENTITY));
    notify(PLAY_SOUND, eSound);
}

void ButtonSystem::onToggleCheckBox(Entity* e){
    e->get<CCheckBox>()->on = opposite(e->get<CCheckBox>()->on);
    if (e->get<CCheckBox>()->on){
        e->get<CTexture>()->file = e->get<CCheckBox>()->onTexture;
    }else{
        e->get<CTexture>()->file = e->get<CCheckBox>()->offTexture;
    }
}

void ButtonSystem::onBringUILayerForward(Entity* e){
    if (e && e->has(Component::UI_LAYER)){
        topUILayer = e->get<CUILayer>()->layer;
        uiLayers.push(e->get<CUILayer>()->layer);
    }else{
        topUILayer = CUILayer::NONE;
        if (uiLayers.size() > 1){
            uiLayers.pop();
        }
    }
}

void ButtonSystem::onWindowLostFocus(Entity* e){
    windowFocused = false;
}

void ButtonSystem::onWindowGainedFocus(Entity* e){
    windowFocused = true;
}

void ButtonSystem::onDoToggleAction(Entity* e){
    if (e->has(Component::STRING_TOGGLE_BUTTON)){
        e->get<CStringToggleButton>()->toggle();

        notify(e->get<CStringToggleButton>()->msgOnToggle, e);
    }
}

void ButtonSystem::setState(Entity* e, CButtonState::State state){
    e->get<CButtonState>()->state = state;
    if (e->has(Component::COMPOUND_BUTTON)){
        EntityList children = e->getObservedEntities("DependentStateButton");
        for (auto& c : children){
            c->get<CButtonState>()->state = state;
        }
    }
}
