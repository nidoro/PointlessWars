#ifndef MATCHCONFIG_H
#define MATCHCONFIG_H

#include <sstream>
#include <SFML/Network.hpp>
#include "Standard.h"
#include "tinyxml2.h"
#include "Component.h"
#include "OSAppDataDir.h"

class MatchConfig {
    public:
        MatchConfig();

        void loadFromFile(string name);

        string name;
        string scenario;
        int nTurns;
        int armySize;
        int nUnitOpt;
        int recruitGroup;
        double uBlockChance;
        double baseBlockChance;
        int streakToWin;
        int maxResistance;
        int nPlayerHeroes;
        int maxBattles;
        bool randomArmy;
        int maxRepicks;
        double turnDuration;
        list<CCaptain::ID> heroPool;
        list<CUnit::ID> unitPool;
};

#endif // MATCHCONFIG_H
