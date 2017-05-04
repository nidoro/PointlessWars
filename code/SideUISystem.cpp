#include "SideUISystem.h"

SideUISystem::SideUISystem() {
    addSubscription(BUTTON_GAINED_FOCUS);
    addSubscription(BUTTON_LOST_FOCUS);
    addSubscription(END_MATCH);
    addSubscription(INITIALIZE_WAR);
    addRequirement(Component::PLAYER);

    eLeft = nullptr;
    eRight = nullptr;
    wHeroButton = 54;
    hHeroButton = 52;
    buttonsSpacing = 5;
    xOffWhenHidden = 100;
    buttonsSpeed = 350;
    active = false;
}

SideUISystem::~SideUISystem() {
    //dtor
}

void SideUISystem::update() {
    if (active) {
        for (Entity* e : entities) {
            //printf("%d\n", entities.size());
            CPlayer::Side side = e->get<CPlayer>()->side;
            if (side == CPlayer::LEFT && eLeft == nullptr) createUI(e);
            if (side == CPlayer::RIGHT && eRight == nullptr) createUI(e);

            if (eLeft != nullptr) updateUI(eLeft);
            if (eRight != nullptr) updateUI(eRight);
        }
    }
}

void SideUISystem::createUI(Entity* ePlayer) {
    int sign = ePlayer->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    Entity* eArea = eManager->createEntity();
    //printf("Side UI created\n");
    double wArea = 100;
    double hArea = hWindow;
    double x = cxWindow + sign*wWindow/2 - sign*(wArea/2);
    double y = cyWindow;

    eArea->add(new CPosition(x, y));
    eArea->add(new CButtonHitbox(wArea, hArea));
    eArea->add(new CButtonState());
    eArea->add(new CButtonTrigger());

    eArea->addObservedEntity("Player", ePlayer);

    Entity* eHeroes = eManager->createEntity();
    eHeroes->attachEmployer(eArea);
    eArea->addObservedEntity("Heroes", eHeroes);

    if (ePlayer->get<CPlayer>()->side == CPlayer::LEFT) eLeft = eArea;
    if (ePlayer->get<CPlayer>()->side == CPlayer::RIGHT) eRight = eArea;
}

void SideUISystem::updateUI(Entity* e) {
    Entity* ePlayer = e->getObservedEntity("Player");

    list<CCaptain::ID> heroesInUI = getHeroesInUI(e);

    for (auto heroID : ePlayer->get<CPlayer>()->heroDeck) {
        if (!contains(heroesInUI, heroID)) {
            addHero(e, heroID);
        }
    }
    for (auto heroID : heroesInUI) {
        if (!contains(ePlayer->get<CPlayer>()->heroDeck, heroID)) {
            removeHero(e, heroID);
        }
    }

    for (auto& p : ePlayer->get<CArmy>()->captains) {
        Entity* eHero = p.second;
        if (eHero->get<CCaptain>()->isConfined) {
            Entity* eDisplayer = getHeroInUI(e, eHero->get<CCaptain>()->id);
            Entity* eJailBars = eDisplayer->getObservedEntity("JailBars");
            if (!eJailBars) {
                eJailBars = eManager->createEntity();
                eJailBars->add(new CPosition(*eDisplayer->get<CPosition>()));
                eJailBars->add(new CDraw(CDraw::GUI_02));
                eJailBars->add(new CTexture("jail-bars.png"));
                eJailBars->add(new CAnchor());
                eJailBars->add(new CDimensions(wHeroButton, hHeroButton));

                eJailBars->attachEmployer(eDisplayer);
                eJailBars->addObservedEntity("Anchor", eDisplayer);
                eDisplayer->addObservedEntity("JailBars", eJailBars);
            }
        } else {
            Entity* eDisplayer = getHeroInUI(e, eHero->get<CCaptain>()->id);
            Entity* eJailBars = eDisplayer->getObservedEntity("JailBars");
            if (eJailBars && !eManager->isDead(eJailBars)) {
                eDisplayer->removeObservedEntity("JailBars");
                eManager->removeEntity(eJailBars);
            }
        }
    }
}

list<CCaptain::ID> SideUISystem::getHeroesInUI(Entity* e) {
    list<CCaptain::ID> l;
    EntityList eList = e->getObservedEntity("Heroes")->getObservedEntities();
    for (Entity* i : eList) {
        l.push_back(i->get<CCaptain>()->id);
    }
    return l;
}

Entity* getHeroInUI(Entity* e, CCaptain::ID idHero) {
    EntityList eList = e->getObservedEntity("Heroes")->getObservedEntities();
    for (Entity* i : eList) {
        if (i->get<CCaptain>()->id == idHero) return i;
    }
    return nullptr;
}

void SideUISystem::addHero(Entity* e, CCaptain::ID heroID) {
    int sign = e->getObservedEntity("Player")->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    Entity* eHeroes = e->getObservedEntity("Heroes");
    Entity* eHero = eManager->createEntity();
    sf::Vector2f origin = getHeroesOrigin(e);
    int nHeroes = getHeroCountInUI(e);

    eHero->add(new CPosition(e->get<CPosition>()->x + sign*xOffWhenHidden, origin.y + nHeroes*(hHeroButton + buttonsSpacing)));
    eHero->add(new CTexture(CCaptain::Map[heroID].portrait));
    eHero->add(new CDimensions(wHeroButton, hHeroButton));
    eHero->add(new CDraw(CDraw::GUI_01));
    eHero->add(new CActor());
    eHero->add(new CVelocity());
    eHero->add(new CButtonHitbox(wHeroButton, hHeroButton));
    eHero->add(new CButtonState());
    eHero->add(new CButtonTrigger());
    eHero->add(new CTooltip(sign == -1 ? CTooltip::HERO_RIGHT : CTooltip::HERO_LEFT));
    eHero->add(new CCaptain(CCaptain::Map[heroID]));

    eHero->attachEmployer(eHeroes);
    eHeroes->addObservedEntity(int2str(nHeroes+1), eHero);
}
void SideUISystem::removeHero(Entity* e, CCaptain::ID heroID) {
    Entity* eHeroes = e->getObservedEntity("Heroes");
    Entity* eHero = getHeroInUI(e, heroID);
    eManager->removeEntity(eHero);
    eHeroes->removeObservedEntity(eHero);
}
void SideUISystem::rearrangeHeroes(Entity* e) {
    EntityList eHeroes = e->getObservedEntity("Heroes")->getObservedEntities();
    sf::Vector2f pos = getHeroesOrigin(e);
    for (Entity* eHero : eHeroes) {
        eHero->get<CActor>()->addNode(new ATeleport(0.0, pos.x, pos.y));
        pos.y += hHeroButton + buttonsSpacing;
    }
}

int SideUISystem::getHeroCountInUI(Entity* e) {
    EntityList eList = e->getObservedEntity("Heroes")->getObservedEntities();
    return eList.size();
}

Entity* SideUISystem::getHeroInUI(Entity* e, CCaptain::ID id) {
    EntityList eList = e->getObservedEntity("Heroes")->getObservedEntities();
    for (Entity* eHero : eList) {
        if (eHero->get<CCaptain>()->id == id) return eHero;
    }
    return nullptr;
}

sf::Vector2f SideUISystem::getHeroesOrigin(Entity* e) {
    sf::Vector2f position;
    int nHeroes = getHeroCountInUI(e);
    position.x = e->get<CPosition>()->x;
    position.y = e->get<CPosition>()->y - (nHeroes-1)*hHeroButton/2;
    return position;
}

void SideUISystem::showUI(Entity* e) {
    EntityList eHeroes = e->getObservedEntity("Heroes")->getObservedEntities();
    sf::Vector2f pos = getHeroesOrigin(e);
    for (Entity* eHero : eHeroes) {
        eHero->get<CActor>()->addNode(new AMove(0.0, pos.x, pos.y, buttonsSpeed));
        pos.y += hHeroButton + buttonsSpacing;
    }
}
void SideUISystem::hideUI(Entity* e) {
    int sign = e->getObservedEntity("Player")->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    EntityList eHeroes = e->getObservedEntity("Heroes")->getObservedEntities();
    sf::Vector2f pos = getHeroesOrigin(e);
    for (Entity* eHero : eHeroes) {
        eHero->get<CActor>()->addNode(new AMove(0.0, pos.x + sign*xOffWhenHidden, pos.y, buttonsSpeed));
        pos.y += hHeroButton + buttonsSpacing;
    }
}

void SideUISystem::onButtonGainedFocus(Entity* e) {
    Entity* eUI = nullptr;
    if (e == eLeft) eUI = eLeft;
    if (e == eRight)  eUI = eRight;
    if (nullptr != eUI) showUI(eUI);
}

void SideUISystem::onButtonLostFocus(Entity* e) {
    Entity* eUI = nullptr;
    if (e == eLeft) eUI = eLeft;
    if (e == eRight)  eUI = eRight;
    if (nullptr != eUI) hideUI(eUI);
}

void SideUISystem::onEndMatch(Entity* e) {
    //printf("Ended Match\n");
    eLeft = nullptr;
    eRight = nullptr;
    active = false;
}

void SideUISystem::onInitializeWar(Entity* e) {
    active = true;
}





























