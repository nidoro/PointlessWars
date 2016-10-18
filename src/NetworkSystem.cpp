#include "NetworkSystem.h"

NetworkSystem::NetworkSystem(){
    addSubscription(GAME_STARTED);
    addSubscription(TRY_AND_CONNECT);
    addSubscription(DISCONNECT_FROM_SERVER);
    addSubscription(FIND_MATCH);
    addSubscription(SEND_PACKET);

}

NetworkSystem::~NetworkSystem(){
    //dtor
}

void NetworkSystem::start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS){
    this->eManager = eManager;
    this->window = window;
    this->targetUPS = targetUPS;

/*
    wWindow = window->getView().getSize().x;
    hWindow = window->getView().getSize().y;
    cxWindow = wWindow/2;
    cyWindow = hWindow/2;
*/
    wWindow = 1280;
    hWindow = 720;
    cxWindow = wWindow/2;
    cyWindow = hWindow/2;

    for(list<Message>::iterator i = subscripts.begin(); i != subscripts.end(); i++){
        subscribe(*i);
    }

    serverPort = 8080;
    //serverIP = "139.102.241.42";
    serverIP = "10.139.2.187";
    serverSocket.setBlocking(false);

    connectionTimeOut = 60;
    state = OFFLINE;

    ipBuffSize = 16;
    portBuffSize = 6;
    ipBuff = new char[ipBuffSize];
    portBuff = new char[portBuffSize];
    clearBuff(ipBuff, ipBuffSize);
    clearBuff(portBuff, portBuffSize);

    //ipBuff = "10.139.2.187";
    //portBuff = "53001";
}
void NetworkSystem::update(){
    updateGUI();
    updateConnection();
    converse();
}

void NetworkSystem::updateGUI(){
    ImGui::Begin("Client");
        ImGuiInputTextFlags inputFlags = 0;
        if (state != OFFLINE) inputFlags = ImGuiInputTextFlags_ReadOnly;
        ImGui::PushItemWidth(120);
        ImGui::InputText("IP", ipBuff, ipBuffSize, inputFlags);
        ImGui::PushItemWidth(50);
        ImGui::InputText("Port", portBuff, portBuffSize, inputFlags);
        ImGui::Text("Status: %s", state != OFFLINE && state != CONNECTING ? "Online":"Offline");
        string labelButton = (state == OFFLINE) ? "Connect" : (state != CONNECTING ? "Disconnect":"Connecting...");
        Message m = (state == OFFLINE) ? TRY_AND_CONNECT : (state != CONNECTING ? DISCONNECT_FROM_SERVER:EMPTY_MESSAGE);
        if (ImGui::Button(labelButton.c_str())) notify(m);

        string str;
        Message ms;
        if (state != OFFLINE && state != CONNECTING){
            str = (state == FINDING_MATCH) ? "Finding Match..." : (state == PLAYING_MATCH ? "Leave Match" : "Find Match");
            ms = (state == FINDING_MATCH) ? EMPTY_MESSAGE : (state == PLAYING_MATCH ? EMPTY_MESSAGE : FIND_MATCH);
            if (ImGui::Button(str.c_str())) notify(ms);
        }
    ImGui::End();
}

void NetworkSystem::converse(){
    if (state != OFFLINE && state != CONNECTING){
        sf::Packet pkt;
        sf::Socket::Status sts;
        while ((sts = serverSocket.receive(pkt)) == sf::Socket::Done){
            processPacket(pkt);
        }
        if (sts == sf::Socket::Disconnected){
            notify(DISCONNECT_FROM_SERVER);
        }
        while (!sendQueue.empty() && serverSocket.send(sendQueue.front()) == sf::Socket::Done){
            sendQueue.pop_front();
        }
    }
}

void NetworkSystem::processPacket(sf::Packet& packet){
    sf::String id;
    sf::Packet packetOriginal = packet;
    packet >> id;
    if (id == "PING"){
        sf::Packet snd;
        snd << sf::String("PONG");
        serverSocket.send(snd);
    }else if (id == "MATCH-FOUND"){
        war = War();
        war.setMultiplayer(true);
        war.setRemotelyControled(0, true);
        //notify(INITIALIZE_WAR);
        state = PLAYING_MATCH;
        //startMatch();
    }else if (id == "LEAVE-MATCH"){
        //isPlayersTurn = false;
        printf("Opponent left...");
    }else if (id == "STATE-RECEIVED"){
        war.setPeerReceivedState(true);
    }else if (id == "SET-P1-CONTROLER"){
        war.setRemotelyControled(1, false);
        war.setRemotelyControled(2, true);
    }else if (id == "SET-P2-CONTROLER"){
        war.setRemotelyControled(1, true);
        war.setRemotelyControled(2, false);
    }else if (id == "SET-STATE-CONTROLER"){
        war.setRemotelyControled(0, false);
    }else if (id == "INITIALIZE-WAR"){
        notify(INITIALIZE_WAR);
    }else{
        Entity* ePacket = eManager->createEntity();
        ePacket->add(new CPacket(packetOriginal));
        notify(PACKET_RECEIVED, ePacket);
    }
}

void NetworkSystem::onSendPacket(Entity* e){
    sendQueue.push_back(e->get<CPacket>()->packet);
}

void NetworkSystem::updateConnection(){
    if (state == CONNECTING){
        if (connectionClock.getElapsedTime().asSeconds() >= connectionTimeOut){
            state = OFFLINE;

            //addToDebugLog("connection failed!");
        }else{
            sf::Socket::Status status = serverSocket.connect(serverIP, serverPort);
            printf("connecting to %s, status: %d\n", serverSocket.getRemoteAddress().toString().c_str(), status);
            if (status == sf::Socket::Done){
                state = JUST_ONLINE;
                printf("connected to %s", serverSocket.getRemoteAddress().toString().c_str());
                //addToDebugLog("connected to " + serverSocket.getRemoteAddress().toString());
            }
        }
    }
}

void NetworkSystem::onTryAndConnect(Entity* e){
    if (state != OFFLINE){
        //addToDebugLog("already connected!");
    }else{
        serverIP = ipBuff;
        serverPort = str2int(portBuff);
        if (!checkNumOnly(portBuff)){
            //addToDebugLog("invalid port, numbers only!");
        }else{
            //addToDebugLog("connecting to " + serverIP.toString());
            printf("connecting to %s", serverIP.toString().c_str());
            state = CONNECTING;
            connectionClock.restart();
        }
    }
}

void NetworkSystem::onDisconnectFromServer(Entity* e){
    if (state != OFFLINE){
        serverSocket.disconnect();
        state = OFFLINE;
        printf("disconnected from %s", serverSocket.getRemoteAddress().toString().c_str());
        //addToDebugLog("disconnected from server...");
    }
}

void NetworkSystem::onFindMatch(Entity* e){
    sf::Packet packet;
    packet << sf::String("FIND-MATCH");
    packet << sf::String("Standard");
    sendQueue.push_back(packet);
    state = FINDING_MATCH;
}
