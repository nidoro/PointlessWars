#ifndef ACTIONSYSTEM_H
#define ACTIONSYSTEM_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

class ActionSystem : public System {
    public:
        ActionSystem();

    private:
        void update();

        void executeAction(CPlayer::ID id);
        void createArmy(Entity* e);
        void composeArmy(Entity* e);
        void doNothing(Entity* e);
        
        // @note: calls the appropriated preprocessor
        void preprocessAction(CPlayer::ID id);
        // @note: system actions preprocessors
        void preprocessCoinThrow(ActionOutcome& outcome, Entity* e);
        // @note: hero abilities preprocessors
        void preprocessIntimidation(ActionOutcome& outcome, Entity* e);
        void preprocessArmyComposition(ActionOutcome& outcome, Entity* e);
        void preprocessRandomArmy(ActionOutcome& outcome, Entity* e);
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
        // @note: unit attack preprocessor
        void preprocessUnitAttack(ActionOutcome& outcome, Entity* e);
        void preprocessVassalAttack(ActionOutcome& outcome, Entity* e);
        void orderTargets(EntityList& targets, map<CAction::ID, CAction>& effects, CUnit::DamageType dmg);
        // @note: battle closure preprocessors
        void preprocessArmyVsArmy(ActionOutcome& outcome, Entity* e);
        void preprocessManVsMan(ActionOutcome& outcome, Entity* e);
        void preprocessEnslave(ActionOutcome& outcome, Entity* e);
        void preprocessMercy(ActionOutcome& outcome, Entity* e);

        // @note: helpers
        bool mapContains(map<CAction::ID, CAction>& m, CAction::ID id);
        bool isCursed(Entity* e);
        EntityList getAliveUnits(Entity* e);
        EntityList getDeadUnits(Entity* e);

};

#endif // ACTIONSYSTEM_H
