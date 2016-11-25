#ifndef MATCHCONFIG_H
#define MATCHCONFIG_H

#include "Standard.h"
#include "tinyxml2.h"
#include "Component.h"
#include "Assets.h"
#include <sstream>
#include <SFML/Network.hpp>

class MatchConfig{
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
