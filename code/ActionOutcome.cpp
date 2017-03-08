#include "ActionOutcome.h"

ActionOutcome::ActionOutcome() {
    ready = false;
}

sf::Packet& operator <<(sf::Packet& packet, const ActionOutcome& outcome) {
    packet << sf::Int32(outcome.action);

    packet << sf::Int32(outcome.idActor);
    packet << sf::Int32(outcome.idEnemy);

    packet << sf::Int32(outcome.idWinner);
    packet << sf::Int32(outcome.idLoser);

    packet << sf::Int32(outcome.unitActionOutcomes.size());

    for (auto& i : outcome.unitActionOutcomes) {
        packet << sf::Int32(i.id);
        packet << sf::Int32(i.idCauser);
        packet << sf::Int32(i.idTarget);
    }

    packet << sf::Int32(outcome.idTargets.size());
    for (auto& i : outcome.idTargets) {
        packet << sf::Int32(i);
    }

    packet << sf::Int32(outcome.idActors.size());
    for (auto& i : outcome.idActors) {
        packet << sf::Int32(i);
    }

    packet << sf::Int32(outcome.idBackfire.size());
    for (auto& i : outcome.idBackfire) {
        packet << sf::Int32(i);
    }
    
    packet << sf::Int32(outcome.nTargets);
    packet << sf::Int32(outcome.nActors);

    packet << sf::Int32(outcome.heroes.size());
    for (auto& i : outcome.heroes) {
        packet << sf::Int32(i);
    }

    packet << sf::Int32(outcome.units.size());
    for (auto& i : outcome.units) {
        packet << sf::Int32(i);
    }

    packet << sf::Int32(outcome.hero);
    packet << sf::Int32(outcome.unit);

    //compos
    packet << sf::Int32(outcome.armyComposition.size());
    for (auto& i : outcome.armyComposition) {
        packet << i.first;
        packet << i.second;
    }

    packet << sf::Int32(outcome.formation);
    packet << sf::Int32(outcome.dmgType);

    packet << sf::Int32(outcome.iValue);
    packet << outcome.dValue;
    packet << sf::String(outcome.sValue);
    packet << outcome.bValue;

    packet << outcome.ready;

    return packet;
}

sf::Packet& operator >>(sf::Packet& packet, ActionOutcome& outcome) {
    sf::Int32 int32;
    sf::Int32 iValue;
    sf::String sValue;

    packet >> int32;
    outcome.action = int32;

    packet >> int32;
    outcome.idActor = int32;
    packet >> int32;
    outcome.idEnemy = int32;

    packet >> int32;
    outcome.idWinner = int32;
    packet >> int32;
    outcome.idLoser = int32;


    packet >> iValue;
    for (int i = 0; i < iValue; i++) {
        UnitActionOutcome uao;
        packet >> int32;
        uao.id = (UnitActionOutcome::Outcome)int32;
        packet >> int32;
        uao.idCauser = int32;
        packet >> int32;
        uao.idTarget = int32;
        outcome.unitActionOutcomes.push_back(uao);
    }

    packet >> iValue;
    for (int i = 0; i < iValue; i++) {
        packet >> int32;
        outcome.idTargets.push_back(int32);
    }

    packet >> iValue;
    for (int i = 0; i < iValue; i++) {
        packet >> int32;
        outcome.idActors.push_back(int32);
    }

    packet >> iValue;
    for (int i = 0; i < iValue; i++) {
        packet >> int32;
        outcome.idBackfire.push_back(int32);
    }
    
    packet >> int32;
    outcome.nTargets = int32;
    packet >> int32;
    outcome.nActors = int32;

    packet >> iValue;
    for (int i = 0; i < iValue; i++) {
        packet >> int32;
        outcome.heroes.push_back(int32);
    }

    packet >> iValue;
    for (int i = 0; i < iValue; i++) {
        packet >> int32;
        outcome.units.push_back(int32);
    }

    packet >> int32;
    outcome.hero = int32;
    packet >> int32;
    outcome.unit = int32;

    //comp
    packet >> iValue;
    sf::Int32(outcome.armyComposition.size());
    for (int i = 0; i < iValue; i++) {
        pair<CUnit::ID, int> p;
        packet >> int32;
        p.first = int32;
        packet >> int32;
        p.second = int32;
        outcome.armyComposition.insert(p);
    }

    packet >> int32;
    outcome.formation = (CArmy::Formation)int32;
    packet >> int32;
    outcome.dmgType = (CUnit::DamageType)int32;

    packet >> int32;
    outcome.iValue = int32;
    packet >> outcome.dValue;
    packet >> sValue;
    outcome.sValue = sValue;
    packet >> outcome.bValue;

    packet >> outcome.ready;

    return packet;
}


