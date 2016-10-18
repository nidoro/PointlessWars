#include "MatchConfig.h"
#include <SFML/Network.hpp>

MatchConfig::MatchConfig(){
    scenario = "scenario-00";
    nTurns = 3;
    armySize = 50;
    nUnitOpt = 5;
    recruitGroup = 5;
    uBlockChance = 0.3;
    streakToWin = 2;
    maxResistance = 3;
    nPlayerHeroes = 3;
    maxBattles = 100;
    randomArmy = false;
    maxRepicks = 3;
    turnDuration = 30;
    heroPool.clear();
    unitPool.clear();
}

string xmldoc2string(tinyxml2::XMLDocument& doc, bool compact = true){
    tinyxml2::XMLPrinter printer(nullptr, compact);
    doc.Print(&printer);
    return string(printer.CStr());
}

sf::Packet& operator <<(sf::Packet& packet, tinyxml2::XMLDocument& doc){
    tinyxml2::XMLPrinter printer(nullptr, false);
    doc.Print(&printer);
    return packet << printer.CStr();
}
sf::Packet& operator >>(sf::Packet& packet, tinyxml2::XMLDocument& doc){
    string s;
    packet >> s;
    doc.Parse(s.c_str());
}

void MatchConfig::loadFromFile(string name){
    MatchConfig();

    tinyxml2::XMLDocument doc;
    string path = Assets::rscRoot + "match-config.xml";
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR){
        printf("Could not open match-config.xml!\n");
        return;
    }
    tinyxml2::XMLElement* element = doc.FirstChildElement(name.c_str());
    if (!element){
        printf("Could not fing match-config %s\n", name.c_str());
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

    element->FirstChildElement("Turns")->QueryIntText(&nTurns);
    element->FirstChildElement("HeroesPerPlayer")->QueryIntText(&nPlayerHeroes);
    element->FirstChildElement("MaxBattles")->QueryIntText(&maxBattles);
    element->FirstChildElement("ArmySize")->QueryIntText(&armySize);
    element->FirstChildElement("UnitaryBlockChance")->QueryDoubleText(&uBlockChance);
    element->FirstChildElement("OptionsOnRebuild")->QueryIntText(&nUnitOpt);
    element->FirstChildElement("RecruitGroup")->QueryIntText(&recruitGroup);
    element->FirstChildElement("GoalScore")->QueryIntText(&streakToWin);
    element->FirstChildElement("RandomArmy")->QueryBoolText(&randomArmy);

    string sValue;

    stringstream fileUnitPool(element->FirstChildElement("UnitPool")->GetText());
    while(fileUnitPool >> sValue){
        unitPool.push_back(str2int(sValue));
    }

    stringstream fileHeroPool(element->FirstChildElement("HeroPool")->GetText());
    while(fileHeroPool >> sValue){
        heroPool.push_back(str2int(sValue));
    }

}
