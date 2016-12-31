#ifndef SCENARIOSYSTEM_H
#define SCENARIOSYSTEM_H

#include "System.h"
#include "Assets.h"

class ScenarioSystem : public System{
    public:
        ScenarioSystem();
        ~ScenarioSystem();
    private:
        void update();
        void load(string name, double xOff = 0, double yOff = 0);
        void clear();
        string getRandom(string except = "");

        void onInitializeWar(Entity* e);
        void onLoadScenario(Entity* e);
        void onNewCommandLine(Entity* e);

        string current;
	list<string> regularScenarios;
};

#endif // SCENARIOSYSTEM_H
