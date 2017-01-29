#include "NetworkSystem.h"

NetworkSystem::NetworkSystem() {
    addSubscription(GAME_STARTED);
    addSubscription(TRY_AND_CONNECT);
    addSubscription(DISCONNECT_FROM_SERVER);
    addSubscription(FIND_MATCH);
    addSubscription(SEND_PACKET);
    addSubscription(NEW_COMMAND_LINE);
    addSubscription(STOP_CONNECTION_TRY);
    addSubscription(CONNECTION_SUCCESSFUL);
    addSubscription(CONNECTION_LOST);
    addSubscription(SET_SERVER_MESSAGE_DISPLAYER_QUICK_MATCH);
    addSubscription(CANCEL_QUICK_MATCH_SEARCH);
    eQMDisplayer = nullptr;
}

NetworkSystem::~NetworkSystem() {
}

void NetworkSystem::start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS) {
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

    for(list<Message>::iterator i = subscripts.begin(); i != subscripts.end(); i++) {
        subscribe(*i);
    }

    serverPort = 8080;
    serverIP = "52.67.40.198";
    serverSocket.setBlocking(false);

    connectionTimeOut = 10;
    state = OFFLINE;

    ipBuffSize = 16;
    portBuffSize = 6;
    ipBuff = new char[ipBuffSize];
    portBuff = new char[portBuffSize];
    clearBuff(ipBuff, ipBuffSize);
    clearBuff(portBuff, portBuffSize);

    eInfoWin = nullptr;
}

void NetworkSystem::update() {
    //updateGUI();
    updateConnection();
    converse();

    if (!processQueue.empty()) {
        processPacket(processQueue.front());
        processQueue.pop_front();
    }
}

void NetworkSystem::converse() {
    if (state != OFFLINE && state != CONNECTING) {
        sf::Packet pkt;
        sf::Socket::Status sts;
        while ((sts = serverSocket.receive(pkt)) == sf::Socket::Done) {
            processQueue.push_back(pkt);
        }
        if (sts == sf::Socket::Disconnected) {
            notify(DISCONNECT_FROM_SERVER);
        }
        while (!sendQueue.empty() && serverSocket.send(sendQueue.front()) == sf::Socket::Done) {
            sendQueue.pop_front();
        }
    }
}

void NetworkSystem::processPacket(sf::Packet& packet) {
    sf::String id;
    sf::Packet packetOriginal = packet;
    packet >> id;
    if (id == "PING") {
        sf::Packet snd;
        snd << sf::String("PONG");
        serverSocket.send(snd);
    } else if (id == "MATCH-FOUND") {
        war = War();
        war.setMultiplayer(true);
        war.setRemotelyControled(0, true);
        //notify(INITIALIZE_WAR);
        state = PLAYING_MATCH;
        //startMatch();
    } else if (id == "LEAVE-MATCH") {
        //isPlayersTurn = false;
        printf("Opponent left...");
    } else if (id == "STATE-RECEIVED") {
        war.setPeerReceivedState(true);
    } else if (id == "SET-P1-CONTROLER") {
        war.setRemotelyControled(1, false);
        war.setRemotelyControled(2, true);
    } else if (id == "SET-P2-CONTROLER") {
        war.setRemotelyControled(1, true);
        war.setRemotelyControled(2, false);
    } else if (id == "SET-STATE-CONTROLER") {
        war.setRemotelyControled(0, false);
    } else if (id == "INITIALIZE-WAR") {
        war.getMatchConfig().loadFromFile("Prototype");
        Entity* eScreen = eManager->createEntity();
        eScreen->add(new CScreen(CScreen::MATCH, CScreen::FADE_BLACK));
        eScreen->add(new CTimer(3, START_SCREEN_TRANSITION));

        eQMDisplayer->add(new CTypingEffect(Assets::getString("MESSAGE-QUICK-MATCH-STARTING"), 50));
        notify(LOCK_ALL_BUTTONS);
        notify(BRING_UI_LAYER_FORWARD);
        //notify(INITIALIZE_WAR);
    } else {
        Entity* ePacket = eManager->createEntity();
        ePacket->add(new CPacket(packetOriginal));
        notify(PACKET_RECEIVED, ePacket);
    }
}

void NetworkSystem::onSendPacket(Entity* e) {
    sendQueue.push_back(e->get<CPacket>()->packet);
}

void NetworkSystem::updateConnection() {
    if (state == CONNECTING) {
        if (connectionClock.getElapsedTime().asSeconds() >= connectionTimeOut) {
            state = OFFLINE;
            createConnectionFailedWindow();
            //addToDebugLog("connection failed!");
        } else {
            sf::Socket::Status status = serverSocket.connect(serverIP, serverPort);
            //printf("connecting to %s, status: %d\n", serverIP.toString().c_str(), status);
            if (status == sf::Socket::Done) {
                state = JUST_ONLINE;
                notify(CONNECTION_SUCCESSFUL);
                printf("connected to %s/%d\n", serverSocket.getRemoteAddress().toString().c_str(), serverPort);
                //addToDebugLog("connected to " + serverSocket.getRemoteAddress().toString());
            }
        }
    }
}

void NetworkSystem::onConnectionSuccessful(Entity* e) {
    eManager->removeEntity(eInfoWin);
    notify(BRING_UI_LAYER_FORWARD);

    Entity* eObj = eManager->createEntity();
    eObj->add(new CGUIGroup("window", "multiplayer"));
    eObj->add(new CUILayer(CUILayer::L1));
    eObj->add(new CDraw(CDraw::GUI_00));
    eObj->addObservedEntity("create-gui-group", eObj);
    notify(CREATE_GUI_GROUP, eObj);
    eManager->removeEntity(eObj);
}

void NetworkSystem::onTryAndConnect(Entity* e) {
    if (state != OFFLINE) {
        notify(CONNECTION_SUCCESSFUL);
    } else {
        /*
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
        */
        printf("connecting to %s/%d\n", serverIP.toString().c_str(), serverPort);
        state = CONNECTING;
        connectionClock.restart();
        createInfoWindow();
    }
}

void NetworkSystem::onDisconnectFromServer(Entity* e) {
    if (state != OFFLINE && state != CONNECTING) {
        printf("disconnected from %s/%d\n", serverSocket.getRemoteAddress().toString().c_str(), serverPort);
        serverSocket.disconnect();
        state = OFFLINE;
        createConnectionLostWindow();
    }
}

void NetworkSystem::onFindMatch(Entity* e) {
    sf::Packet packet;
    packet << sf::String("FIND-MATCH");
    packet << sf::String("Standard");
    sendQueue.push_back(packet);
    state = SEARCHING_QUICK_MATCH;

    eQMDisplayer->add(new CTypingEffect(Assets::getString("MESSAGE-QUICK-MATCH-SEARCHING"), 50));
}

void NetworkSystem::onCancelQuickMatchSearch(Entity* e) {
    if (state == SEARCHING_QUICK_MATCH) {
        sf::Packet packet;
        packet << sf::String("CANCEL-QUICK-MATCH-SEARCH");
        sendQueue.push_back(packet);

        state = JUST_ONLINE;
    }
}

void NetworkSystem::onNewCommandLine(Entity* e) {
    if (e->get<CCommandLine>()->command == "connect-to-server") {
        if (e->get<CCommandLine>()->hasParam("port")) {
            serverPort = str2int(e->get<CCommandLine>()->params["port"]);
        }
        if (e->get<CCommandLine>()->hasParam("ip")) {
            serverIP = e->get<CCommandLine>()->params["ip"];
        }
        notify(TRY_AND_CONNECT);
    } else if (e->get<CCommandLine>()->command == "disconnect-from-server") {
        notify(DISCONNECT_FROM_SERVER);
    }
}

void NetworkSystem::createInfoWindow() {
    eManager->removeEntity(eInfoWin);

    string panelTexture = "9p-grey-frame-rounded.png";
    double xPanel = cxWindow;
    double yPanel = cyWindow;
    double wPanel = 350;
    double hPanel = 100;
    CUILayer::Layer uiLayer = CUILayer::L1;

    eInfoWin = eManager->createEntity();
    eInfoWin->add(new CPosition(xPanel, yPanel));
    eInfoWin->add(new CTexture(panelTexture));
    eInfoWin->add(new CDimensions(wPanel, hPanel));
    eInfoWin->add(new CDraw(CDraw::GUI_00));
    eInfoWin->add(new CUILayer(uiLayer));


    Entity* eObj;
    double x, y;
    x = xPanel;
    y = yPanel;
    double spacing = 30;

    /// Message
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y - spacing/2));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::White));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CTypingEffect(Assets::getString("MESSAGE-CONNECTING"), 40));
    eObj->attachEmployer(eInfoWin);
    eInfoWin->addObservedEntity("message", eObj);

    /// Button
    eObj = createRectButton(Assets::getString("LABEL-CANCEL"), 14, 30, x, y + spacing/2,
                            sf::Color::White, sf::Color::Black, sf::Color::White, 2, STOP_CONNECTION_TRY, uiLayer);
    eObj->attachEmployer(eInfoWin);

    notify(BRING_UI_LAYER_FORWARD, eInfoWin);
}

void NetworkSystem::createConnectionFailedWindow() {
    eManager->removeEntity(eInfoWin);

    string panelTexture = "9p-grey-frame-rounded.png";
    double xPanel = cxWindow;
    double yPanel = cyWindow;
    double wPanel = 350;
    double hPanel = 100;
    CUILayer::Layer uiLayer = CUILayer::L1;

    eInfoWin = eManager->createEntity();
    eInfoWin->add(new CPosition(xPanel, yPanel));
    eInfoWin->add(new CTexture(panelTexture));
    eInfoWin->add(new CDimensions(wPanel, hPanel));
    eInfoWin->add(new CDraw(CDraw::GUI_00));
    eInfoWin->add(new CUILayer(uiLayer));

    Entity* eObj;
    double x, y;
    x = xPanel;
    y = yPanel;
    double spacing = 30;

    /// Message
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y - spacing/2));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::White));
    eObj->add(new CDraw(CDraw::GUI_01));
    eObj->add(new CTypingEffect(Assets::getString("MESSAGE-CONNECTION-FAILED"), 40));
    eObj->attachEmployer(eInfoWin);
    eInfoWin->addObservedEntity("message", eObj);

    /// Button
    eObj = createRectButton(Assets::getString("LABEL-CANCEL"), 14, 30, x, y + spacing/2,
                            sf::Color::White, sf::Color::Black, sf::Color::White, 2, STOP_CONNECTION_TRY, uiLayer);
    eObj->attachEmployer(eInfoWin);

    notify(BRING_UI_LAYER_FORWARD, eInfoWin);
}

void NetworkSystem::createConnectionLostWindow() {
    eManager->removeEntity(eInfoWin);

    string panelTexture = "9p-grey-frame-rounded.png";
    double xPanel = cxWindow;
    double yPanel = cyWindow;
    double wPanel = 350;
    double hPanel = 100;
    CUILayer::Layer uiLayer = CUILayer::L5;

    eInfoWin = eManager->createEntity();
    eInfoWin->add(new CPosition(xPanel, yPanel));
    eInfoWin->add(new CTexture(panelTexture));
    eInfoWin->add(new CDimensions(wPanel, hPanel));
    eInfoWin->add(new CDraw(CDraw::GUI_04));
    eInfoWin->add(new CUILayer(uiLayer));

    Entity* eObj;
    double x, y;
    x = xPanel;
    y = yPanel;
    double spacing = 30;

    /// Message
    eObj = eManager->createEntity();
    eObj->add(new CPosition(x, y - spacing/2));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 14, sf::Color::White));
    eObj->add(new CDraw(CDraw::GUI_05));
    eObj->add(new CTypingEffect(Assets::getString("MESSAGE-CONNECTION-LOST"), 40));
    eObj->attachEmployer(eInfoWin);
    eInfoWin->addObservedEntity("message", eObj);

    /// Button
    eObj = createRectButton(Assets::getString("LABEL-OK"), 14, 30, x, y + spacing/2,
                            sf::Color::White, sf::Color::Black, sf::Color::White, 2, CONNECTION_LOST, uiLayer);
    eObj->get<CDraw>()->tag = CDraw::GUI_05;
    eObj->attachEmployer(eInfoWin);

    notify(BRING_UI_LAYER_FORWARD, eInfoWin);
}

void NetworkSystem::onStopConnectionTry(Entity* e) {
    notify(DISCONNECT_FROM_SERVER);
    eManager->removeEntity(eInfoWin);
    notify(BRING_UI_LAYER_FORWARD);
}

void NetworkSystem::onConnectionLost(Entity* e) {
    eManager->removeEntity(eInfoWin);
    notify(BRING_UI_LAYER_FORWARD);
}

Entity* NetworkSystem::createRectButton(string label, double fontSize, double h, double x, double y,
                                        sf::Color textColor, sf::Color fillColor, sf::Color outColor, double outThickness,
                                        Message m, CUILayer::Layer UILayer) {
    sf::RectangleShape rect;
    double w = 0;
    double sideSpacing = 10;
    Entity* e = eManager->createEntity();
    e->add(new CTextbox2(label, Assets::getFont(Assets::getPrimaryFont()), fontSize, textColor));
    w = e->get<CTextbox2>()->content.getLocalBounds().width + 2*sideSpacing;
    rect.setOutlineColor(outColor);
    rect.setOutlineThickness(outThickness);
    rect.setPosition(x, y);
    rect.setSize(sf::Vector2f(w, h));
    rect.setFillColor(fillColor);
    e->add(new CRectShape(sf::RectangleShape()));
    e->add(new CPosition(x, y));
    e->add(new CDimensions(w, h));
    e->add(new CButtonHitbox(w, h));
    e->add(new CButtonState());
    e->add(new CButtonTrigger(m));
    e->add(new CRectButton(sf::RectangleShape(), rect, rect));
    e->add(new CDraw(CDraw::GUI_01));
    e->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    e->add(new CUILayer(UILayer));
    return e;
}

void NetworkSystem::stop() {
    notify(DISCONNECT_FROM_SERVER);
}
void NetworkSystem::onSetServerMessageDisplayerQuickMatch(Entity* e) {
    eQMDisplayer = e;
}
void NetworkSystem::updateGUI() {
    /*
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
    */
}
