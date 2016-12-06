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
        void stop();

    private:
        enum State{FINDING_MATCH, SEARCHING_QUICK_MATCH, PLAYING_MATCH, JUST_ONLINE, OFFLINE, CONNECTING};

        /// EVENT HANDLERS
        void onTryAndConnect(Entity* e);
        void onDisconnectFromServer(Entity* e);
        void onFindMatch(Entity* e);
        void onSendPacket(Entity* e);
        void onNewCommandLine(Entity* e);
        void onStopConnectionTry(Entity* e);
        void onConnectionSuccessful(Entity* e);
        void onConnectionLost(Entity* e);
        void onSetServerMessageDisplayerQuickMatch(Entity* e);
        void onCancelQuickMatchSearch(Entity* e);
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
        list<sf::Packet> processQueue;

        ///CONNECTION WINDOW
        Entity* eInfoWin;
        void createInfoWindow();
        void createConnectionFailedWindow();
        void createConnectionLostWindow();
        Entity* createRectButton(string label, double fontSize, double h, double x, double y,
                                 sf::Color textColor, sf::Color fillColor, sf::Color outColor, double outThickness,
                                 Message m, CUILayer::Layer UILayer);

        Entity* eQMDisplayer;                       ///QUICK MATCH DISPLAYER

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
