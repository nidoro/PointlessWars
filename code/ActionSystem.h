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

        void onExecuteActions(Entity* e);
        void onStartBattleState(Entity* e);
        void onPlayAction(Entity* e);
        void onSceneEnded(Entity* e);
        void onSystemAction(Entity* e);

        void executeAction(Entity* e);
        void playAction(Entity* e);

        void createArmy(Entity* e);
        void composeArmy(Entity* e);

        void playPresentArmy(Entity* e);
        void playAdvanceArmy(Entity* e);
        void playPresentCaptain(Entity* e);
        void playReturnArmy(Entity* e);
        void playRemoveDead(Entity* e);
        void playUnitAttack(Entity* e, CUnit::ID);
        void playManVsMan(Entity*);
        void playEnslave(Entity*);
        void playIntimidate(Entity* e);
        void playFocusFire(Entity* e);
        void playResurrect(Entity* e);
        void playConvert(Entity* e);
        void playSummon(Entity* e);
        void playTrueForm(Entity* e);
        void playHex(Entity* e);
        void playSwapHeroes(Entity* e);
        void playPurify(Entity* e);
        void playConfusion(Entity* e);
        void playFocus(Entity* e, CUnit::DamageType dmgType);
        void playChangeFormation(Entity* e);
        void playMindControl(Entity* e);
        void playPoison(Entity* e);
        void playCurse(Entity* e);
        void playTeleport(Entity* e);
        void playHeroAttack(Entity* e, CAction::ID actID);
        void playCroak(Entity* e);
        void playSilence(Entity* e);
        void playImmortality(Entity* e);
        void playTarot(Entity* e);
        void playTimeDilatation(Entity* e);
        void playCallSlaves(Entity* e);
        void playStampede(Entity* e);
        void playBecomeHuman(Entity* e);

        void playFireBuffAction(Entity* e);
        void playWaterBuffAction(Entity* e);
        void playEarthBuffAction(Entity* e);
        void playFireDebuffAction(Entity* e);
        void playWaterDebuffAction(Entity* e);
        void playEarthDebuffAction(Entity* e);
        void playBuffDebuff(Entity* e, CUnit::DamageType dmgType, bool buff);
        void doNothing(Entity* e);

        ///NEW THINGS
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

        void throwCoin();
        void orderTargets(EntityList& targets, map<CAction::ID, CAction>& effects, CUnit::DamageType dmg);

        void playEffectAction(Entity* e);

        void computeMeleeBattle(Entity* e, Entity* eOut);
        void playMeleeBattle(Entity* e);
        bool contains(map<CAction::ID, CAction>& m, CAction::ID id);
        bool mapContains(map<CAction::ID, CAction>& m, CAction::ID id);


        EntityList getAliveUnits(Entity* e);
        EntityList getDeadUnits(Entity* e);

};

#endif // ACTIONSYSTEM_H
