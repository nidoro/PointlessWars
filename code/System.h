#ifndef SYSTEM_H
#define SYSTEM_H
#include <stack>
#include "Entity.h"
#include "EntitiesManager.h"
#include "Standard.h"
#include "War.h"
#include "Config.h"
#include "OSAppDataDir.h"
class System{
    public:
        System();
        virtual ~System();

        static void notify(Message m, Entity* e = nullptr){
            for (list<System*>::iterator sys = observers[m].begin(); sys != observers[m].end(); sys++){
                (*sys)->onNotify(m, e);
            }
        }
        static void notify(Message m, std::string strMessage){
            Entity eTemp;
            eTemp.add(new CStringMessage(strMessage));
            for (list<System*>::iterator sys = observers[m].begin(); sys != observers[m].end(); sys++){
                (*sys)->onNotify(m, &eTemp);
            }
        }

        virtual void start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS);
        virtual void stop();

        static void shutdownAll();

        bool checkAndUpdate();
        void onNotify(Message m, Entity* e);
        virtual void update(){}
        virtual void listenMessages();

        bool hasRequirements(Entity* entity);
        void addEntity(Entity* entity);
        void addRequirement(Component::Type req);
        void addSubscription(Message m){
            subscripts.push_back(m);
        }
        virtual void updateEntities();

        ///SHARED BY SYSTEMS
        static CProfile profile;
        static War war;
        static Config config;
        static CUILayer::Layer topUILayer;
        static std::stack<CUILayer::Layer> uiLayers;
        static std::string getAppDataDir();
        
    protected:
        virtual void onGameStarted(Entity* e){}
        virtual void onButtonGainedFocus(Entity* e){}
        virtual void onButtonLostFocus(Entity* e){}
        virtual void onButtonPressed(Entity* e){}
        virtual void onButtonReleased(Entity* e){}
        virtual void onButtonTriggered(Entity* e){}
        virtual void onCreateSpinButtons(Entity* e){}
        virtual void onChangeSpinValue(Entity* e){}
        virtual void onIncreaseSpinButtonValue(Entity* e){}
        virtual void onDecreaseSpinButtonValue(Entity* e){}
        virtual void onSelectOptionBox(Entity* e){}
        virtual void onExpandOptionBox(Entity* e){}
        virtual void onRetractOptionBox(Entity* e){}
        virtual void onButtonNewGame(Entity* e){}
        virtual void onStartFadeOutTransition(Entity* e){}
        virtual void onStartFadeInTransition(Entity* e){}
        virtual void onFadeEnded(Entity* e){}
        virtual void onBtNewGame(Entity* e){}
        virtual void onInitializeWar(Entity* e){}
        virtual void onAssignRandomArmy(Entity* e){}
        virtual void onStartNewBattle(Entity* e){}
        virtual void onActionSelected(Entity* e){}
        virtual void onExecuteActions(Entity* e){}
        virtual void onStartActionTargetSelection(Entity* e){}
        virtual void onTargetSelected(Entity* e){}
        virtual void onFinishActionTargetSelection(Entity* e){}
        virtual void onInitializePlayer(Entity* e){}
        virtual void onInitializeCaptainButton(Entity* e){}
        virtual void onStartBattleState(Entity* e){}
        virtual void onEndBattleState(Entity* e){}
        virtual void onPlayAction(Entity* e){}
        virtual void onCreateArmy(Entity* e){}
        virtual void onRecomposeArmy(Entity* e){}
        virtual void onBeginPanel(Entity* e){}
        virtual void onEndPanel(Entity* e){}
        virtual void onAttachEntityToPanel(Entity* e){}
        virtual void onRemovePanel(Entity* e){}
        virtual void onStartAnimation(Entity* e){}
        virtual void onTriggerObjectAnimation(Entity* e){}
        virtual void onPlaySound(Entity* e){}
        virtual void onInitializeEditor(Entity* e){}
        virtual void onBtEditor(Entity* e){}
        virtual void onInitializeScrollList(Entity* e){}
        virtual void onScroll(Entity* e){}
        virtual void onSetPreviewBackground(Entity* e){}
        virtual void onBtEditorLoad(Entity* e){}
        virtual void onBtEditorSave(Entity* e){}
        virtual void onBtEditorClear(Entity* e){}
        virtual void onBtEditorTest(Entity* e){}
        virtual void onAddObjectToPreview(Entity* e){}
        virtual void onDrag(Entity* e){}
        virtual void onDrop(Entity* e){}
        virtual void onKeyPressed(Entity* e){}
        virtual void onKeyReleased(Entity* e){}
        virtual void onMouseButtonPressed(Entity* e){}
        virtual void onMouseButtonReleased(Entity* e){}
        virtual void onTextEntered(Entity* e){}
        virtual void onActivateInputTextBox(Entity* e){}
        virtual void onLoadScenarioPreview(Entity* e){}
        virtual void onClosePanel(Entity* e){}
        virtual void onSaveScenario(Entity* e){}
        virtual void onDeactivateInputTextBox(Entity* e){}
        virtual void onTestScenario(Entity* e){}
        virtual void onLoadScenario(Entity* e){}
        virtual void onEndScenarioTest(Entity* e){}
        virtual void onSceneStarted(Entity* e){}
        virtual void onSceneEnded(Entity* e){}
        virtual void onUnitEffectsUpdated(Entity* e){}
        virtual void onUpdateEffects(Entity* e){}
        virtual void onHighlightUnits(Entity* e){}
        virtual void onHighlightUnitsOff(Entity* e){}
        virtual void onRemoveEntity(Entity* e){}
        virtual void onAnnounceVictory(Entity* e){}
        virtual void onCreateScreen(Entity* e){}
        virtual void onStartScreenTransition(Entity* e){}
        virtual void onTakeCaptain(Entity* e){}
        virtual void onEndCaptainsPoolPhase(Entity* e){}
        virtual void onOpenMenu(Entity* e){}
        virtual void onLockAllButtons(Entity* e){}
        virtual void onUnlockAllButtons(Entity* e){}
        virtual void onResumeGame(Entity* e){}
        virtual void onNewScreen(Entity* e){}
        virtual void onShakeCamera(Entity* e){}
        virtual void onExpandDropList(Entity* e){}
        virtual void onCollapseDropList(Entity* e){}
        virtual void onUpdateDropList(Entity* e){}
        virtual void onToggleCheckBox(Entity* e){}
        virtual void onStartCustomMatch(Entity* e){}
        virtual void onChangeMenuPage(Entity* e){}
        virtual void onLoadFormation(Entity* e){}
        virtual void onSaveFormation(Entity* e){}
        virtual void onBtFormationSave(Entity* e){}
        virtual void onDeleteFormation(Entity* e){}
        virtual void onClearFormationEditor(Entity* e){}
        virtual void onRandomizeArmy(Entity* e){}
        virtual void onAcceptArmy(Entity* e){}
        virtual void onUpdateTooltip(Entity* e){}
        virtual void onCreatePlayerEditor(Entity* e){}
        virtual void onCreateProfile(Entity* e){}
        virtual void onSelectCharacter(Entity* e){}
        virtual void onSwitchCharAnimation(Entity* e){}
        virtual void onCreateProfileMenu(Entity* e){}
        virtual void onEditProfile(Entity* e){}
        virtual void onDeleteProfile(Entity* e){}
        virtual void onSelectProfile(Entity* e){}
        virtual void onRemoveSiblings(Entity* e){}
        virtual void onRemoveCousins(Entity* e){}
        virtual void onRemoveNephews(Entity* e){}
        virtual void onRemoveParent(Entity* e){}
        virtual void onRemoveChildren(Entity* e){}
        virtual void onStartSliding(Entity* e){}
        virtual void onSystemAction(Entity* e){}
        virtual void onSystemActionCompleted(Entity* e){}
        virtual void onPickHero(Entity* e){}
        virtual void onSelectAction(Entity* e){}
        virtual void onShowHeroPool(Entity* e){}
        virtual void onThrowCoin(Entity* e){}
        virtual void onTryAndConnect(Entity* e){}
        virtual void onDisconnectFromServer(Entity* e){}
        virtual void onFindMatch(Entity* e){}
        virtual void onPacketReceived(Entity* e){}
        virtual void onSendPacket(Entity* e){}
        virtual void onRemoveFromHeroPool(Entity* e){}
        virtual void onEndHeroPool(Entity* e){}
        virtual void onScoreUpdated(Entity* e){}
        virtual void onPlayMusic(Entity* e){}
        virtual void onStopMusic(Entity* e){}
        virtual void onAddActor(Entity* e){}
        virtual void onNewCommandLine(Entity* e){}
        virtual void onBringUILayerForward(Entity* e){}
        virtual void onWindowClosed(Entity* e){}
        virtual void onWindowLostFocus(Entity* e){}
        virtual void onWindowGainedFocus(Entity* e){}
        virtual void onDoToggleAction(Entity* e){}
        virtual void onHasBeenToggled(Entity* e){}
        virtual void onHasChangedValue(Entity* e){}
        virtual void onCreateGUIGroup(Entity* e){}
        virtual void onChangeWindowPage(Entity* e){}
        virtual void onRemoveGUIGroup(Entity* e){}
        virtual void onUpdateResolutionWithDropList(Entity* e){}
        virtual void onUpdateFullscreenWithToggleButton(Entity* e){}
        virtual void onStopConnectionTry(Entity* e){}
        virtual void onConnectionSuccessful(Entity* e){}
        virtual void onConnectionLost(Entity* e){}
        virtual void onUpdatePlayerNicknameWithInputTextbox(Entity* e){}
        virtual void onSetServerMessageDisplayerQuickMatch(Entity* e){}
        virtual void onCancelQuickMatchSearch(Entity* e){}
        virtual void onEndMatch(Entity* e){}
        virtual void onSetMatchConfig(Entity* e){}
        virtual void onChooseLanguage(Entity* e){}
        
        void subscribe(Message m){
            addObserver(this, m);
        }
        void unsubscribe(Message m){
            removeObserver(this, m);
        }
        static void addObserver(System* sys, Message m){
            observers[m].push_back(sys);
        }
        static void removeObserver(System* sys, Message m){
            observers[m].remove(sys);
        }

        double targetUPS;
        double delay;
        sf::Clock updateClock;

        double wWindow, hWindow;
        double cxWindow, cyWindow;

        std::list<Entity*> entities;
        std::list<Component::Type> requirements;

        EntitiesManager* eManager;
        sf::RenderWindow* window;

        list<Message> subscripts;
        
    private:
        static vector< list<System*> > observers;
        static std::string appDataDir;
};

#endif // SYSTEM_H
