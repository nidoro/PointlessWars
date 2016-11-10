#include "CommandListener.h"

CommandListener::CommandListener(){
    addSubscription(SHOW_CAPTAIN_OPTIONS);
    addSubscription(START_BATTLE_STATE);
    addSubscription(TAKE_CAPTAIN);
    addSubscription(ACCEPT_ARMY);
    addSubscription(RANDOMIZE_ARMY);
    addSubscription(SYSTEM_ACTION);
    addSubscription(PICK_HERO);
    addSubscription(SELECT_ACTION);
    addSubscription(SHOW_HERO_POOL);
    //addSubscription(REMOVE_FROM_HERO_POOL);
    addSubscription(END_HERO_POOL);
    addSubscription(SYSTEM_ACTION);

    addRequirement(Component::COMMAND_OPTION);
    eListener = nullptr;
    listening = true;
}

CommandListener::~CommandListener(){
    //dtor
}

void CommandListener::update(){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        if (e->has(Component::OPTION_BOX) && e->get<COptionBox>()->state == COptionBox::CHOSEN){
            CAction::ID id = (CAction::ID) e->get<COptionBox>()->selected;
            Entity* eArmy = e->get<CCommandOption>()->eArmy;
            eArmy->get<CArmy>()->nextAction = id;
            notify(ACTION_SELECTED);
            eManager->removeEntity(e);
        }else if (e->has(Component::ARMY_COMPOSITION) && e->get<CArmyComposition>()->composed){
            /*
            for(EntityListIt i = e->get<CArmyComposition>()->eOptions.begin(); i != e->get<CArmyComposition>()->eOptions.end(); i++){
                notify(REMOVE_PANEL, *i);
            }
            eManager->removeEntity(e);

            Entity* eArmy = eListener->get<COwner>()->e;
            war.getNextActionOutcome(eArmy->get<CPlayer>()->id).action = -1;
            if (war.getSystemAction() == war.ASK_HERO_PICK){
                Entity* eParent = e->get<CParentPanel>()->e;
                eManager->removeEntity(e);
                eParent->get<CPanel>()->objects.remove(e);
            }

            eManager->removeEntity(eListener);
            eListener = nullptr;
            notify(ACTION_SELECTED);
            */
        }else if (e->has(Component::ARMY_COMPOSITION) && !e->get<CArmyComposition>()->composed){
            /*
            if (checkArmySize(e)){
                e->get<CTooltip>()->text = "OK!";                e->get<CButtonTrigger>()->message = CREATE_ARMY;
            }else{
                Entity* eArmy = e->get<CCommandOption>()->eArmy;
                int maxPicks = eArmy->get<CPlayer>()->maxPicks;
                e->get<CTooltip>()->text = "You can only recruit " + int2str(maxPicks) + " units!";
                e->get<CButtonTrigger>()->message = EMPTY_MESSAGE;
            }
            */
        }
    }

    if (war.getSystemAction() == War::SHOW_HERO_POOL){
        //showHeroPool();
        //war.setActionCompleted(0, true);
        //listening = true;
    }else if (war.getSystemAction() == War::END_HERO_POOL){
        //endHeroPool();
    }

    Entity* eActor = war.getActor();
    if (!eActor) return;

    //for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        if (!listening){
            if (war.getSystemAction() == War::ASK_ARMY_ASSEMBLE && !war.getRemotelyControled(eActor->get<CPlayer>()->id)){
                //clearArmy(war.getPlayer1());
                //cout << war.getActorID() << endl;
                showUnitOptions(eActor);
                listening = true;
                /*
                clearArmy(war.getPlayer2());
                notify(RECOMPOSE_ARMY, war.getPlayer2());
                */
            }else if (war.getSystemAction() == War::ASK_CAPTAIN_SELECTION && !eActor->has(Component::AI) && !war.getRemotelyControled(eActor->get<CPlayer>()->id)){
                showCaptainOptions(eActor);
                listening = true;
            }else if (war.getSystemAction() == War::ASK_FORMATION && !eActor->has(Component::AI)&& !war.getRemotelyControled(eActor->get<CPlayer>()->id)){
                showFormationOptions(eActor);
                listening = true;
            }else if (war.getSystemAction() == War::ASK_CAPTAIN_ACTION && !eActor->has(Component::AI)&& !war.getRemotelyControled(eActor->get<CPlayer>()->id)){
                showCaptainActionOptions(eActor);
                listening = true;
            }else if (war.getSystemAction() == War::ASK_ARMY_ACTION && !eActor->has(Component::AI)&& !war.getRemotelyControled(eActor->get<CPlayer>()->id)){
                showArmyActionOptions(eActor);
                listening = true;
            }else if (war.getSystemAction() == War::ASK_BATTLE_CLOSURE && !eActor->has(Component::AI)&& !war.getRemotelyControled(eActor->get<CPlayer>()->id)){
                showBattleClosureOptions(eActor);
                listening = true;
            }
            /*else if (war.getSystemAction() == War::PRESENT_ARMY){
                presentArmy(eActor);
            }else if (war.getSystemAction() == War::ADVANCE_ARMY){
                advanceArmy(eActor);
            }else if (war.getSystemAction() == War::FIELD_CLEANUP){
                cleanupField(eActor);
            }else if (war.getSystemAction() == War::SHOW_HERO_POOL){
                //showHeroPool();
            }
            */else if (war.getSystemAction() == War::ASK_HERO_PICK){
                askHeroPick(eActor);
            }
        }
    //}
}

void CommandListener::onStartBattleState(Entity* e){
    if (BattleState::ASK_ARMY_ASSEMBLE == e->get<CBattle>()->state.id){
        clearArmy(e->get<CBattle>()->state.actors.front());
        showUnitOptions(e->get<CBattle>()->state.actors.front());
        clearArmy(e->get<CBattle>()->state.actors.front()->get<CPlayer>()->enemy);
        notify(RECOMPOSE_ARMY, e->get<CBattle>()->state.actors.front()->get<CPlayer>()->enemy);
    }

    for(EntityListIt i = e->get<CBattle>()->state.actors.begin(); i != e->get<CBattle>()->state.actors.end(); i++){
        Entity* actor = *i;
        BattleState::ID state = e->get<CBattle>()->state.id;
        if (actor->has(Component::AI) && (
            state == BattleState::ASK_CAPTAIN_SELECTION ||
            state == BattleState::ASK_FORMATION ||
            state == BattleState::ASK_CAPTAIN_ACTION ||
            state == BattleState::ASK_ARMY_ACTION ||
            state == BattleState::ASK_BATTLE_CLOSURE
            )){

            continue;
        }

        switch(e->get<CBattle>()->state.id){
            case BattleState::ASK_CAPTAIN_SELECTION: showCaptainOptions(actor); break;
            case BattleState::ASK_FORMATION: showFormationOptions(actor); break;
            case BattleState::ASK_CAPTAIN_ACTION: showCaptainActionOptions(actor); break;
            case BattleState::ASK_ARMY_ACTION: showArmyActionOptions(actor); break;
            case BattleState::ASK_BATTLE_CLOSURE: showBattleClosureOptions(actor); break;
            case BattleState::PLAY_MELEE_BATTLE: playMeleeBattle(actor); break;
            case BattleState::PRESENT_ARMY: presentArmy(actor); break;
            case BattleState::ADVANCE_ARMY: advanceArmy(actor); break;
            case BattleState::FIELD_CLEANUP: cleanupField(actor); break;
            default: break;
        }
    }
}

void CommandListener::onSystemAction(Entity* e){
    listening = false;
}

void CommandListener::askHeroPick(Entity* e){
    if (!eListener) return;
    EntityList eOptionList = eListener->getObservedEntity("HeroOptions")->getObservedEntities();
    //printf("options size = %d", eOptionList.size());
    if (!war.getRemotelyControled(e->get<CPlayer>()->id) && !war.getNextActionOutcome(e->get<CPlayer>()->id).ready){
        eListener->addObservedEntity("PlayerBeingListenedTo", e);
        for(auto& eOpt : eOptionList){
            eOpt->get<CButtonTrigger>()->setUniqueTrigger(SELECT_ACTION);
        }
    }else{
        eListener->addObservedEntity("PlayerBeingListenedTo", e);
        //printf("Clearing triggers..\n");
        for(auto& eOpt : eOptionList){
            eOpt->get<CButtonTrigger>()->msgs.clear();
        }
    }
}

void CommandListener::onShowHeroPool(Entity* e){
    eListener = eManager->createEntity();

    list<CCaptain::ID> options = war.getNextActionOutcome().heroes;
    double spacing = 70;
    double x = cxWindow - (options.size()-1)*spacing/2;
    double y = cyWindow - spacing;
    double tStart = 1.0;
    double tBetween = 0.2;
    int count = 0;

    Entity* eOptions = eManager->createEntity();

    for (list<CCaptain::ID>::iterator i = options.begin(); i != options.end(); i++){
        CCaptain cap = CCaptain::Map[*i];
        Entity* eOption = eManager->createEntity();
        eOption->add(new CPosition(x,y));
        eOption->add(new CTexture());
        //eOption->add(new CDimensions(60, 60));
        eOption->add(new CDefaultTexture(cap.portrait));
        eOption->add(new CHoverTexture(cap.portrait));
        eOption->add(new CActiveTexture(cap.portrait));
        eOption->add(new CDraw(CDraw::GUI));
        eOption->add(new CButtonState());
        eOption->add(new CButtonHitbox(60, 60));
        eOption->add(new CButtonTrigger(SELECT_ACTION));
        eOption->add(new CCaptain(cap));
        eOption->add(new CTooltip(CTooltip::CAPTAIN));
        eOption->add(new CAction(600 + *i));
        eOption->add(new CActor());

        animateButtonInPuff(eOption, tStart + count*tBetween);

        //eListener->attachEmployee(eOption);
        eOptions->addObservedEntity(int2str(cap.id), eOption);

        x += spacing;
        count++;
    }

    eListener->addObservedEntity("HeroOptions", eOptions);

    listening = true;
}

void CommandListener::showCaptainOptions(Entity* e){
    eListener = eManager->createEntity();

    int K = 100;
    int options = e->get<CArmy>()->captains.size();

    double w = 50;
    double h = 50;
    double cellSpacing = 60;
    double x = cxWindow;
    double y = cyWindow - (options-1)*cellSpacing/2;
    double tStart = 0.0;
    double tBetween = 0.2;
    int count = 0;

    Entity* eOptions = eManager->createEntity();

    for(auto& i : e->get<CArmy>()->captains){
        if (i.second->get<CCaptain>()->isConfined) continue;
        CCaptain::ID idCap = i.first;
        CAction::ID idAct = K+idCap;

        Entity* eBut = eManager->createEntity();
        eBut->add(new CButtonState());
        eBut->add(new CPosition(x, y));
        eBut->add(new CButtonTrigger(SELECT_ACTION));
        eBut->add(new CTexture(CCaptain::Map[idCap].btDefTexture));
        eBut->add(new CDefaultTexture(CCaptain::Map[idCap].btDefTexture));
        eBut->add(new CHoverTexture(CCaptain::Map[idCap].btHovTexture));
        eBut->add(new CActiveTexture(CCaptain::Map[idCap].btActTexture));
        eBut->add(new CDimensions(w, h));
        eBut->add(new CButtonHitbox(w, h));
        eBut->add(new CDraw(CDraw::GUI));
        eBut->add(new CTooltip(CTooltip::ACTION));
        eBut->add(new CAction(idAct));
        eBut->add(new CCaptain(idCap));
        eBut->add(new CActor());
        eListener->attachEmployee(eBut);

        animateButtonInPuff(eBut, tStart + count*tBetween);

        y += cellSpacing;
        count++;
        eOptions->addObservedEntity(int2str(count), eBut);
    }
    eListener->addObservedEntity("PlayerBeingListenedTo", e);
    eListener->addObservedEntity("Options", eOptions);

    listening = true;
}

void CommandListener::showFormationOptions(Entity* e){
    eListener = eManager->createEntity();
    Entity* eOptions = eManager->createEntity();

    Entity* eCap = e->get<CArmy>()->captain;

    int K = 300;
    int options = 3;
    if (contains(eCap->get<CCaptain>()->actions, 226)) options = 4;
    int nAvailableOptions = options;
    if (e->get<CArmy>()->prohibitedFormation != CArmy::N_FORMATIONS) nAvailableOptions--;

    double w = 50;
    double h = 50;
    double cellSpacing = 60;
    double x = cxWindow;
    double y = cyWindow - (nAvailableOptions-1)*cellSpacing/2;
    double tStart = 0.0;
    double tBetween = 0.2;
    int count = 0;

    for(int i = 0; i < options; i++){
        if (i == e->get<CArmy>()->prohibitedFormation) continue;
        CAction::ID idAct = K+i;
        Entity* eBut = eManager->createEntity();
        eBut->add(new CButtonState());
        eBut->add(new CPosition(x, y));
        eBut->add(new CButtonTrigger(SELECT_ACTION));
        eBut->add(new CTexture(CAction::Map[idAct].btDefTexture));
        eBut->add(new CDefaultTexture(CAction::Map[idAct].btDefTexture));
        eBut->add(new CHoverTexture(CAction::Map[idAct].btHovTexture));
        eBut->add(new CActiveTexture(CAction::Map[idAct].btActTexture));
        eBut->add(new CDimensions(w, h));
        eBut->add(new CButtonHitbox(w, h));
        eBut->add(new CDraw(CDraw::GUI));
        eBut->add(new CTooltip(CTooltip::ACTION));
        eBut->add(new CAction(idAct));
        eBut->add(new CActor());
        eListener->attachEmployee(eBut);

        animateButtonInPuff(eBut, tStart + count*tBetween);

        y += cellSpacing;
        count++;
        eOptions->addObservedEntity(int2str(count), eBut);
    }
    eListener->addObservedEntity("PlayerBeingListenedTo", e);
    eListener->addObservedEntity("Options", eOptions);

    listening = true;
}

void CommandListener::showCaptainActionOptions(Entity* e){
    eListener = eManager->createEntity();
    Entity* eOptions = eManager->createEntity();

    int K = 100;
    int options = e->get<CArmy>()->captain->get<CCaptain>()->actions.size();
    if (contains(e->get<CArmy>()->captain->get<CCaptain>()->actions, 226)) options--;

    double w = 40;
    double h = 40;
    double cellSpacing = 50;
    double x = cxWindow;
    double y = cyWindow - (options-1)*cellSpacing/2;
    double tStart = 0.0;
    double tBetween = 0.2;
    int count = 0;

    for(auto& i : e->get<CArmy>()->captain->get<CCaptain>()->actions){
        if (i == 226) continue;
        CAction::ID idAct = i;
        Entity* eBut = eManager->createEntity();
        eBut->add(new CButtonState());
        eBut->add(new CPosition(x, y));
        eBut->add(new CButtonTrigger(SELECT_ACTION));
        eBut->add(new CTexture(CAction::Map[idAct].btDefTexture));
        eBut->add(new CDefaultTexture(CAction::Map[idAct].btDefTexture));
        eBut->add(new CHoverTexture(CAction::Map[idAct].btHovTexture));
        eBut->add(new CActiveTexture(CAction::Map[idAct].btActTexture));
        eBut->add(new CDimensions(w, h));
        eBut->add(new CButtonHitbox(w, h));
        eBut->add(new CDraw(CDraw::GUI));
        eBut->add(new CTooltip(CTooltip::ACTION));
        eBut->add(new CAction(idAct));
        eBut->add(new CActor());
        eListener->attachEmployee(eBut);

        animateButtonInPuff(eBut, tStart + count*tBetween);

        y += cellSpacing;
        count++;
        eOptions->addObservedEntity(int2str(count), eBut);
    }
    eListener->addObservedEntity("PlayerBeingListenedTo", e);
    eListener->addObservedEntity("Options", eOptions);

    listening = true;
}

void CommandListener::showArmyActionOptions(Entity* e){
    eListener = eManager->createEntity();
    Entity* eOptions = eManager->createEntity();

    int K = 100;
    int options = e->get<CArmy>()->unitCount.size();

    //double w = Assets::getTexture(CUnit::Map.begin()->second.btActionDef)->getSize().x*1.5;
    //double h = Assets::getTexture(CUnit::Map.begin()->second.btActionDef)->getSize().y*1.5;
    double w = 40;
    double h = 40;
    double cellSpacing = 50;
    double x = cxWindow;
    double y = cyWindow - (options-1)*cellSpacing/2;
    double tStart = 0.0;
    double tBetween = 0.2;
    int count = 0;

    for(auto& i : e->get<CArmy>()->unitCount){
        if (i.second <= 0) continue;
        CUnit::ID idUnit = i.first;
        CAction::ID actID = CUnit::Map[i.first].action;
        if (actID == -1) continue;
        Entity* eBut = eManager->createEntity();
        eBut->add(new CButtonState());
        eBut->add(new CPosition(x, y));
        eBut->add(new CButtonTrigger(SELECT_ACTION));
        eBut->add(new CTexture(CUnit::Map[idUnit].btActionDef));
        eBut->add(new CDefaultTexture(CUnit::Map[idUnit].btActionDef));
        eBut->add(new CHoverTexture(CUnit::Map[idUnit].btActionHov));
        eBut->add(new CActiveTexture(CUnit::Map[idUnit].btActionAct));
        eBut->add(new CDimensions(w, h));
        eBut->add(new CButtonHitbox(w, h));
        eBut->add(new CDraw(CDraw::GUI));
        //eBut->add(new CTooltip(CTooltip::ACTION));
        eBut->add(new CAction(actID));
        eBut->add(new CUnit(CUnit::Map[i.first]));
        eBut->add(new CHighlightTrigger(CUnitHighlight2::ATTACK, i.first));
        eBut->add(new COwner(e));
        eBut->add(new CActor());
        eBut->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
        eBut->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;
        eListener->attachEmployee(eBut);

        animateButtonInPuff(eBut, tStart + count*tBetween);

        y += cellSpacing;
        count++;
        eOptions->addObservedEntity(int2str(count), eBut);
    }
    eListener->addObservedEntity("PlayerBeingListenedTo", e);
    eListener->addObservedEntity("Options", eOptions);

    listening = true;
}

void CommandListener::showBattleClosureOptions(Entity* e){
    eListener = eManager->createEntity();

    Entity* eCap = e->get<CArmy>()->captain;
    Entity* eOptions = eManager->createEntity();

    int K = 400;
    int options = 4;

    double w = 50;
    double h = 50;
    double cellSpacing = 60;
    double x = cxWindow;
    double y = cyWindow - (options-1)*cellSpacing/2;
    double tStart = 0.0;
    double tBetween = 0.2;
    int count = 0;

    for(int i = 0; i < options; i++){
        CAction::ID idAct = K+i;
        Entity* eBut = eManager->createEntity();
        eBut->add(new CButtonState());
        eBut->add(new CPosition(x, y));
        eBut->add(new CButtonTrigger(SELECT_ACTION));
        eBut->add(new CTexture(CAction::Map[idAct].btDefTexture));
        eBut->add(new CDefaultTexture(CAction::Map[idAct].btDefTexture));
        eBut->add(new CHoverTexture(CAction::Map[idAct].btHovTexture));
        eBut->add(new CActiveTexture(CAction::Map[idAct].btActTexture));
        eBut->add(new CDimensions(w, h));
        eBut->add(new CButtonHitbox(w, h));
        eBut->add(new CDraw(CDraw::GUI));
        eBut->add(new CTooltip(CTooltip::ACTION));
        eBut->add(new CAction(idAct));
        eBut->add(new CActor());
        eListener->attachEmployee(eBut);

        animateButtonInPuff(eBut, tStart + count*tBetween);

        y += cellSpacing;
        count++;
        eOptions->addObservedEntity(int2str(count), eBut);
    }
    eListener->addObservedEntity("PlayerBeingListenedTo", e);
    eListener->addObservedEntity("Options", eOptions);

    listening = true;
}

void CommandListener::playMeleeBattle(Entity* e){
      CAction::ID id = (CAction::ID) 400;
      e->get<CArmy>()->nextAction = id;
      notify(PLAY_ACTION, e);
}

void CommandListener::selectRandomUnits(list<CUnit::ID>& output, list<CUnit::ID>& input, int n){
    output.clear();
    list<CUnit::ID> deck = input;
    n = min(n, (int) deck.size());
    for(int i = 0; i < n; i++){
        output.push_back(popRandom(deck));
    }
}

bool ascendingUnitID(Entity* e1, Entity* e2){
    return e1->get<CUnit>()->id < e2->get<CUnit>()->id;
}

void CommandListener::showUnitOptions(Entity* e){
    if (!war.getMatchConfig().randomArmy){
        list<CUnit::ID> options;
        Entity* eOptions = eManager->createEntity();
        list<CUnit::ID> unitDeck;
        for (list<CUnit::ID>::iterator it = e->get<CPlayer>()->unitDeck.begin(); it != e->get<CPlayer>()->unitDeck.end(); it++){
            if (CUnit::Map[*it].damage != e->get<CArmy>()->prohibitedDamageType){
                //unitDeck.remove(*it);
                unitDeck.push_back(*it);
            }else{
                //it++;
            }
        }
        selectRandomUnits(options, unitDeck, war.getMatchConfig().nUnitOpt);

        for (list<CUnit::ID>::iterator i = options.begin(); i != options.end(); i++){
            CUnit::ID id = *i;
            Entity* eOption = eManager->createEntity();
            eOption->add(new CSpinButton(0, war.getMatchConfig().recruitGroup, 0, e->get<CPlayer>()->maxPicks));
            eOption->add(new CPosition());
            eOption->add(new CTexture("unit-counter.png"));
            eOption->add(new CDraw(CDraw::GUI2));
            eOption->add(new CButtonState());
            eOption->add(new CButtonHitbox(38, 120));
            eOption->add(new CButtonTrigger());
            eOption->add(new CDefaultTexture("unit-counter.png"));
            eOption->add(new CUnit(CUnit::Map[id]));
            eOption->add(new CTooltip(CTooltip::UNIT));
            eOption->add(new CActor());
            eOption->add(new CVelocity());
            eOption->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 12, sf::Color::Black, 0, -2));
            eOption->add(new CDisplayer(CDisplayer::SPIN_BUTTON, eOption));
            eOption->addObservedEntity("SpinButtonDisplayer", eOption);

            Entity* eIcon = eManager->createEntity();
            eIcon->add(new CPosition());
            eIcon->add(new CTexture(CUnit::Map[id].displayer));
            eIcon->add(new CDraw(CDraw::GUI3));
            eIcon->add(new CButtonState());
            eIcon->add(new CButtonHitbox());
            eIcon->add(new CButtonTrigger());
            eIcon->add(new CButtonTextures(CUnit::Map[id].displayer));
            eIcon->add(new CUnit(CUnit::Map[id]));
            eIcon->add(new CTooltip(CTooltip::UNIT));
            eIcon->add(new CActor());
            eIcon->add(new CVelocity());
            eOption->addObservedEntity("SpinButtonIcon", eIcon);

            eOption->attachEmployee(eIcon);
            eOptions->addObservedEntity(int2str(id), eOption);
        }
        EntityList eListOptions = eOptions->getObservedEntities();
        eListOptions.sort(ascendingUnitID);

        for (Entity* i : eListOptions){
            notify(CREATE_SPIN_BUTTONS, i);
        }

        double spacing = 60;
        double x = cxWindow - (eOptions->getObservedEntities().size()-1)*spacing/2;
        double y = -50;
        for(Entity* eOption : eListOptions){
            eOption->get<CPosition>()->x = x;
            eOption->get<CPosition>()->y = y;

            Entity* eIcon = eOption->getObservedEntity("SpinButtonIcon");
            eIcon->get<CPosition>()->x = x;
            eIcon->get<CPosition>()->y = y;

            Entity* eMore = eOption->getObservedEntity("SpinButtonIncrease");
            eMore->get<CPosition>()->x = x;
            eMore->get<CPosition>()->y = y;

            Entity* eLess = eOption->getObservedEntity("SpinButtonDecrease");
            eLess->get<CPosition>()->x = x;
            eLess->get<CPosition>()->y = y;

            animateUnitOptionIn(eOption);

            x += spacing;
        }
        //updatePosition(POptions);

        eListener = eManager->createEntity();
        Entity* btEnd = eManager->createEntity();
        x = cxWindow;
        y = cyWindow + 100;
        double w = 120;
        double h = 35;
        int act = 500;
        btEnd->add(new CPosition(x, y));
        btEnd->add(new CDimensions(w, h));
        btEnd->add(new CButtonHitbox(w, h));
        btEnd->add(new CButtonState(CButtonState::LOCKED));
        btEnd->add(new CButtonTrigger(SELECT_ACTION));
        btEnd->add(new CButtonTextures("9p-button-02.png", "9p-button-03.png", "9p-button-03.png"));
        btEnd->add(new CTexture("9p-button-02.png"));
        btEnd->add(new CDraw(CDraw::GUI2));
        btEnd->add(new CCommandOption(e));
        //btEnd->add(new CArmyComposition(eOptions));
        btEnd->add(new CAction(511));
        btEnd->add(new CTextbox2(Assets::getString("LABEL-READY"), Assets::getFont(Assets::getPrimaryFont()), 18, sf::Color::White));
        btEnd->add(new CTooltip(CTooltip::TEXT_1));
        btEnd->add(new CActor());

        ///ANIMATE READY BUTTON
        animateButtonInPuff(btEnd, 1.5, true);
        //btEnd->get<CActor>()->addNode(new AVariable(1.5, AVariable::BUT_LOCKED, false));
        //btEnd->get<CActor>()->addNode(new AVariable(0, AVariable::HIDDEN, false));

        eListener->addObservedEntity("UnitOptions", eOptions);
        eListener->addObservedEntity("AssemblyDone", btEnd);
        eListener->addObservedEntity("PlayerBeingListenedTo", e);
    }else{
        notify(ASSIGN_RANDOM_ARMY, e);

        e->get<CArmy>()->repicks = e->get<CPlayer>()->maxRepicks;

        Entity* btEnd = eManager->createEntity();
        double x = cxWindow;
        double y = 150;
        double w = 100;
        double h = 50;
        int act = 500;
        btEnd->add(new CPosition(x, y));
        btEnd->add(new CDimensions(w, h));
        btEnd->add(new CButtonHitbox(w, h));
        btEnd->add(new CButtonState());
        btEnd->add(new CButtonTrigger(ACCEPT_ARMY));
        btEnd->add(new CTexture("tooltip-02.png"));
        btEnd->add(new CDefaultTexture("tooltip-02.png"));
        btEnd->add(new CHoverTexture("tooltip-02.png"));
        btEnd->add(new CActiveTexture("tooltip-02.png"));
        btEnd->add(new CDraw(CDraw::GUI2));
        btEnd->add(new CTextbox2(Assets::getString("LABEL-READY"), Assets::getFont(Assets::getPrimaryFont()), 18, sf::Color::White));

        Entity* btRepick = eManager->createEntity();

        btRepick->add(new CPosition(x, y + 2*h));
        btRepick->add(new CDimensions(w, h));
        btRepick->add(new CButtonHitbox(w, h));
        btRepick->add(new CButtonState());
        btRepick->add(new CButtonTrigger(RANDOMIZE_ARMY));
        btRepick->add(new CTexture("tooltip-02.png"));
        btRepick->add(new CDefaultTexture("tooltip-02.png"));
        btRepick->add(new CHoverTexture("tooltip-02.png"));
        btRepick->add(new CActiveTexture("tooltip-02.png"));
        btEnd->add(new CDraw(CDraw::GUI2));
        btRepick->add(new CTextbox2("Repick!", Assets::getFont(Assets::getPrimaryFont()), 18, sf::Color::White));
        btRepick->add(new CTooltip(CTooltip::TEXT_1));
        btRepick->add(new COwner(e));
        btRepick->get<CTooltip>()->text = "Repicks remaining: " + int2str(e->get<CArmy>()->repicks);

        ///BTend DOESNT HAVE PANEL COMPONENT
    }
}

void CommandListener::clearArmy(Entity* e){
    e->get<CArmy>()->unitCount.clear();
    e->get<CArmy>()->allUnits.clear();
}

void CommandListener::reassembleArmies(Entity* e){

}

void CommandListener::presentArmy(Entity* e){
    e->get<CArmy>()->nextAction = 500;
    notify(PLAY_ACTION, e);
}

void CommandListener::advanceArmy(Entity* e){
    e->get<CArmy>()->nextAction = 502;
    notify(PLAY_ACTION, e);
}

void CommandListener::cleanupField(Entity* e){
    Entity* eWin = e;
    Entity* eLos = e->get<CPlayer>()->enemy;
    CAction::ID na = eWin->get<CArmy>()->nextAction;

    if (na != 402){
        eWin->get<CArmy>()->nextAction = 503;
        notify(PLAY_ACTION, eWin);
    }

    eWin->get<CArmy>()->nextAction = 504;
    notify(PLAY_ACTION, eWin);


    if (na != 402){
        eLos->get<CArmy>()->nextAction = 503;
        notify(PLAY_ACTION, eLos);
    }

    eLos->get<CArmy>()->nextAction = 504;
    notify(PLAY_ACTION, eLos);

    eWin->get<CArmy>()->allUnits.clear();
    eLos->get<CArmy>()->allUnits.clear();
}

bool CommandListener::checkArmySize(Entity* e){
    Entity* eArmy = e->get<CCommandOption>()->eArmy;
    int nUnits = 0;
    for(EntityListIt i = e->get<CArmyComposition>()->eOptions.begin(); i != e->get<CArmyComposition>()->eOptions.end(); i++){
        Entity* eOpt = *i;
        nUnits += eOpt->get<CSpinButton>()->value;
    }
    if (nUnits > eArmy->get<CPlayer>()->maxPicks){
        return false;
    }else{
        return true;
    }
}

void CommandListener::onAcceptArmy(Entity* e){
    eManager->removeEntity(e);
    notify(ACTION_SELECTED);
}

void CommandListener::onRandomizeArmy(Entity* e){
    Entity* eArmy = e->get<COwner>()->e;
    notify(ASSIGN_RANDOM_ARMY, eArmy);
    notify(UPDATE_TOOLTIP, e);
    eArmy->get<CArmy>()->repicks--;
    if (eArmy->get<CArmy>()->repicks <= 0){
        eManager->removeEntity(e);
    }else{
        e->get<CTooltip>()->text = "Repicks remaining: " + int2str(eArmy->get<CArmy>()->repicks);
        notify(UPDATE_TOOLTIP, e);
    }
}

void CommandListener::onPickHero(Entity* e){
}

void CommandListener::endHeroPool(){
    notify(REMOVE_PANEL, eListener);
    eListener = nullptr;
}

void CommandListener::onEndHeroPool(Entity* e){
    //printf("OK\n");
    bool first = true;
    EntityList eList = eListener->getObservedEntity("HeroOptions")->getObservedEntities();
    for(auto& i : eList){
        animateButtonOutPuff(i, 0.0, first ? true:false);
        first = false;
    }
    eListener = nullptr;
    //listening = false;
}

void CommandListener::onSelectAction(Entity* e){
    ///FIRST SAVE THE ACTION ID TO THE ACTION OUTCOME STRUCTURE
    Entity* eArmy = eListener->getObservedEntity("PlayerBeingListenedTo");
    war.getNextActionOutcome(eArmy->get<CPlayer>()->id).action = e->get<CAction>()->id;

    ///THEN REMOVE OPTIONS FROM SCREEN
    if (war.getSystemAction() == war.ASK_HERO_PICK){
        CCaptain::ID heroID = (int)e->get<CAction>()->id - 600;
        Entity* eButton = eListener->getObservedEntity("HeroOptions")->getObservedEntity(int2str((int) heroID));
        eListener->getObservedEntity("HeroOptions")->removeObservedEntity(eButton);
        animateButtonOutPuff(eButton);

    }else if (war.getSystemAction() == war.ASK_ARMY_ASSEMBLE){
        eArmy->get<CArmy>()->unitCount.clear();
        map<CUnit::ID, CUnit>& units = CUnit::Map;
        EntityList eOptions = eListener->getObservedEntity("UnitOptions")->getObservedEntities();

        bool first = true;
        Entity* btEnd = eListener->getObservedEntity("AssemblyDone");
        for (Entity* eOpt : eOptions){
            CUnit::ID id = eOpt->get<CUnit>()->id;
            if (eOpt->get<CSpinButton>()->value > 0){
                eArmy->get<CArmy>()->unitCount.insert(make_pair(id, eOpt->get<CSpinButton>()->value));
            }
            animateUnitOptionOut(eOpt, first);
            first = false;
        }
        animateButtonOutPuff(btEnd, 0.0, true);
        eListener = nullptr;

    }else if (war.getSystemAction() == war.ASK_CAPTAIN_SELECTION
        ||    war.getSystemAction() == war.ASK_FORMATION
        ||    war.getSystemAction() == war.ASK_ARMY_ACTION
        ||    war.getSystemAction() == war.ASK_CAPTAIN_ACTION
        ||    war.getSystemAction() == war.ASK_BATTLE_CLOSURE){

        EntityList eOptions = eListener->getObservedEntity("Options")->getObservedEntities();
        bool first = true;
        for(Entity* i : eOptions){
            if (first) animateButtonOutPuff(i, 0.0, true);
            else animateButtonOutPuff(i, 0.0, false);
            first = false;
        }
        eListener = nullptr;
    }
}

void CommandListener::onRemoveFromHeroPool(Entity* e){
    CCaptain::ID idHero = war.getNextActionOutcome(e->get<CPlayer>()->id).hero;
    Entity* eHero = nullptr;
    for(auto& i : eListener->get<CPanel>()->objects){
        eHero = i;
        if (eHero->get<CCaptain>()->id == idHero) break;
    }
    //printf("player %d, hero %d\n", e->get<CPlayer>()->id, idHero);
    if (eHero){
        eManager->removeEntity(eHero);
        eListener->get<CPanel>()->objects.remove(eHero);
    }
}

void CommandListener::animateButtonInPuff(Entity* e, double after, bool sound){
    e->get<CDraw>()->isHidden = true;
    e->get<CButtonState>()->state = CButtonState::LOCKED;
    e->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::HIDDEN, true));
    e->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::BUT_LOCKED, true));

    Entity* eSmoke = eManager->createEntity();
    string animation = "poof-03.png";
    double w = Assets::getAnimation(animation).wSprite;
    double h = Assets::getAnimation(animation).hSprite;
    if (e->has(Component::DIMENSIONS)){
        w = max(w, e->get<CDimensions>()->width);
        h = max(h, e->get<CDimensions>()->height);
    }
    eSmoke->add(new CAnimation(false, animation));
    eSmoke->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y));
    eSmoke->add(new CDraw(CDraw::GUI5));
    eSmoke->add(new CDimensions(w, h));
    eSmoke->add(new CActor());

    eSmoke->get<CDraw>()->isHidden = true;
    eSmoke->get<CActor>()->timeline.push_back(new AVariable(after, AVariable::HIDDEN, false));
    eSmoke->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
    if (sound) eSmoke->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-poof-01.wav"));
    eSmoke->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));

    e->get<CActor>()->timeline.push_back(new AVariable(after+Assets::getAnimation(animation).rate, AVariable::HIDDEN, false));
    e->get<CActor>()->timeline.push_back(new AVariable(Assets::getAnimation(animation).duration, AVariable::BUT_LOCKED, false));
}
void CommandListener::animateButtonOutPuff(Entity* e, double after, bool sound){
    notify(BUTTON_LOST_FOCUS, e);
    e->get<CDraw>()->isHidden = true;
    e->get<CButtonState>()->state = CButtonState::LOCKED;
    e->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::HIDDEN, true));
    e->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::BUT_LOCKED, true));

    Entity* eSmoke = eManager->createEntity();
    string animation = "poof-03.png";
    double w = Assets::getAnimation(animation).wSprite;
    double h = Assets::getAnimation(animation).hSprite;
    if (e->has(Component::DIMENSIONS)){
        w = max(w, e->get<CDimensions>()->width);
        h = max(h, e->get<CDimensions>()->height);
    }
    eSmoke->add(new CAnimation(false, animation));
    eSmoke->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y));
    eSmoke->add(new CDraw(CDraw::GUI5));
    eSmoke->add(new CDimensions(w, h));
    eSmoke->add(new CActor());

    eSmoke->get<CDraw>()->isHidden = true;
    eSmoke->get<CActor>()->timeline.push_back(new AVariable(after, AVariable::HIDDEN, false));
    eSmoke->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
    if (sound) eSmoke->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-poof-03.wav"));
    eSmoke->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));

    e->get<CActor>()->timeline.push_back(new ADestroy(after+Assets::getAnimation(animation).duration));
}

void CommandListener::animateUnitOptionIn(Entity* e){
    double yTarget = cyWindow - 60;
    double spacing = 30;
    double spacing2 = 106;
    double speed = 250;
    double speed2 = 40;
    double speed3 = 300;
    double tAux;

    double xCurrent = e->get<CPosition>()->x;
    double yCurrent = e->get<CPosition>()->y;

    Entity* eIcon = e->getObservedEntity("SpinButtonIcon");
    Entity* eMore = e->getObservedEntity("SpinButtonIncrease");
    Entity* eLess = e->getObservedEntity("SpinButtonDecrease");

    eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed));
    eIcon->get<CActor>()->addNode(new AVariable(0.0, AVariable::BUT_LOCKED, true));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed);
    eIcon->get<CActor>()->timeline.push_back(new AMove(tAux, xCurrent, yTarget - spacing/2, speed2));
    tAux = getTravelTime(xCurrent, yTarget, xCurrent, yTarget - spacing/2, speed2);
    eIcon->get<CActor>()->addNode(new AVariable(tAux, AVariable::BUT_LOCKED, false));

    e->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed));
    e->get<CActor>()->addNode(new AVariable(0.0, AVariable::BUT_LOCKED, true));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed);
    e->get<CActor>()->timeline.push_back(new AMove(tAux, xCurrent, yTarget + spacing/2, speed2));
    tAux = getTravelTime(xCurrent, yTarget, xCurrent, yTarget + spacing/2, speed2);
    e->get<CActor>()->addNode(new AVariable(tAux, AVariable::BUT_LOCKED, false));

    eMore->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed));
    eMore->get<CActor>()->addNode(new AVariable(0.0, AVariable::BUT_LOCKED, true));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed);
    eMore->get<CActor>()->timeline.push_back(new AMove(tAux, xCurrent, yTarget - spacing2/2, speed3));
    tAux = getTravelTime(xCurrent, yTarget, xCurrent, yTarget - spacing2/2, speed3);
    eMore->get<CActor>()->addNode(new AVariable(tAux, AVariable::BUT_LOCKED, false));

    eLess->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed));
    eLess->get<CActor>()->addNode(new AVariable(0.0, AVariable::BUT_LOCKED, true));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed);
    eLess->get<CActor>()->timeline.push_back(new AMove(tAux, xCurrent, yTarget + spacing2/2, speed3));
    tAux = getTravelTime(xCurrent, yTarget, xCurrent, yTarget + spacing2/2, speed3);
    eLess->get<CActor>()->addNode(new AVariable(tAux, AVariable::BUT_LOCKED, false));
}
void CommandListener::animateUnitOptionOut(Entity* e, bool sound){
    double yTarget = cyWindow - 60;
    double spacing = 30;
    double spacing2 = 106;
    double speed = 250;
    double speed2 = 40;
    double speed3 = 300;
    double tAux;
    double after;
    double littleDelay = 1;

    double xCurrent = e->get<CPosition>()->x;
    double yCurrent = e->get<CPosition>()->y;

    Entity* eIcon = e->getObservedEntity("SpinButtonIcon");
    Entity* eMore = e->getObservedEntity("SpinButtonIncrease");
    Entity* eLess = e->getObservedEntity("SpinButtonDecrease");
    e->get<CButtonState>()->state = CButtonState::LOCKED;
    eIcon->get<CButtonState>()->state = CButtonState::LOCKED;
    eMore->get<CButtonState>()->state = CButtonState::LOCKED;
    eLess->get<CButtonState>()->state = CButtonState::LOCKED;

    eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed2));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed2);
    after = tAux;
    eIcon->get<CActor>()->timeline.push_back(new ADestroy(tAux+littleDelay));

    e->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed2));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed2);
    e->get<CActor>()->timeline.push_back(new ADestroy(tAux+littleDelay));

    eMore->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed3));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed3);
    eMore->get<CActor>()->timeline.push_back(new ADestroy(tAux));

    eLess->get<CActor>()->timeline.push_back(new AMove(0.0, xCurrent, yTarget, speed3));
    tAux = getTravelTime(xCurrent, yCurrent, xCurrent, yTarget, speed3);
    eLess->get<CActor>()->timeline.push_back(new ADestroy(tAux));

    Entity* eSmoke = eManager->createEntity();
    string animation = "poof-03.png";
    eSmoke->add(new CAnimation(false, animation));
    eSmoke->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y));
    eSmoke->add(new CDraw(CDraw::GUI5));
    eSmoke->add(new CActor());

    eSmoke->get<CDraw>()->isHidden = true;
    eSmoke->get<CActor>()->timeline.push_back(new AVariable(after+littleDelay, AVariable::HIDDEN, false));
    eSmoke->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
    if (sound) eSmoke->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-poof-03.wav"));
    eSmoke->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));

    notify(ADD_ACTOR, eSmoke);
}













