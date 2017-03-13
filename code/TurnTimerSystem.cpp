#include "TurnTimerSystem.h"

TurnTimerSystem::TurnTimerSystem() {
    addSubscription(SYSTEM_ACTION);
    addSubscription(SELECT_ACTION);
    
    addRequirement(Component::TURN_TIMER);
}

void TurnTimerSystem::update() {
    for (Entity* e : entities) {
        e->get<CTurnTimer>()->remaining -= delay;
        if (e->get<CTurnTimer>()->remaining <= 0) {
            notify(PLAYER_TURN_TIME_EXPIRED);
            eManager->removeEntity(e);
        } else {
            e->get<CTextbox2>()->content.setString(int2str((int) e->get<CTurnTimer>()->remaining));
        }
    }
}

void TurnTimerSystem::createTimer(float length) {
    float xPosition = cxWindow;
    float yPosition = 200;
    float timeOfAppearance = length - 5;
    
    Entity* e = eManager->createEntity();
    e->add(new CPosition(xPosition, yPosition));
    e->add(new CDraw(CDraw::GUI3, true, 255.f));
    e->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::Black));
    e->add(new CTurnTimer(length+1));
    e->add(new CActor());
    e->get<CActor>()->addNode(new AVariable(timeOfAppearance, AVariable::HIDDEN, false));
    
    Entity* ePoof = eManager->createEntity();
    ePoof->attachEmployer(e);
    ePoof->add(new CPosition(xPosition, yPosition));
    ePoof->add(new CDraw(CDraw::GUI4));
    ePoof->add(new CAnimation(false, "alpha.png"));
    ePoof->add(new CActor());
    ePoof->get<CActor>()->addNode(new ASpriteAnimation(timeOfAppearance, "poof-02.png"));
    ePoof->get<CActor>()->addNode(new ADestroy(Assets::getAnimation("poof-02.png").duration));
    
    Entity* eObj = nullptr;
    sf::RectangleShape rect;

    // @note: red particle emitter
    rect.setSize(sf::Vector2f(4, 4));
    rect.setFillColor(sf::Color(255, 0, 0));
    eObj = eManager->createEntity();
    eObj->attachEmployer(e);
    eObj->add(new CPosition(xPosition + 40, yPosition));
    eObj->add(new CParticleEmmiter(30, rect, 0.125, 70, 90, 0, 360, 5));
    eObj->add(new CActor());
    eObj->get<CActor>()->addNode(new AVariable(timeOfAppearance, AVariable::PARTICLE_EMMITER, true));
}

void TurnTimerSystem::onSystemAction(Entity* e) {
    Entity* ePlayer = war.getActor();
    if (!ePlayer || ePlayer->has(Component::AI)) return;
    if (!ePlayer || (war.getLocalPlayer() != ePlayer && war.getMultiplayer())) return;
    CPlayer::ID playerID = ePlayer->get<CPlayer>()->id;
    
    if (war.getSystemAction() == war.ASK_HERO_PICK) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_FORMATION) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_CAPTAIN_ACTION) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_ARMY_ACTION) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_BATTLE_CLOSURE) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_ARMY_ASSEMBLE) {
        createTimer(10);
    } else if (war.getSystemAction() == war.ASK_CAPTAIN_SELECTION) {
        createTimer(10);
    }
}

void TurnTimerSystem::onSelectAction(Entity* e) {
    removeTimer();
}

void TurnTimerSystem::removeTimer() {
    float xPosition = cxWindow;
    float yPosition = 200;
    
    if (!entities.empty() && !eManager->isDead(entities.front())) {
        if (!entities.front()->get<CDraw>()->isHidden) {
            Entity* ePoof = eManager->createEntity();
            ePoof->add(new CPosition(xPosition, yPosition));
            ePoof->add(new CDraw(CDraw::GUI4));
            ePoof->add(new CAnimation(false, "alpha.png"));
            ePoof->add(new CActor());
            ePoof->get<CActor>()->addNode(new ASpriteAnimation(0.f, "poof-02.png"));
            ePoof->get<CActor>()->addNode(new ADestroy(Assets::getAnimation("poof-02.png").duration));
        }
        eManager->removeEntity(entities.front());
    }
}
