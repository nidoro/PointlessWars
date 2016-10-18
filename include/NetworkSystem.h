#ifndef NETWORKSYSTEM_H
#define NETWORKSYSTEM_H

#include "System.h"
#include <SFML/Network.hpp>
#include "imgui.h"

class NetworkSystem : public System{
    public:
        NetworkSystem();
        ~NetworkSystem();

        void update();
        void start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS);

    private:
        enum State{FINDING_MATCH, PLAYING_MATCH, JUST_ONLINE, OFFLINE, CONNECTING};

        /// EVENT HANDLERS
        void onTryAndConnect(Entity* e);
        void onDisconnectFromServer(Entity* e);
        void onFindMatch(Entity* e);
        void onSendPacket(Entity* e);
        //void onSendMessage(Entity* e);

        /// CALLED ON UPDATE
        void updateGUI();
        void updateConnection();
        void converse();
        void processPacket(sf::Packet& packet);

        /// SERVER
        sf::IpAddress serverIP;
        int serverPort;
        sf::TcpSocket serverSocket;
        list<sf::Packet> sendQueue;

        bool connected;
        bool connecting;
        State state;
        double connectionTimeOut;
        bool multiplayerMatch;
        sf::Clock connectionClock;

        char* ipBuff;
        char* portBuff;
        int ipBuffSize;
        int portBuffSize;
};

#endif // NETWORKSYSTEM_H
