#ifndef ACTIONSYSTEM_H
#define ACTIONSYSTEM_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

class ActionSystem : public System {
    public:
        ActionSystem();
        ~ActionSystem();

    private:
        void update();

        void onSceneEnded(Entity* e);

        void createArmy(Entity* e);
        void composeArmy(Entity* e);

        void playPresentArmy(Entity* e);
        void playAdvanceArmy(Entity* e);
        void playPresentCaptain(Entity* e);
        void playReturnArmy(Entity* e);
        void playRemoveDead(Entity* e);
        void playEnslave(Entity*);
        void playBecomeHuman(Entity* e);

        void playBuffDebuff(Entity* e, CUnit::DamageType dmgType, bool buff);
        void doNothing(Entity* e);

        void executeAction(CPlayer::ID id);
        
        ///PREPROCESSORS
        void preprocessAction(CPlayer::ID id);
        void preprocessIntimidation(ActionOutcome& outcome, Entity* e);
        void preprocessArmyComposition(ActionOutcome& outcome, Entity* e);
        void preprocessRandomArmy(ActionOutcome& outcome, Entity* e);
        void preprocessCoinThrow(ActionOutcome& outcome, Entity* e);
        void preprocessBuffDebuff(ActionOutcome& outcome, Entity* e);
        void preprocessConfusion(ActionOutcome& outcome, Entity* e);
        void preprocessSummon(ActionOutcome& outcome, Entity* e);
        void preprocessChangeFormation(ActionOutcome& outcome, Entity* e);
        void preprocessHex(ActionOutcome& outcome, Entity* e);
        void preprocessTarot(ActionOutcome& outcome, Entity* e);
        void preprocessPurify(ActionOutcome& outcome, Entity* e);
        void preprocessResurect(ActionOutcome& outcome, Entity* e);
        void preprocessCurse(ActionOutcome& outcome, Entity* e);
        void preprocessSomniferous(ActionOutcome& outcome, Entity* e);
        void preprocessSleep(ActionOutcome& outcome, Entity* e);
        void preprocessTimeDilatation(ActionOutcome& outcome, Entity* e);
        void preprocessSlavesCall(ActionOutcome& outcome, Entity* e);
        void preprocessConversion(ActionOutcome& outcome, Entity* e);
        void preprocessTrueForm(ActionOutcome& outcome, Entity* e);
        void preprocessImmortality(ActionOutcome& outcome, Entity* e);
        void preprocessDoom(ActionOutcome& outcome, Entity* e);
        void preprocessTeleport(ActionOutcome& outcome, Entity* e);
        void preprocessSwap(ActionOutcome& outcome, Entity* e);
        void preprocessStampede(ActionOutcome& outcome, Entity* e);
        void preprocessBecomeHuman(ActionOutcome& outcome, Entity* e);
        void preprocessCroak(ActionOutcome& outcome, Entity* e);
        void preprocessHelp(ActionOutcome& outcome, Entity* e);
        void preprocessUnitAttack(ActionOutcome& outcome, Entity* e);
        void preprocessArmyVsArmy(ActionOutcome& outcome, Entity* e);
        void preprocessManVsMan(ActionOutcome& outcome, Entity* e);
        void preprocessEnslave(ActionOutcome& outcome, Entity* e);
        void preprocessMercy(ActionOutcome& outcome, Entity* e);

        void orderTargets(EntityList& targets, map<CAction::ID, CAction>& effects, CUnit::DamageType dmg);

        void computeMeleeBattle(Entity* e, Entity* eOut);
        void playMeleeBattle(Entity* e);
        bool contains(map<CAction::ID, CAction>& m, CAction::ID id);
        bool mapContains(map<CAction::ID, CAction>& m, CAction::ID id);
        bool isCursed(Entity* e);

        EntityList getAliveUnits(Entity* e);
        EntityList getDeadUnits(Entity* e);

};

#endif // ACTIONSYSTEM_H
