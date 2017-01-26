#ifndef CMDLINESYSTEM_H
#define CMDLINESYSTEM_H

#include "System.h"
#include <thread>

class CMDLineSystem : public System {
    public:
        CMDLineSystem();
        ~CMDLineSystem();

        void update();
        void start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS);
        void stop();

    private:

        Entity* eCommand;
        list<string> commands;
        std::thread userInputThread;

};

void readUserInput(list<string>& commands);

#endif // CMDLINESYSTEM_H
