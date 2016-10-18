#ifndef ACTIONOUTCOME_H
#define ACTIONOUTCOME_H

#include "Component.h"
#include <SFML/Network.hpp>

struct UnitActionOutcome{
    enum Outcome{BLOCKED, DIED, SUCESS, FAIL};
    UnitActionOutcome(){}
    UnitActionOutcome(int idCauser, Outcome id){
        this->idCauser = idCauser;
        this->id = id;
    }
    Outcome id;
    int idCauser;
    int idTarget;
};

class ActionOutcome{
    public:
        CAction::ID action;

        ActionOutcome();
        ActionOutcome(CAction::ID id, CPlayer::ID idActor, CPlayer::ID idEnemy){
            action = id;
            this->idActor = idActor;
            this->idEnemy = idEnemy;
            ready = false;
        }

        CPlayer::ID idActor;
        CPlayer::ID idEnemy;

        CPlayer::ID idWinner;
        CPlayer::ID idLoser;

        list<UnitActionOutcome> unitActionOutcomes;

        list<int> idTargets;
        list<int> idActors;
        int nTargets;
        int nActors;

        ///POOLS
        list<CCaptain::ID> heroes;
        list<CUnit::ID> units;

        CCaptain::ID hero;
        CUnit::ID unit;

        ///ARMY COMPOSITION
        map<CUnit::ID, int> armyComposition;

        ///FORMATION
        CArmy::Formation formation;

        ///DAMAGE
        CUnit::DamageType dmgType;

        int iValue;
        double dValue;
        string sValue;
        bool bValue;

        bool ready;
};

sf::Packet& operator <<(sf::Packet& packet, const ActionOutcome& outcome);

sf::Packet& operator >>(sf::Packet& packet, ActionOutcome& outcome);

#endif // ACTIONOUTCOME_H
