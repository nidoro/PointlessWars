#ifndef SCRIPTEDANIMATION_H
#define SCRIPTEDANIMATION_H

#include "System.h"
#include "Assets.h"

class ScriptedAnimation : public System {
    public:
        ScriptedAnimation();

        void update();
        
    private:
        void onTriggerObjectAnimation(Entity* e);
        void onGameStarted(Entity* e);
        void onInitializeWar(Entity* e);
        void onSystemAction(Entity* e);
        void onSceneStarted(Entity* e);
        void onPlayAction(Entity* e);
        void onAddActor(Entity* e);
        void onNewScreen(Entity* e);

        void triggerNode(Entity* e, AnimationNode* node);
        void popFrontAnimationNode(Entity* e);
        void addActor(Entity* e);
        void clearScene();
        void createFormations();
        void setFormation(CArmy::Formation formation, int p[23][15]);

        double getAngleToHit(double ox, double oy, double tx, double ty, double& v, double gravity = 60);
        double getAngleToHit2(double ox, double oy, double tx, double ty, double& v,  double gravity = 60);
        double getMinimumSpeedToHit(double ox, double oy, double tx, double ty,  double gravity = 60);

        void createProjectile(Entity* e, double x0, double y0, double x1, double y1, AShoot::Object obj);
        
        // SYSTEM ANIMATIONS
        void scriptMeleeBattle(Entity* e);
        void scriptPresentArmy(Entity* e);
        void scriptPresentCaptain(Entity* e);
        void scriptAdvanceArmy(Entity* e);
        void scriptReturnArmy(Entity* e);
        void scriptRemoveDead(Entity* e);
        void scriptBattleCleanup();
        void scriptThrowCoin(ActionOutcome& outcome, Entity* e);
        
        void askEndOfMatchQuestions(double delay);
        
        // BATTLE CLOSURES
        void scriptArmyVsArmy(ActionOutcome& outcome, Entity* e);
        void scriptEnslave(ActionOutcome& outcome, Entity* eActor);
        void scriptManVsMan(ActionOutcome& outcome, Entity* eActor);
        void scriptMercy(ActionOutcome& outcome, Entity* eActor);
        
        // UNIT ATTACKS
        void scriptPreAttackSpeech(ActionOutcome& outcome, Entity* e);
        void scriptPostAttackSpeech(ActionOutcome& outcome, Entity* e, double date = 0.f);
        void scriptMetheore(ActionOutcome& outcome, Entity* e);
        void scriptIceDragons(ActionOutcome& outcome, Entity* e);
        void scriptFlamingArrows(ActionOutcome& outcome, Entity* e);
        void scriptMakeItRain(ActionOutcome& outcome, Entity* e);
        void scriptThrowBomb(ActionOutcome& outcome, Entity* e);
        void scriptTelekinesis(ActionOutcome& outcome, Entity* e);
        void scriptBubbles(ActionOutcome& outcome, Entity* e);
        void scriptThrowScimitar(ActionOutcome& outcome, Entity* e);
        void scriptLongbow(ActionOutcome& outcome, Entity* e);
        void scriptEarthquake(ActionOutcome& outcome, Entity* e);
        void scriptStalactite(ActionOutcome& outcome, Entity* e);
        void scriptSonic(ActionOutcome& outcome, Entity* e);
        void scriptStrongWoman(ActionOutcome& outcome, Entity* e);
        void scriptNinja(ActionOutcome& outcome, Entity* e);
        void scriptSamurai(ActionOutcome& outcome, Entity* e);
        void scriptTornado(ActionOutcome& outcome, Entity* e);
        
        // HERO ABILITIES
        void scriptIntimidate(ActionOutcome& outcome, Entity* eActor);
        void scriptConfusion(ActionOutcome& outcome, Entity* e);
        void scriptSummon(ActionOutcome& outcome, Entity* e);
        void scriptChangeFormation(ActionOutcome& outcome, Entity* e);
        void scriptHex(ActionOutcome& outcome, Entity* e);
        void scriptTarot(ActionOutcome& outcome, Entity* e);
        void scriptPurify(ActionOutcome& outcome, Entity* e);
        void scriptResurect(ActionOutcome& outcome, Entity* e);
        void scriptCurse(ActionOutcome& outcome, Entity* e);
        void scriptSomniferous(ActionOutcome& outcome, Entity* e);
        void scriptSleep(ActionOutcome& outcome, Entity* e);
        void scriptTimeDilatation(ActionOutcome& outcome, Entity* e);
        void scriptSlavesCall(ActionOutcome& outcome, Entity* e);
        void scriptConversion(ActionOutcome& outcome, Entity* e);
        void scriptTrueForm(ActionOutcome& outcome, Entity* e);
        void scriptImmortality(ActionOutcome& outcome, Entity* e);
        void scriptDoom(ActionOutcome& outcome, Entity* e);
        void scriptTeleport(ActionOutcome& outcome, Entity* e);
        void scriptSwap(ActionOutcome& outcome, Entity* e);
        void scriptStampede(ActionOutcome& outcome, Entity* e);
        void scriptBecomeHuman(ActionOutcome& outcome, Entity* e);
        void scriptCroak(ActionOutcome& outcome, Entity* e);
        void scriptHelp(ActionOutcome& outcome, Entity* e);
        void scriptGrantFormationBuff(ActionOutcome& outcome, Entity* e);
        void scriptBackfire(ActionOutcome& outcome, Entity* e);
        void scriptDebuffFire(ActionOutcome& outcome, Entity* e);
        void scriptDebuffWater(ActionOutcome& outcome, Entity* e);
        void scriptDebuffEarth(ActionOutcome& outcome, Entity* e);
        void scriptDebuffAir(ActionOutcome& outcome, Entity* e);

        // SUB SCRIPTS
        void scriptFightSmokeCloud(double duration);
        void scriptVictoryAnimation(double delay);
        void scriptConfettiRain(double delay);
        void scriptPoofAt(double x, double y, double when);
        void scriptBlockIcon(double x, double y, double timing, bool hFlip, CUnit::DamageType type);
        void scriptDeathIcon(double x, double y, double timing, bool hFlip, CUnit::DamageType type);
        void scriptCurseIcon(double x, double y, double timing, bool hFlip);
        void scriptGeiser(double x, double y, double timing, double duration);
        void scriptPurificationFlask(double x, double y, double tStart);
        
        // HELPERS
        double getActingDuration();
        EntityList getActors(ActionOutcome& outcome, Entity* e);
        EntityList getTargets(ActionOutcome& outcome, Entity* e);
        int getDeathCount(ActionOutcome& outcome);
        sf::Vector2f getArmyMiddlePoint(Entity* e);
        Entity* getUnitByID(Entity* eArmy, int id);
        
        // DATA
        bool playingScene;
        EntityList actors;
        vector< list<sf::Vector2i> > positions;
        int wFormation;
        int hFormation;
        double uxFormation;
        double uyFormation;
        double x0Formation;
        double xOffCaptain;
        double wWalkStep;
        double totalWalk;
        double midSpace;
        double unitRest1;
        double unitRest2;
        double intimSpeechDur;
        double hurtDuration;
        bool mapContains(map<Entity*, double> m, Entity* e);
};

#endif // SCRIPTEDANIMATION_H
