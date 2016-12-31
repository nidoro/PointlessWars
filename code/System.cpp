#include "System.h"
vector< list<System*> > System::observers(N_MESSAGES);
War System::war;
Config System::config;
CProfile System::profile;
CUILayer::Layer System::topUILayer = CUILayer::NONE;
std::stack<CUILayer::Layer> System::uiLayers;
std::string System::appDataDir = helper::getAppDataDir();

System::System(){
    delay = 0.f;
    //ctor
}

System::~System(){
    //stop();
    //entities.clear();
}

bool System::hasRequirements(Entity* entity){
    std::list<Component::Type>::iterator it;
    for (it = requirements.begin(); it != requirements.end(); it++){
        if (!entity->has(*it)) return false;
    }
    return true;
}

void System::addEntity(Entity* entity){
    entities.push_back(entity);
}

void System::addRequirement(Component::Type req){
    requirements.push_back(req);
}

void System::start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS){
    this->eManager = eManager;
    this->window = window;
    this->targetUPS = targetUPS;
/*
    wWindow = window->getView().getSize().x;
    hWindow = window->getView().getSize().y;
    cxWindow = wWindow/2;
    cyWindow = hWindow/2;
*/
    wWindow = 1280;
    hWindow = 720;
    cxWindow = wWindow/2;
    cyWindow = hWindow/2;

    for(list<Message>::iterator i = subscripts.begin(); i != subscripts.end(); i++){
        subscribe(*i);
    }
}

void System::stop(){
    for(list<Message>::iterator i = subscripts.begin(); i != subscripts.end(); i++){
        unsubscribe(*i);
    }
}

void System::onNotify(Message m, Entity* e){
    switch(m){
        case GAME_STARTED: onGameStarted(e); break;
        case BUTTON_GAINED_FOCUS: onButtonGainedFocus(e); break;
        case BUTTON_LOST_FOCUS: onButtonLostFocus(e); break;
        case BUTTON_PRESSED: onButtonPressed(e); break;
        case BUTTON_RELEASED: onButtonReleased(e); break;
        case BUTTON_TRIGGERED: onButtonTriggered(e); break;
        case CREATE_SPIN_BUTTONS: onCreateSpinButtons(e); break;
        case CHANGE_SPIN_VALUE: onChangeSpinValue(e); break;
        case INCREASE_SPIN_BUTTON_VALUE: onIncreaseSpinButtonValue(e); break;
        case DECREASE_SPIN_BUTTON_VALUE: onDecreaseSpinButtonValue(e); break;
        case SELECT_OPTION: onSelectOptionBox(e); break;
        case EXPAND_OPTION_BOX: onExpandOptionBox(e); break;
        case RETRACT_OPTION_BOX: onRetractOptionBox(e); break;
        case START_FADE_OUT_TRANSITION: onStartFadeOutTransition(e); break;
        case START_FADE_IN_TRANSITION: onStartFadeInTransition(e); break;
        case FADE_ENDED: onFadeEnded(e); break;
        case BT_NEW_GAME: onBtNewGame(e); break;
        case INITIALIZE_WAR: onInitializeWar(e); break;
        case ASSIGN_RANDOM_ARMY: onAssignRandomArmy(e); break;
        case START_NEW_BATTLE: onStartNewBattle(e); break;
        case ACTION_SELECTED: onActionSelected(e); break;
        case EXECUTE_ACTIONS: onExecuteActions(e); break;
        case START_ACTION_TARGET_SELECTION: onStartActionTargetSelection(e); break;
        case TARGET_SELECTED: onTargetSelected(e); break;
        case FINISH_ACTION_TARGET_SELECTION: onFinishActionTargetSelection(e); break;
        case START_BATTLE_STATE: onStartBattleState(e); break;
        case END_BATTLE_STATE: onEndBattleState(e); break;
        case INITIALIZE_PLAYER: onInitializePlayer(e); break;
        case PLAY_ACTION: onPlayAction(e); break;
        case CREATE_ARMY: onCreateArmy(e); break;
        case RECOMPOSE_ARMY: onRecomposeArmy(e); break;
        case BEGIN_PANEL: onBeginPanel(e); break;
        case END_PANEL: onEndPanel(e); break;
        case ATTACH_ENTITY_TO_PANEL: onAttachEntityToPanel(e); break;
        case REMOVE_PANEL: onRemovePanel(e); break;
        case START_ANIMATION: onStartAnimation(e); break;
        case TRIGGER_OBJECT_ANIMATION: onTriggerObjectAnimation(e); break;
        case PLAY_SOUND: onPlaySound(e); break;
        case INITIALIZE_EDITOR: onInitializeEditor(e); break;
        case BT_EDITOR: onBtEditor(e); break;
        case INITIALIZE_SCROLL_LIST: onInitializeScrollList(e); break;
        case SCROLL: onScroll(e); break;
        case SET_PREVIEW_BACKGROUND: onSetPreviewBackground(e); break;
        case BT_EDITOR_LOAD: onBtEditorLoad(e); break;
        case BT_EDITOR_SAVE: onBtEditorSave(e); break;
        case BT_EDITOR_CLEAR: onBtEditorClear(e); break;
        case BT_EDITOR_TEST: onBtEditorTest(e); break;
        case ADD_OBJECT_TO_PREVIEW: onAddObjectToPreview(e); break;
        case DRAG: onDrag(e); break;
        case DROP: onDrop(e); break;
        case KEY_PRESSED: onKeyPressed(e); break;
        case KEY_RELEASED: onKeyReleased(e); break;
        case MOUSE_BUTTON_PRESSED: onMouseButtonPressed(e); break;
        case MOUSE_BUTTON_RELEASED: onMouseButtonReleased(e); break;
        case TEXT_ENTERED: onTextEntered(e); break;
        case ACTIVATE_INPUT_TEXT_BOX: onActivateInputTextBox(e); break;
        case DEACTIVATE_INPUT_TEXT_BOX: onDeactivateInputTextBox(e); break;
        case LOAD_SCENARIO_PREVIEW: onLoadScenarioPreview(e); break;
        case CLOSE_PANEL: onClosePanel(e); break;
        case SAVE_SCENARIO: onSaveScenario(e); break;
        case TEST_SCENARIO: onTestScenario(e); break;
        case LOAD_SCENARIO: onLoadScenario(e); break;
        case END_SCENARIO_TEST: onEndScenarioTest(e); break;
        case SCENE_STARTED: onSceneStarted(e); break;
        case SCENE_ENDED: onSceneEnded(e); break;
        case UNIT_EFFECTS_UPDATED: onUnitEffectsUpdated(e); break;
        case UPDATE_EFFECTS: onUpdateEffects(e); break;
        case HIGHLIGHT_UNITS: onHighlightUnits(e); break;
        case HIGHLIGHT_UNITS_OFF: onHighlightUnitsOff(e); break;
        case REMOVE_ENTITY: onRemoveEntity(e); break;
        case ANNOUNCE_VICTORY: onAnnounceVictory(e); break;
        case CREATE_SCREEN: onCreateScreen(e); break;
        case START_SCREEN_TRANSITION: onStartScreenTransition(e); break;
        case TAKE_CAPTAIN: onTakeCaptain(e); break;
        case END_CAPTAINS_POOL_PHASE: onEndCaptainsPoolPhase(e); break;
        case OPEN_MENU: onOpenMenu(e); break;
        case LOCK_ALL_BUTTONS: onLockAllButtons(e); break;
        case UNLOCK_ALL_BUTTONS: onUnlockAllButtons(e); break;
        case RESUME_GAME: onResumeGame(e); break;
        case NEW_SCREEN: onNewScreen(e); break;
        case SHAKE_CAMERA: onShakeCamera(e); break;
        case EXPAND_DROP_LIST: onExpandDropList(e); break;
        case COLLAPSE_DROP_LIST: onCollapseDropList(e); break;
        case UPDATE_DROP_LIST: onUpdateDropList(e); break;
        case TOGGLE_CHECK_BOX: onToggleCheckBox(e); break;
        case START_CUSTOM_MATCH: onStartCustomMatch(e); break;
        case CHANGE_MENU_PAGE: onChangeMenuPage(e); break;
        case LOAD_FORMATION: onLoadFormation(e); break;
        case SAVE_FORMATION: onSaveFormation(e); break;
        case BT_FORMATION_SAVE: onBtFormationSave(e); break;
        case CLEAR_FORMATION_EDITOR: onClearFormationEditor(e); break;
        case DELETE_FORMATION: onDeleteFormation(e); break;
        case RANDOMIZE_ARMY: onRandomizeArmy(e); break;
        case ACCEPT_ARMY: onAcceptArmy(e); break;
        case UPDATE_TOOLTIP: onUpdateTooltip(e); break;
        case CREATE_PLAYER_EDITOR: onCreatePlayerEditor(e); break;
        case CREATE_PROFILE: onCreateProfile(e); break;
        case SELECT_CHARACTER: onSelectCharacter(e); break;
        case SWITCH_CHAR_ANIMATION: onSwitchCharAnimation(e); break;
        case CREATE_PROFILE_MENU: onCreateProfileMenu(e); break;
        case EDIT_PROFILE: onEditProfile(e); break;
        case DELETE_PROFILE: onDeleteProfile(e); break;
        case SELECT_PROFILE: onSelectProfile(e); break;
        case REMOVE_SIBLINGS: onRemoveSiblings(e); break;
        case REMOVE_COUSINS: onRemoveCousins(e); break;
        case REMOVE_NEPHEWS: onRemoveNephews(e); break;
        case REMOVE_PARENT: onRemoveParent(e); break;
        case REMOVE_CHILDREN: onRemoveChildren(e); break;
        case START_SLIDING: onStartSliding(e); break;
        case SYSTEM_ACTION: onSystemAction(e); break;
        case SYSTEM_ACTION_COMPLETED: onSystemActionCompleted(e); break;
        case PICK_HERO: onPickHero(e); break;
        case SELECT_ACTION: onSelectAction(e); break;
        case SHOW_HERO_POOL: onShowHeroPool(e); break;
        case THROW_COIN: onThrowCoin(e); break;
        case TRY_AND_CONNECT: onTryAndConnect(e); break;
        case DISCONNECT_FROM_SERVER: onDisconnectFromServer(e); break;
        case FIND_MATCH: onFindMatch(e); break;
        case PACKET_RECEIVED: onPacketReceived(e); break;
        case SEND_PACKET: onSendPacket(e); break;
        case REMOVE_FROM_HERO_POOL: onRemoveFromHeroPool(e); break;
        case END_HERO_POOL: onEndHeroPool(e); break;
        case SCORE_UPDATED: onScoreUpdated(e); break;
        case PLAY_MUSIC: onPlayMusic(e); break;
        case STOP_MUSIC: onStopMusic(e); break;
        case ADD_ACTOR: onAddActor(e); break;
        case NEW_COMMAND_LINE: onNewCommandLine(e); break;
        case BRING_UI_LAYER_FORWARD: onBringUILayerForward(e); break;
        case WINDOW_CLOSED: onWindowClosed(e); break;
        case WINDOW_LOST_FOCUS: onWindowLostFocus(e); break;
        case WINDOW_GAINED_FOCUS: onWindowGainedFocus(e); break;
        case DO_TOGGLE_ACTION: onDoToggleAction(e); break;
        case HAS_BEEN_TOGGLED: onHasBeenToggled(e); break;
        case HAS_CHANGED_VALUE: onHasChangedValue(e); break;
        case CREATE_GUI_GROUP: onCreateGUIGroup(e); break;
        case CHANGE_WINDOW_PAGE: onChangeWindowPage(e); break;
        case REMOVE_GUI_GROUP: onRemoveGUIGroup(e); break;
        case UPDATE_RESOLUTION_WITH_DROP_LIST: onUpdateResolutionWithDropList(e); break;
        case UPDATE_FULLSCREEN_WITH_TOGGLE_BUTTON: onUpdateFullscreenWithToggleButton(e); break;
        case STOP_CONNECTION_TRY: onStopConnectionTry(e); break;
        case CONNECTION_SUCCESSFUL: onConnectionSuccessful(e); break;
        case CONNECTION_LOST: onConnectionLost(e); break;
        case UPDATE_PLAYER_NICKNAME_WITH_INPUT_TEXTBOX: onUpdatePlayerNicknameWithInputTextbox(e); break;
        case SET_SERVER_MESSAGE_DISPLAYER_QUICK_MATCH: onSetServerMessageDisplayerQuickMatch(e); break;
        case CANCEL_QUICK_MATCH_SEARCH: onCancelQuickMatchSearch(e); break;
        case END_MATCH: onEndMatch(e); break;
        case SET_MATCH_CONFIG: onSetMatchConfig(e); break;
        case CHOOSE_LANGUAGE: onChooseLanguage(e); break;
        default: break;
    }
}

bool System::checkAndUpdate(){
    /*
    if (targetUPS < 0){
        delay = updateClock.restart().asSeconds();
        update();
        return true;
    }else if (updateClock.getElapsedTime().asSeconds() >= 1.f/targetUPS){
        delay = updateClock.restart().asSeconds();
        update();
        return true;
    }
    return false;
     */
    delay += updateClock.restart().asSeconds();
    if (targetUPS < 0){
        delay = updateClock.restart().asSeconds();
        update();
        return true;
    }else if (delay >= 1.f/targetUPS){
        double aux = delay;
        delay = 1.f/targetUPS;
        update();
        delay = aux;
        delay -= 1.f/targetUPS;
        return true;
    }
    return false;
}

void System::updateEntities(){
    std::list<Entity*>::iterator it;
    for (it = eManager->modifiedEntities.begin(); it != eManager->modifiedEntities.end(); it++){
        entities.remove(*it);
        if (hasRequirements(*it)){
            entities.push_back(*it);
        }
    }
    for (it = eManager->addedEntities.begin(); it != eManager->addedEntities.end(); it++){
        if (hasRequirements(*it)){
            entities.push_back(*it);
        }
    }
    for (it = eManager->removedEntities.begin(); it != eManager->removedEntities.end(); it++){
        entities.remove(*it);
    }
}

void System::listenMessages(){

}

void System::shutdownAll(){
    for(auto& l : observers){
        l.clear();
    }
    observers.clear();
}

std::string System::getAppDataDir(){
    return appDataDir;
}




