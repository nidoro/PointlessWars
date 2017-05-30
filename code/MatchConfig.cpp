#include "MatchConfig.h"

std::list<CCaptain::ID> MatchConfig::allowedHeroes = {2, 3, 4, 5, 8, 10};
std::list<CUnit::ID> MatchConfig::allowedUnits = {0, 1, 4, 5, 8, 9, 12, 13};

MatchConfig::MatchConfig() {
    name = "Prototype";
    scenario = "scenario-00";
    nTurns = 3;
    armySize = 50;
    nUnitOpt = 5;
    recruitGroup = 5;
    uBlockChance = 0.25;
    baseBlockChance = 0.10;
    streakToWin = 2;
    maxResistance = 3;
    nPlayerHeroes = 3;
    maxBattles = 100;
    randomArmy = false;
    maxRepicks = 3;
    durationHeroPick = 20;
    durationBattleActions = 20;
    durationRecruitment = 40;
    heroPool.clear();
    unitPool.clear();
}

string xmldoc2string(tinyxml2::XMLDocument& doc, bool compact = true) {
    tinyxml2::XMLPrinter printer(nullptr, compact);
    doc.Print(&printer);
    return string(printer.CStr());
}

sf::Packet& operator <<(sf::Packet& packet, tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLPrinter printer(nullptr, false);
    doc.Print(&printer);
    return packet << printer.CStr();
}
sf::Packet& operator >>(sf::Packet& packet, tinyxml2::XMLDocument& doc) {
    string s;
    packet >> s;
    doc.Parse(s.c_str());
    return packet;
}

void MatchConfig::loadFromFile(string name) {
    MatchConfig();

    tinyxml2::XMLDocument doc;
    string path = helper::getAppDataDir() + "/match-config.xml";
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR) {
        printf("Could not open match-config.xml!\n");
        return;
    }
    tinyxml2::XMLElement* element = doc.FirstChildElement("MatchConfig");
    while (element) {
        string configName = element->FirstChildElement("Name")->GetText();
        if (configName == name) {
            break;
        }
        element = element->NextSiblingElement("MatchConfig");
    }
    if (!element) {
        printf("Could not find match config %s\n", name.c_str());
        return;
    }
    /*
        std::stringstream ssDocTest;
        ssDocTest << "<Prototype>";
            ssDocTest << "<Name>Prototype</Name>";
            ssDocTest << "<HeroesPerPlayer>3</HeroesPerPlayer>";
            ssDocTest << "<MaxBattles>NONE</MaxBattles>";
            ssDocTest << "<Turns>3</Turns>";
            ssDocTest << "<ArmySize>50</ArmySize>";
            ssDocTest << "<UnitaryBlockChance>0.25</UnitaryBlockChance>";
            ssDocTest << "<OptionsOnRebuild>16</OptionsOnRebuild>";
            ssDocTest << "<RecruitGroup>10</RecruitGroup>";
            ssDocTest << "<GoalScore>3</GoalScore>";
            ssDocTest << "<RandomArmy>false</RandomArmy>";
            ssDocTest << "<UnitPool>00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15</UnitPool>";
            ssDocTest << "<HeroPool>00 01 02 03 04 05 06 07 08 09 10 11 12</HeroPool>";
        ssDocTest << "</Prototype>";

        tinyxml2::XMLDocument docTest;
        docTest.Parse(ssDocTest.str().c_str());

        sf::Packet pkt;
        pkt << xmldoc2string(docTest, false);
        tinyxml2::XMLDocument docOut;
        pkt >> docOut;

        string str = xmldoc2string(docOut, false);
        cout << str << endl;
    */

    this->name = element->FirstChildElement("Name")->GetText();
    this->scenario = element->FirstChildElement("Scenery")->GetText();
    element->FirstChildElement("Turns")->QueryIntText(&nTurns);
    element->FirstChildElement("HeroesPerPlayer")->QueryIntText(&nPlayerHeroes);
    element->FirstChildElement("MaxBattles")->QueryIntText(&maxBattles);
    element->FirstChildElement("ArmySize")->QueryIntText(&armySize);
    element->FirstChildElement("UnitaryBlockChance")->QueryDoubleText(&uBlockChance);
    element->FirstChildElement("BaseBlockChance")->QueryDoubleText(&baseBlockChance);
    element->FirstChildElement("OptionsOnRebuild")->QueryIntText(&nUnitOpt);
    element->FirstChildElement("RecruitGroup")->QueryIntText(&recruitGroup);
    element->FirstChildElement("GoalScore")->QueryIntText(&streakToWin);
    element->FirstChildElement("RandomArmy")->QueryBoolText(&randomArmy);
    
    element->FirstChildElement("TurnDuration")->FirstChildElement("HeroPick")->QueryDoubleText(&durationHeroPick);
    element->FirstChildElement("TurnDuration")->FirstChildElement("BattleActions")->QueryDoubleText(&durationBattleActions);
    element->FirstChildElement("TurnDuration")->FirstChildElement("Recruitment")->QueryDoubleText(&durationRecruitment);

    string sValue;

    stringstream fileUnitPool(element->FirstChildElement("UnitPool")->GetText());
    while(fileUnitPool >> sValue) {
        if (contains(allowedUnits, str2int(sValue))) {
            unitPool.push_back(str2int(sValue));
        }
    }

    stringstream fileHeroPool(element->FirstChildElement("HeroPool")->GetText());
    while(fileHeroPool >> sValue) {
        if (contains(allowedHeroes, str2int(sValue))) {
            heroPool.push_back(str2int(sValue));
        }
    }

}

sf::Packet& operator <<(sf::Packet& packet, const MatchConfig& match) {
    /*
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
    double durationHeroPick;
    double durationBattleActions;
    double durationRecruitment;
    list<CCaptain::ID> heroPool;
    list<CUnit::ID> unitPool;
     */
    packet << sf::String(match.name);
    packet << sf::String(match.scenario);
    packet << sf::Int32(match.nTurns);
    packet << sf::Int32(match.armySize);
    packet << sf::Int32(match.nUnitOpt);
    packet << sf::Int32(match.recruitGroup);
    packet << match.uBlockChance;
    packet << match.baseBlockChance;
    packet << sf::Int32(match.streakToWin);
    packet << sf::Int32(match.maxResistance);
    packet << sf::Int32(match.nPlayerHeroes);
    packet << sf::Int32(match.maxBattles);
    packet << match.randomArmy;
    packet << sf::Int32(match.maxRepicks);
    packet << match.durationHeroPick;
    packet << match.durationBattleActions;
    packet << match.durationRecruitment;

    packet << sf::Int32(match.heroPool.size());
    for (auto& i : match.heroPool) {
        if (contains(MatchConfig::allowedHeroes, i)) {
            packet << sf::Int32(i);
        }
    }

    packet << sf::Int32(match.unitPool.size());
    for (auto& i : match.unitPool) {
        if (contains(MatchConfig::allowedUnits, i)) {
            packet << sf::Int32(i);
        }
    }
    
    return packet;
}

sf::Packet& operator >>(sf::Packet& packet, MatchConfig& match) {
    /*
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
    double durationHeroPick;
    double durationBattleActions;
    double durationRecruitment;
    list<CCaptain::ID> heroPool;
    list<CUnit::ID> unitPool;
     */    
    
    sf::Int32 int32;
    sf::String sValue;
    
    packet >> sValue; match.name = sValue;
    packet >> sValue; match.scenario = sValue;
    packet >> int32; match.nTurns = int32;
    packet >> int32; match.armySize = int32;
    packet >> int32; match.nUnitOpt = int32;
    packet >> int32; match.recruitGroup = int32;
    packet >> match.uBlockChance;
    packet >> match.baseBlockChance;
    packet >> int32; match.streakToWin = int32;
    packet >> int32; match.maxResistance = int32;
    packet >> int32; match.nPlayerHeroes = int32;
    packet >> int32; match.maxBattles = int32;
    packet >> match.randomArmy;
    packet >> int32; match.maxRepicks = int32;
    packet >> match.durationHeroPick;
    packet >> match.durationBattleActions;
    packet >> match.durationRecruitment;
    
    int heroPoolSize;
    packet >> int32; heroPoolSize = int32;
    for (int i = 0; i < heroPoolSize; ++i) {
        packet >> int32; 
        if (contains(MatchConfig::allowedHeroes, int32)) {
            match.heroPool.push_back((CCaptain::ID) ((int)int32));
        }
    }

    int unitPoolSize;
    packet >> int32; unitPoolSize = int32;
    for (int i = 0; i < unitPoolSize; ++i) {
        packet >> int32; 
        if (contains(MatchConfig::allowedUnits, int32)) {
            match.unitPool.push_back((CUnit::ID) ((int)int32));
        }
    }
    
    return packet;
}

















