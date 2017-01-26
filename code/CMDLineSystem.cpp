#include "CMDLineSystem.h"

CMDLineSystem::CMDLineSystem() {
    eCommand = nullptr;
}

CMDLineSystem::~CMDLineSystem() {
    //dtor
}

void CMDLineSystem::update() {
    for(auto& command : commands) {
        std::stringstream ss(command);
        string str;
        ss >> eCommand->get<CCommandLine>()->command;
        string lastParam;
        eCommand->get<CCommandLine>()->params.clear();
        while (ss >> str) {
            if (str[0] == '-') {
                str.erase(str.begin());
                eCommand->get<CCommandLine>()->params.insert(make_pair(str, ""));
                lastParam = str;
            } else if (!eCommand->get<CCommandLine>()->params.empty()) {
                eCommand->get<CCommandLine>()->params[lastParam] = str;
            }
        }

        notify(NEW_COMMAND_LINE, eCommand);
    }
    commands.clear();
}

void CMDLineSystem::start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS) {
    System::start(eManager, window, targetUPS);

    ///INPUT THREAD
    userInputThread = std::thread(readUserInput, ref(commands));
    userInputThread.detach();

    eCommand = eManager->createEntity();
    eCommand->add(new CCommandLine());
    eCommand->add(new CSystem());
}

void CMDLineSystem::stop() {
    System::stop();
}

void readUserInput(list<string>& commands) {
    int buffSize = 255;
    char input[255];
    while(1) {
        std::cin.getline(input, 255);
        commands.push_back(input);
        for(int j = 0; j < buffSize; j++) input[j] = '\0';
    }
}
