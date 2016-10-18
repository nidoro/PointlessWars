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
    if (topUILayer != CUILayer::NONE){
        if (!e->has(Component::UI_LAYER)
        || (e->has(Component::UI_LAYER) && e->get<CUILayer>()->layer != topUILayer)){
            e->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
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
            cState->state = CButtonState::ACTIVE;
        }else if (cState->state == CButtonState::ACTIVE){
            notify(BUTTON_RELEASED, e);
            if (cState->state != CButtonState::LOCKED) cState->state = CButtonState::HOVERED;
        }else{
            cState->state = CButtonState::HOVERED;
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
            cState->state = CButtonState::ACTIVE;
        }else if (cState->state == CButtonState::ACTIVE && e->has(Component::BUTTON_TRIGGER)){
            notify(BUTTON_RELEASED, e);
            if (cState->state != CButtonState::LOCKED) cState->state = CButtonState::HOVERED;
        }else{
            cState->state = CButtonState::HOVERED;
        }
    }else{
        if (cState->state != CButtonState::NON_ACTIVE || eManager->isDead(e)){
            notify(BUTTON_LOST_FOCUS, e);
            notify(e->get<CButtonState>()->lostFocusMessage, e);
        }
        cState->state = CButtonState::NON_ACTIVE;
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
    sf::Color color = e->get<CSimpleButton>()->shape.getFillColor();
    sf::Color defColor = e->get<CSimpleButton>()->defaultColor;
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
        notify(e->get<CButtonTrigger>()->message, e);
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
        notify(e->get<CButtonTrigger>()->message, e);
    }
}

void ButtonSystem::onKeyReleased(Entity* e){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* eBt = *i;
        if (eBt->get<CButtonState>()->state == CButtonState::LOCKED) continue;
        if (topUILayer != CUILayer::NONE && (eBt->has(Component::UI_LAYER) &&  eBt->get<CUILayer>()->layer != topUILayer)
        || (topUILayer != CUILayer::NONE && !eBt->has(Component::UI_LAYER))){
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
        eBt->get<CButtonState>()->state = CButtonState::LOCKED;
    }
}

void ButtonSystem::onLockAllButtons(Entity* e){
    buttonsLocked.push_back(EntityList());
    int lastLayer = buttonsLocked.size()-1;
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* eBt = *i;
        if (eBt->get<CButtonState>()->state != CButtonState::LOCKED){
            eBt->get<CButtonState>()->state = CButtonState::LOCKED;
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
        eBt->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
    }
    buttonsLocked[lastLayer].clear();
    buttonsLocked.pop_back();
}

void ButtonSystem::onNewScreen(Entity* e){
    buttonsLocked.clear();
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
    }else{
        topUILayer = CUILayer::NONE;
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
        notify(HAS_BEEN_TOGGLED, e);
    }
}
