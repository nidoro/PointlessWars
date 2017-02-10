#ifndef WARSYSTEM_H
#define WARSYSTEM_H

#include "System.h"
#include "Assets.h"

using namespace std;

class WarSystem : public System {
    public:
        WarSystem();

        void update();

    private:
        void onInitializeWar(Entity* e);
        void onPacketReceived(Entity* e);
        void onEndMatch(Entity* e);
        void onSetMatchConfig(Entity* e);

        void initializeState();
        void sendStateToPeer();
        void sendAction(CPlayer::ID id);
        void processPackets();

        void callNextSystemAction();
        void startHeroPoolActionQueue(Entity* eFirst);
        void startFirstBattleActionQueue(Entity* eFirst);
        void startBattleActionQueue();
        void checkBattleClosure();

        list< pair<War::SystemAction, CPlayer::ID> > sysActionQueue;
        void addSystemAction(War::SystemAction sysAct, CPlayer::ID p = 0);
        std::list<sf::Packet> packetsQueue;

        bool inBattle;
        bool hasMatchEnded;
};

#endif
