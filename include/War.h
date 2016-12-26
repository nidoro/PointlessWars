#ifndef WAR_H
#define WAR_H

#include "Standard.h"
#include "Entity.h"
#include "MatchConfig.h"
#include "ActionOutcome.h"

class War{
    public:
        enum SystemAction{NONE, COIN,
                        ASK_HERO_PICK, ASK_FORMATION, PLAY_FORMATION, ADVANCE_ARMY,
                        ASK_CAPTAIN_ACTION, PLAY_ACTION, BETWEEN_ROUNDS,
                        ASK_ARMY_ACTION, ASK_BATTLE_CLOSURE, ASK_ARMY_ASSEMBLE,
                        ASK_CAPTAIN_SELECTION, PRESENT_ARMY, RECOMPOSE_ARMY,
                        SHOW_HERO_POOL, START_FIRST_BATTLE, START_BATTLE,
                        PRESENT_ARMIES, UPDATE_BATTLE_QUEUE, END_HERO_POOL,
                        PRESENT_HEROES, PLAY_ACTIONS, ADVANCE_ARMIES,
                        PLAY_INITIAL_INTIMIDATION, CLEAR_BATTLE_FIELD, START_NEW_BATTLE,
                        ANNOUNCE_VICTORY, ASSIGN_RANDOM_ARMY, SET_FORMATION_EFFECT,
                    FIELD_CLEANUP, BETWEEN_TURNS, ENDING, BEGINING, UPDATE_SCORE};

        enum BattleClosure{NO_CLOSURE_YET, ALL_KILLED, DRAW, ARMY_VS_ARMY, MAN_VS_MAN, CONFINE, MERCY};

        War();
        ~War();

        void shutdown();
        void setMatchConfig(string config);
        void setMatchConfig(MatchConfig& config);
        MatchConfig& getMatchConfig();
        void setPlayers(Entity* P1, Entity* P2);
        Entity* getPlayer1();
        Entity* getPlayer2();
        Entity* getPlayer(CPlayer::ID id);
        void setBattleWinner(Entity* e);
        Entity* getBattleWinner(Entity* e);
        CPlayer::ID getFirstMover();
        void setFirstMover(CPlayer::ID id);
        SystemAction getSystemAction();
        bool getPaused();
        void setPaused(bool p);
        bool getPlayingAnimation();
        void setPlayingAnimation(bool playing);
        CPlayer::ID getActorID();
        void setActorID(CPlayer::ID id);
        void setSystemAction(SystemAction s);
        Entity* getActor();
        ActionOutcome& getNextActionOutcome(CPlayer::ID id = 0);
        void setNextAction(CAction::ID id);
        void setNextAction(CPlayer::ID player, CAction::ID id);
        bool getPlayerReady(CPlayer::ID id);
        void setPlayerReady(CPlayer::ID id, bool b);
        bool getPlayersReady();
        bool getActionCompleted();
        bool getActionCompleted(CPlayer::ID id);
        bool getSystemReady();
        void setSystemReady(bool b);
        bool getPeerReady();
        void setPeerReady(bool b);
        void setP1ActionCompleted(bool b);
        void setP2ActionCompleted(bool b);
        void setSysActionCompleted(bool b);
        void setActionCompleted(CPlayer::ID id, bool c);
        bool getPeerReceivedState();
        void setPeerReceivedState(bool b);
        bool getMultiplayer();
        void setMultiplayer(bool b);
        void setRemotelyControled(CPlayer::ID id, bool b);
        bool getRemotelyControled(CPlayer::ID id);
        bool getActionReceived(CPlayer::ID id);
        void setActionReceived(CPlayer::ID id, bool b);
        bool getActionSent(CPlayer::ID id);
        void setActionSent(CPlayer::ID id, bool b);
        bool getPlaying();
        void setPlaying(bool b);
        void setBattleWinner(CPlayer::ID id);
        void setBattleLoser(CPlayer::ID id);
        CPlayer::ID getBattleWinner();
        CPlayer::ID getBattleLoser();
        void setBattleClosure(BattleClosure closure);
        BattleClosure getBattleClosure();
        void addPendingAction(CPlayer::ID id, ActionOutcome action);
        ActionOutcome pullPendingAction(CPlayer::ID id);
        bool hasPendingAction(CPlayer::ID id);

        Entity* getLocalPlayer();

    private:
        MatchConfig matchConfig;

        Entity* P1;
        Entity* P2;

        BattleClosure battleClosure;
        CPlayer::ID idWinner;
        CPlayer::ID idLoser;

        CPlayer::ID idFirstMover;

        Message waitingFor;

        bool isPlayingAnimation;

        bool isPaused;
        SystemAction sysActionOnUnpause;
        CPlayer::ID idActorOnUnpause;

        CPlayer::ID idActor;
        SystemAction sysAction;

        ActionOutcome nextOutcome;
        ActionOutcome P1nextOutcome;
        ActionOutcome P2nextOutcome;
        std::vector<std::list<ActionOutcome> > pendingActions;

        bool isP1ready;
        bool isP2ready;
        bool isSystemReady;
        bool isMultiplayer;
        bool didPeerReceiveState;

        bool stateRemotelyControled;
        bool P1remotelyControled;
        bool P2remotelyControled;

        bool P1actionReceived;
        bool P2actionReceived;
        bool sysActionReceived;

        bool P1actionSent;
        bool P2actionSent;
        bool sysActionSent;

        bool actionP1Completed;
        bool actionP2Completed;
        bool actionSysCompleted;

        bool playing;
};

#endif // WAR_H
