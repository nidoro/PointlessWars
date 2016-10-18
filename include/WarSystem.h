#ifndef WARSYSTEM_H
#define WARSYSTEM_H

#include "System.h"
#include "Assets.h"

using namespace std;

struct GameObserver{
    int nHeroesP1;
    int nHeroesP2;
    bool isPlayingAnimation;
    CCaptain::ID hero;
    CArmy::Formation formation;
};

class WarSystem : public System{
    public:
        WarSystem();
        ~WarSystem();

        void update();

    private:
        void onInitializeWar(Entity* e);
        void onPacketReceived(Entity* e);

        void initializeState();
        void sendStateToPeer();
        void sendAction(CPlayer::ID id);

        void callNextSystemAction();
        void startHeroPoolActionQueue(Entity* eFirst);
        void startFirstBattleActionQueue(Entity* eFirst);
        void startBattleActionQueue();
        void checkBattleClosure();

        list< pair<War::SystemAction, CPlayer::ID> > sysActionQueue;
        void addSystemAction(War::SystemAction sysAct, CPlayer::ID p = 0);

        GameObserver gameObserver;
        bool inBattle;
};

#endif // WARSYSTEM_H
