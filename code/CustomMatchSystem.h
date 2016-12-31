#ifndef CUSTOMMATCHSYSTEM_H
#define CUSTOMMATCHSYSTEM_H

#include "System.h"
#include "Assets.h"

class CustomMatchSystem : public System{
    public:
        CustomMatchSystem();
        ~CustomMatchSystem();

        void update();
    private:
        void onCreateScreen(Entity* e);
        void onStartCustomMatch(Entity* e);
        void saveConfiguration(string name);

        void createMenu();

        Entity* ePlayer1;
        Entity* ePlayer2;
        Entity* eColor1;
        Entity* eColor2;
        Entity* eScenario;
        Entity* eTurns;
        Entity* eBattles;
        Entity* eArmySize;
        Entity* ePlayerHeroes;
        Entity* eUnitOptions;
        Entity* eStreak;
        list<Entity*> eUnitPool;
        list<Entity*> eHeroPool;

        MatchConfig current;

        Entity* eStart;

        bool onMenu;
};

#endif // CUSTOMMATCHSYSTEM_H
