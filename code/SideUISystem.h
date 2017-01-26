#ifndef SIDEUISYSTEM_H
#define SIDEUISYSTEM_H

#include "System.h"
#include "Standard.h"

class SideUISystem : public System {
    public:
        SideUISystem();
        ~SideUISystem();

        void update();

    private:
        void onButtonGainedFocus(Entity* e);
        void onButtonLostFocus(Entity* e);
        void onEndMatch(Entity* e);
        void onInitializeWar(Entity* e);

        void createUI(Entity* ePlayer);
        list<CCaptain::ID> getHeroesInUI(Entity* e);
        void updateUI(Entity* e);
        void addHero(Entity* e, CCaptain::ID heroID);
        void removeHero(Entity* e, CCaptain::ID heroID);
        void rearrangeHeroes(Entity* e);
        int getHeroCountInUI(Entity* e);
        Entity* getHeroInUI(Entity* e, CCaptain::ID id);
        sf::Vector2f getHeroesOrigin(Entity* e);
        void showUI(Entity* e);
        void hideUI(Entity* e);

        Entity* eLeft;
        Entity* eRight;

        double wHeroButton;
        double hHeroButton;
        double buttonsSpacing;
        double xOffWhenHidden;
        double buttonsSpeed;
        bool active;

};

#endif // SIDEUISYSTEM_H
