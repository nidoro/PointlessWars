#include "ScriptedAnimation.h"

ScriptedAnimation::ScriptedAnimation() {
    addSubscription(TRIGGER_OBJECT_ANIMATION);
    addSubscription(INITIALIZE_WAR);
    addSubscription(GAME_STARTED);
    addSubscription(SYSTEM_ACTION);
    addSubscription(SCENE_STARTED);
    addSubscription(PLAY_ACTION);
    addSubscription(ADD_ACTOR);
    addSubscription(NEW_SCREEN);

    addRequirement(Component::ACTOR);

    playingScene = false;
    positions.resize(CArmy::N_FORMATIONS);
    unitRest1 = 0.5;
    unitRest2 = 2;
    intimSpeechDur = 2;
    hurtDuration = 0.1;
}

void ScriptedAnimation::onTriggerObjectAnimation(Entity* e) {
    if (e->get<CActor>()->acting) return;
    string a1 = e->get<CScenarioObject>()->aClick;
    string a2 = e->get<CScenarioObject>()->aDefault;
    double a1Duration = Assets::getAnimation(a1).duration;
    e->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, a1, e->get<CScenarioObject>()->clickSound));
    e->get<CActor>()->timeline.push_back(new ASpriteAnimation(a1Duration, a2));
}

void ScriptedAnimation::update() {
    for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
        Entity* e = *i;
        if (eManager->isDead(e)) {
            e->get<CActor>()->acting = false;
            continue;
        }
        if (!e->get<CActor>()->timeline.empty()) {
            if (e->get<CActor>()->acting == false) {
                e->get<CActor>()->acting = true;
                e->get<CActor>()->timer.restart();
            }
            while (e->get<CActor>()->timer.getElapsedTime().asSeconds() >= e->get<CActor>()->timeline.front()->triggerTime) {
                triggerNode(e, e->get<CActor>()->timeline.front());
                popFrontAnimationNode(e);
                if (e->get<CActor>()->timeline.empty()) {
                    e->get<CActor>()->acting = false;
                    break;
                }
                e->get<CActor>()->timer.restart();
            }
        }
    }

    bool acting = false;
    EntityList act = actors;
    for(EntityListIt i = act.begin(); i != act.end(); ) {
        Entity* e = *i;
        if (!eManager->isDead(e) && e->get<CActor>()->acting) {
            acting = true;
            break;
        }
        if (eManager->isDead(e)) {
            actors.remove(e);
        } else {
            i++;
        }
    }

    if (playingScene && !acting) {
        clearScene();
        playingScene = false;
        notify(SCENE_ENDED);
    }
    war.setPlayingAnimation(playingScene);

}

void ScriptedAnimation::triggerNode(Entity* e, AnimationNode* node) {
    if (node->type == AnimationNode::A_NOTHING) {

    } else if (node->type == AnimationNode::A_SPRITE_ANIMATION) {
        ASpriteAnimation* nd = static_cast<ASpriteAnimation*>(node);
        e->get<CAnimation>()->current = nd->animation;
        e->get<CAnimation>()->update = true;

        if (!nd->sound.empty()) {
            Entity* eSound = eManager->createEntity();
            eSound->add(new CSound(nd->sound, CSound::REMOVE_ENTITY));
            notify(PLAY_SOUND, eSound);
        }
    } else if (node->type == AnimationNode::A_MOVE) {
        AMove* nd = static_cast<AMove*>(node);
        e->add(new CPath());
        e->get<CPath>()->addNode(nd->x, nd->y);
        e->get<CVelocity>()->speed = nd->speed;
        eManager->addModified(e);
    } else if (node->type == AnimationNode::A_ZOOM) {
        AZoom* nd = static_cast<AZoom*>(node);
        e->add(new CZoom(nd->dx, nd->dy, nd->wTarget, nd->hTarget));
        eManager->addModified(e);
    } else if (node->type == AnimationNode::A_FADE) {
        AFade* nd = static_cast<AFade*>(node);
        if (!e->has(Component::FADE)) {
            e->add(new CFade(nd->speed, nd->alphaTarget));
            eManager->addModified(e);
        } else {
            e->get<CFade>()->alphaSpeed = nd->speed;
            e->get<CFade>()->alphaTarget = nd->alphaTarget;
        }
    } else if (node->type == AnimationNode::A_DESTROY) {
        if (e->has(Component::PARENT_PANEL)) {
            //Entity* eParent = e->get<CParentPanel>()->e;
            //eParent->get<CPanel>()->objects.remove(e);
        }
        eManager->removeEntity(e);
        if (contains(actors, e)) actors.remove(e);
    } else if (node->type == AnimationNode::A_ADD_COMPONENT) {
        AAddComponent* nd = static_cast<AAddComponent*>(node);
        e->add(nd->c, nd->cType);
        eManager->addModified(e);
    } else if (node->type == AnimationNode::A_REMOVE_COMPONENT) {
        ARemoveComponent* nd = static_cast<ARemoveComponent*>(node);
        e->remove(nd->c);
        eManager->addModified(e);
    } else if (node->type == AnimationNode::A_TELEPORT) {
        ATeleport* nd = static_cast<ATeleport*>(node);
        e->get<CPosition>()->x = nd->x;
        e->get<CPosition>()->y = nd->y;
    } else if (node->type == AnimationNode::A_SPEAK) {
        ASpeak* nd = static_cast<ASpeak*>(node);
        Entity* eCallout = eManager->createEntity();
        double yOff = e->has(Component::DIMENSIONS) ? -(e->get<CDimensions>()->height/2 + 10) : -35;
        CTextbox2 boxAux(nd->text, Assets::getFont(Assets::getPrimaryFont()), 11, sf::Color::Black);
        eCallout->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 11, sf::Color::Black));
        eCallout->add(new CRectShape(boxAux.content.getLocalBounds().width + 5, 20, sf::Color::White, 1));
        eCallout->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y + yOff));
        eCallout->add(new CDraw(CDraw::WORLD_3));
        eCallout->add(new CTimer(nd->duration, REMOVE_ENTITY));
        eCallout->add(new CAnchor(0, yOff));
        eCallout->add(new CTypingEffect(nd->text, 40));
        eCallout->addObservedEntity("Anchor", e);
        eCallout->attachEmployer(e);

    } else if (node->type == AnimationNode::A_VARIABLE) {
        AVariable* nd = static_cast<AVariable*>(node);
        switch(nd->var) {
        case AVariable::DEAD:
            e->get<CUnit>()->dead = nd->bValue;
            break;
        case AVariable::ALPHA:
            e->get<CDraw>()->alpha = nd->iValue;
            break;
        case AVariable::AUTO_P:
            if (nd->bValue) {
                e->add(new CAutoPilot);
            } else {
                e->remove(Component::AUTO_PILOT);
            }
            eManager->addModified(e);
            break;
        case AVariable::DRAW_TAG:
            e->get<CDraw>()->tag = (CDraw::Tag)(nd->iValue);
            break;
        case AVariable::ANGLE:
            e->get<CRotation>()->angle = nd->dValue;
            break;
        case AVariable::ROTATION_VEL:
            e->get<CRotation>()->velocity = nd->dValue;
            break;
        case AVariable::X_VEL:
            e->get<CVelocity>()->x = nd->dValue;
            break;
        case AVariable::Y_VEL:
            e->get<CVelocity>()->y = nd->dValue;
            break;
        case AVariable::X_ACC:
            e->get<CAcceleration>()->x = nd->dValue;
            break;
        case AVariable::Y_ACC:
            e->get<CAcceleration>()->y = nd->dValue;
            break;
        case AVariable::H_FLIP:
            e->get<CAnimation>()->hFlip = nd->bValue;
            break;
        case AVariable::V_FLIP:
            e->get<CAnimation>()->vFlip = nd->bValue;
            break;
        case AVariable::FREEZE:
            e->get<CAnimation>()->freeze = nd->bValue;
            break;
        case AVariable::ADD_EFFECT:
            if (e->has(Component::UNIT)) e->get<CUnit>()->effects.insert(make_pair(nd->iValue, CAction::Map[nd->iValue]));
            else e->get<COwner>()->e->get<CArmy>()->armyEffects.insert(make_pair(nd->iValue, CAction::Map[nd->iValue]));
            break;
        case AVariable::REMOVE_EFFECT:
            if (e->has(Component::UNIT)) e->get<CUnit>()->effects.erase(e->get<CUnit>()->effects.find(nd->iValue));
            else e->get<COwner>()->e->get<CArmy>()->armyEffects.erase(e->get<COwner>()->e->get<CArmy>()->armyEffects.find(nd->iValue));
            break;
        //case AVariable::WIN: e->get<COwner>()->e->get<CArmy>()->win = nd->bValue; break;
        case AVariable::REPLACE_HERO:
            e->get<COwner>()->e->get<CArmy>()->captain = e->getObservedEntity("ReplaceHero");
            break;
        case AVariable::REMOVE_FROM_ARMY:
            e->get<COwner>()->e->get<CArmy>()->allUnits.remove(e);
            break;
        case AVariable::HIDDEN:
            e->get<CDraw>()->isHidden = nd->bValue;
            break;
        case AVariable::COIN:
            e->get<COwner>()->e->get<CArmy>()->hasCoin = nd->bValue;
            break;
        case AVariable::HERO_CONFINED:
            e->get<CCaptain>()->isConfined = nd->bValue;
            break;
        case AVariable::BUT_LOCKED:
            e->get<CButtonState>()->state = nd->bValue ? CButtonState::LOCKED : CButtonState::NON_ACTIVE;
            break;
        case AVariable::PARTICLE_EFFECT:
            for(EntityListIt i = e->get<CParticleEffect>()->emmiters.begin(); i != e->get<CParticleEffect>()->emmiters.end(); i++) {
                Entity* eEmmiter = *i;
                eEmmiter->get<CParticleEmmiter>()->on = nd->bValue;
                eEmmiter->get<CParticleEmmiter>()->clock.restart();
            }
            break;
        default:
            break;
        }
        if (e->has(Component::UNIT) && e->get<CUnit>()->dead) {
            e->get<CActor>()->timeline.clear();
        }

    } else if (node->type == AnimationNode::A_SHOOT) {
        AShoot* nd = static_cast<AShoot*>(node);
        createProjectile(e, e->get<CPosition>()->x, e->get<CPosition>()->y, nd->xTarget, nd->yTarget, nd->object);
    } else if (node->type == AnimationNode::A_SOUND) {
        ASound* nd = static_cast<ASound*>(node);
        Entity* eSound = eManager->createEntity();
        eSound->add(new CSound(nd->name, CSound::REMOVE_ENTITY));
        notify(PLAY_SOUND, eSound);
    }
}


void ScriptedAnimation::onGameStarted(Entity* e) {
    createFormations();
}

void ScriptedAnimation::onInitializeWar(Entity* e) {
    wFormation = 15;
    hFormation = 23;
    uxFormation = 16;
    uyFormation = 13;
    xOffCaptain = 50;
    x0Formation = wFormation*uxFormation;
    midSpace = 130;
    totalWalk = cxWindow - midSpace/2 - x0Formation - xOffCaptain;
    wWalkStep = totalWalk/war.getMatchConfig().nTurns;

    ///TEST
    //scriptVictoryAnimation(1.0);
}

void ScriptedAnimation::onSceneStarted(Entity* e) {
    playingScene = true;
    war.setPlayingAnimation(playingScene);
}

void ScriptedAnimation::onSystemAction(Entity* e) {
    if (war.getSystemAction() == war.PRESENT_HEROES) {
        scriptPresentCaptain(war.getPlayer1());
        scriptPresentCaptain(war.getPlayer2());
        notify(SCENE_STARTED, e);

    } else if (war.getSystemAction() == war.ADVANCE_ARMIES) {
        scriptAdvanceArmy(war.getPlayer1());
        scriptAdvanceArmy(war.getPlayer2());
        notify(SCENE_STARTED, e);

    } else if (war.getSystemAction() == war.PRESENT_ARMIES) {
        scriptPresentArmy(war.getPlayer1());
        scriptPresentArmy(war.getPlayer2());
        notify(SCENE_STARTED, e);
    } else if (war.getSystemAction() == war.SET_FORMATION_EFFECT) {
        scriptGrantFormationBuff(war.getNextActionOutcome(war.getActorID()), war.getActor());
    }

}

void ScriptedAnimation::scriptBackfire(ActionOutcome& outcome, Entity* e) {
    for (auto i : outcome.idBackfire) {
        Entity* eUnit = getUnitByID(e, i);
        float when = unitRest1;
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eUnit->get<CUnit>()->aDeath));
        eUnit->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eUnit->get<CUnit>()->aDeath).duration, eUnit->get<CUnit>()->aDead));
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, when, eUnit->get<CAnimation>()->hFlip, outcome.dmgType);
    }
}

void ScriptedAnimation::onPlayAction(Entity* e) {
    CPlayer::ID idPlayer;
    if (!e) {
        idPlayer = 0;
    } else {
        idPlayer = e->get<CPlayer>()->id;
    }
    if (war.getNextActionOutcome(idPlayer).action < 100) {
        scriptPreAttackSpeech(war.getNextActionOutcome(idPlayer), e);
        scriptBackfire(war.getNextActionOutcome(idPlayer), e);
    }

    switch(war.getNextActionOutcome(idPlayer).action) {

    case 0:
        scriptFlamingArrows(war.getNextActionOutcome(idPlayer), e);
        break;
    case 1:
        scriptThrowBomb(war.getNextActionOutcome(idPlayer), e);
        break;
    case 2:
        scriptThrowScimitar(war.getNextActionOutcome(idPlayer), e);
        break;
    case 3:
        scriptMetheore(war.getNextActionOutcome(idPlayer), e);
        break;
    case 4:
        scriptEarthquake(war.getNextActionOutcome(idPlayer), e);
        break;
    case 5:
        scriptSonic(war.getNextActionOutcome(idPlayer), e);
        break;
    case 6:
        scriptStalactite(war.getNextActionOutcome(idPlayer), e);
        break;
    case 7:
        scriptStrongWoman(war.getNextActionOutcome(idPlayer), e);
        break;
    case 8:
        scriptBubbles(war.getNextActionOutcome(idPlayer), e);
        break;
    case 9:
        scriptSamurai(war.getNextActionOutcome(idPlayer), e);
        break;
    case 10:
        scriptMakeItRain(war.getNextActionOutcome(idPlayer), e);
        break;
    case 11:
        scriptIceDragons(war.getNextActionOutcome(idPlayer), e);
        break;
    case 12:
        scriptLongbow(war.getNextActionOutcome(idPlayer), e);
        break;
    case 13:
        scriptTornado(war.getNextActionOutcome(idPlayer), e);
        break;
    case 14:
        scriptNinja(war.getNextActionOutcome(idPlayer), e);
        break;
    case 15:
        scriptTelekinesis(war.getNextActionOutcome(idPlayer), e);
        break;
    case 16:
        //scriptGenericUnitAttack(war.getNextActionOutcome(idPlayer), e);
        break;
    case 200:
        //scriptEffectAction(e);
        break;
    case 201:
        //scriptFocusFire(e);
        break;
    case 202:
        scriptIntimidate(war.getNextActionOutcome(idPlayer), e);
        break;
    case 203:
        scriptResurect(war.getNextActionOutcome(idPlayer), e);
        break;
    case 204:
        //scriptBuffDebuff(war.getNextActionOutcome(idPlayer), e);
        break;
    case 205:
        //scriptBuffDebuff(war.getNextActionOutcome(idPlayer), e);
        break;
    case 206:
        //scriptBuffDebuff(war.getNextActionOutcome(idPlayer), e);
        break;
    case 207:
        scriptDebuffFire(war.getNextActionOutcome(idPlayer), e);
        break;
    case 208:
        scriptDebuffWater(war.getNextActionOutcome(idPlayer), e);
        break;
    case 209:
        scriptDebuffEarth(war.getNextActionOutcome(idPlayer), e);
        break;
    case 212:
        scriptChangeFormation(war.getNextActionOutcome(idPlayer), e);
        break;
    case 213:
        scriptTimeDilatation(war.getNextActionOutcome(idPlayer), e);
        break;
    case 214:
        //scriptHeroAttack(e);
        break;
    case 215:
        //scriptHeroAttack(e);
        break;
    case 216:
        scriptPurify(war.getNextActionOutcome(idPlayer), e);
        break;
    case 217:
        scriptConversion(war.getNextActionOutcome(idPlayer), e);
        break;
    case 218:
        scriptSummon(war.getNextActionOutcome(idPlayer), e);
        break;
    case 220:
        scriptSwap(war.getNextActionOutcome(idPlayer), e);
        break;
    case 219:
        scriptTrueForm(war.getNextActionOutcome(idPlayer), e);
        break;
    case 221:
        scriptSomniferous(war.getNextActionOutcome(idPlayer), e);
        break;
    case 222:
        scriptCurse(war.getNextActionOutcome(idPlayer), e);
        break;
    case 223:
        scriptHex(war.getNextActionOutcome(idPlayer), e);
        break;
    case 227:
        scriptTarot(war.getNextActionOutcome(idPlayer), e);
        break;
    case 228:
        scriptTeleport(war.getNextActionOutcome(idPlayer), e);
        break;
    case 230:
        scriptConfusion(war.getNextActionOutcome(idPlayer), e);
        break;
    case 231:
        scriptHelp(war.getNextActionOutcome(idPlayer), e);
        break;
    case 232:
        //scriptBuffDebuff(war.getNextActionOutcome(idPlayer), e);
        break;
    case 233:
        scriptDebuffAir(war.getNextActionOutcome(idPlayer), e);
        break;
    case 235:
        scriptSleep(war.getNextActionOutcome(idPlayer), e);
        break;
    case 237:
        //scriptHeroAttack(e);
        break;
    case 239:
        scriptSlavesCall(war.getNextActionOutcome(idPlayer), e);
        break;
    case 240:
        scriptCroak(war.getNextActionOutcome(idPlayer), e);
        break;
    case 241:
        scriptStampede(war.getNextActionOutcome(idPlayer), e);
        break;
    case 242:
        scriptBecomeHuman(war.getNextActionOutcome(idPlayer), e);
        break;
    case 400:
        scriptArmyVsArmy(war.getNextActionOutcome(idPlayer), e);
        break;
    case 401:
        scriptManVsMan(war.getNextActionOutcome(idPlayer), e);
        break;
    case 402:
        scriptEnslave(war.getNextActionOutcome(idPlayer), e);
        break;
    case 403:
        scriptMercy(war.getNextActionOutcome(idPlayer), e);
        break;
    case 500:
        scriptPresentArmy(e);
        break;
    case 501:
        scriptPresentCaptain(e);
        break;
    case 502:
        scriptAdvanceArmy(e);
        break;
    case 503:
        scriptReturnArmy(e);
        break;
    case 504:
        scriptRemoveDead(e);
        break;
    case 512:
        scriptThrowCoin(war.getNextActionOutcome(idPlayer), e);
        break;
    case 514:
        scriptBattleCleanup();
        break;
    case 515:
        scriptVictoryAnimation(0);
        break;
    default:
        break;
    }
    if (war.getNextActionOutcome(idPlayer).action < 100) {
        scriptPostAttackSpeech(war.getNextActionOutcome(idPlayer), e, getActingDuration());
    }

    notify(SCENE_STARTED, e);
}

void ScriptedAnimation::scriptDebuffEarth(ActionOutcome& outcome, Entity* e) {
    Entity* eObj;
    int hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;
    //int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:+1;
    Entity* eTarget = e->get<CPlayer>()->enemy;

    //double cxArmy = eTarget->get<CArmy>()->x - sign*wFormation*uxFormation/2;

    double tAux;

    ///CHOPPER
    sf::Vector2f middlePoint = getArmyMiddlePoint(eTarget);
    double yOff = -240;
    double yStart = -100;
    double chopperSpeed = 100;
    double yAmp = 5;
    double angSpeed = 360;
    double tChopperArrive;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(middlePoint.x, yStart));
    eObj->add(new CAnimation(hFlip, "air-balloon.png"));
    eObj->add(new CDraw(CDraw::WORLD_3));
    eObj->add(new CActor());
    eObj->add(new CVelocity());

    eObj->get<CActor>()->addNode(new AMove(0.0, middlePoint.x, cyWindow + yOff, chopperSpeed));
    tAux = getTravelTime(middlePoint.x, yStart, middlePoint.x, cyWindow + yOff, chopperSpeed);
    eObj->get<CActor>()->addNode(new AAddComponent(tAux, new CElipsoidalMovement(middlePoint.x, cyWindow + yOff, 0, yAmp, angSpeed, 0, false, true), Component::ELIPSOIDAL_MOVEMENT));
    eObj->get<CActor>()->addNode(new ARemoveComponent(2, Component::ELIPSOIDAL_MOVEMENT));
    eObj->get<CActor>()->addNode(new AMove(0.0, middlePoint.x, yStart, chopperSpeed));
    eObj->get<CActor>()->addNode(new ADestroy(tAux));

    addActor(eObj);

    tChopperArrive = tAux;

    for(auto& i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(eTarget, i);
        double tStart = randomDouble(tChopperArrive, tChopperArrive + 1.0);
        Entity* eGem = eManager->createEntity();
        eGem->add(new CPosition(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y));
        eGem->add(new CDraw(CDraw::WORLD_2));
        eGem->add(new CTexture("type-02-gem.png"));
        eGem->add(new CActor());
        eGem->add(new CVelocity());
        eGem->get<CDraw>()->isHidden = true;

        eGem->get<CActor>()->timeline.push_back(new AVariable(tStart, AVariable::HIDDEN, false));
        eGem->get<CActor>()->timeline.push_back(new AMove(0.0, middlePoint.x, cyWindow + yOff, 200));
        eGem->get<CActor>()->timeline.push_back(new ADestroy(getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, middlePoint.x, cyWindow + yOff, 200)));

        eUnit->get<CActor>()->timeline.push_back(new AVariable(tStart + 2, AVariable::ADD_EFFECT, (int)outcome.action));

        scriptBlockIcon(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, tStart + 2, opposite(hFlip), CUnit::EARTH);

        addActor(eGem);
    }

    Entity* eHero = e->get<CArmy>()->captain;
    std::string heroName = toUpper(eHero->get<CCaptain>()->uniqueName);
    std::string speech = Assets::getString("SPEECH-" + heroName + "-EARTH-DEBUFF-" + int2str(randomInt(1,1), 2));
    eHero->get<CActor>()->addNode(new ASpeak(0.f, speech, 2.5f));
}

void ScriptedAnimation::scriptDebuffWater(ActionOutcome& outcome, Entity* e) {
    Entity* eObj;
    int hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;
    //int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:+1;
    Entity* eTarget = e->get<CPlayer>()->enemy;

    //double cxArmy = eTarget->get<CArmy>()->x - sign*wFormation*uxFormation/2;

    double tAux;

    ///CLOUD
    sf::Vector2f middlePoint = getArmyMiddlePoint(eTarget);
    double yOff = -240;
    double yStart = cyWindow + yOff;
    double yAmp = 5;
    double angSpeed = 360;
    //double tChopperArrive;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(middlePoint.x, yStart));
    eObj->add(new CTexture("cloud-01.png"));
    eObj->add(new CDraw(CDraw::WORLD_3, 0.f));
    eObj->add(new CElipsoidalMovement(middlePoint.x, yStart, 0, yAmp, angSpeed, 0, false, true));
    eObj->add(new CActor());

    eObj->get<CActor>()->addNode(new AFade(0.0, 255.f, 255.f));
    tAux = 1.f;
    eObj->get<CActor>()->addNode(new AFade(tAux + 6.5f, -255.f, 0.f));
    eObj->get<CActor>()->addNode(new ADestroy(tAux));

    addActor(eObj);

    ///HAND
    double yHand = yStart + 45;
    double tHandReady = 1.f;
    double tdHandStill = 3.f;
    double handSpeed = 35;

    eObj = eManager->createEntity();
    eObj->add(new CPosition(middlePoint.x, yStart));
    eObj->add(new CTexture("hand-of-god.png"));
    eObj->add(new CDraw(CDraw::WORLD_2));
    eObj->add(new CActor());
    eObj->add(new CVelocity());
    eObj->get<CDraw>()->isHidden = true;

    eObj->get<CActor>()->addNode(new AVariable(1.f, AVariable::HIDDEN, false));
    eObj->get<CActor>()->addNode(new AMove(0.f, middlePoint.x, yHand, handSpeed));
    tAux = getTravelTime(middlePoint.x, yStart, middlePoint.x, yHand, handSpeed);
    tHandReady += tAux;
    eObj->get<CActor>()->addNode(new AAddComponent(tAux, new CElipsoidalMovement(middlePoint.x, yHand, 0, yAmp, angSpeed, 0.f, false, true), Component::ELIPSOIDAL_MOVEMENT));
    eObj->get<CActor>()->addNode(new ARemoveComponent(tdHandStill, Component::ELIPSOIDAL_MOVEMENT));
    eObj->get<CActor>()->addNode(new AMove(0.0, middlePoint.x, yStart, handSpeed));
    eObj->get<CActor>()->addNode(new ADestroy(tAux));

    for (auto& i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(eTarget, i);
        double tStart = randomDouble(tHandReady, tHandReady + 1.0);
        Entity* eGem = eManager->createEntity();
        eGem->add(new CPosition(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y));
        eGem->add(new CDraw(CDraw::WORLD_2));
        eGem->add(new CTexture("type-02-gem.png"));
        eGem->add(new CActor());
        eGem->add(new CVelocity());
        eGem->get<CDraw>()->isHidden = true;

        eGem->get<CActor>()->timeline.push_back(new AVariable(tStart, AVariable::HIDDEN, false));
        eGem->get<CActor>()->timeline.push_back(new AMove(0.0, middlePoint.x, yHand, 200));
        eGem->get<CActor>()->timeline.push_back(new ADestroy(getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, middlePoint.x, yHand, 200)));

        eUnit->get<CActor>()->timeline.push_back(new AVariable(tStart+2, AVariable::ADD_EFFECT, (int)outcome.action));

        scriptBlockIcon(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, tStart + 2, opposite(hFlip), CUnit::EARTH);

        addActor(eGem);
    }

    Entity* eHero = e->get<CArmy>()->captain;
    std::string heroName = toUpper(eHero->get<CCaptain>()->uniqueName);
    std::string speech = Assets::getString("SPEECH-" + heroName + "-WATER-DEBUFF-" + int2str(randomInt(1,1), 2));
    eHero->get<CActor>()->addNode(new ASpeak(0.f, speech, 2.5f));
}

void ScriptedAnimation::scriptConfusion(ActionOutcome& outcome, Entity* e) {
    Entity* eCap = e->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Get confused!", 3));

    Entity* eEnemyHero = e->get<CPlayer>()->enemy->get<CArmy>()->captain;
    eEnemyHero->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::ADD_EFFECT, (int)outcome.action));
}

void ScriptedAnimation::scriptSummon(ActionOutcome& outcome, Entity* e) {
    int hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;
    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:+1;

    CUnit::ID id = 19;
    int n = outcome.iValue;

    ///CREATE-SUMMONS
    EntityList summons;
    for(int i = 0; i < n; i++) {
        Entity* eUnit = eManager->createEntity();
        int subUnit = randomInt(1, 2);
        CUnit u = CUnit::Map[id];
        u.aIdle = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aWalk = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aDeath = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aDead = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        eUnit->add(new CUnit(u));
        eUnit->add(new COwner(e));
        eUnit->add(new CAnimation(hFlip, u.aIdle));
        eUnit->add(new CDraw(CDraw::WORLD));
        eUnit->add(new CVelocity());
        eUnit->add(new CPosition());
        eUnit->get<CVelocity>()->speed = 80;
        eUnit->add(new CActor());
        eUnit->add(new CUnitHighlight2(Assets::colors[id]));
        eUnit->get<CUnit>()->armyID = e->get<CArmy>()->nextUnitID++;

        e->get<CArmy>()->allUnits.push_back(eUnit);
        summons.push_back(eUnit);
    }

    //double dx = wWalkStep;
    double ux = uxFormation;
    double uy = uyFormation;
    double x0 = e->get<CArmy>()->x;
    double y0 = cyWindow - uy*hFormation/2;

    ///ANIM
    list<sf::Vector2i> P = positions[e->get<CArmy>()->formation];
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        if (contains(summons, eUnit)) {
            eUnit->get<CActor>()->timeline.push_back(new ATeleport(1.0, x0 + sign*p.x*ux, y0 + p.y*uy));
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aIdle));
        } else {
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
            eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, 200));
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                        getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 200),
                        eUnit->get<CUnit>()->aIdle));
        }
        addActor(eUnit);
    }

    Entity* eCap = e->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Meet my new friends!", 2));

}

void ScriptedAnimation::scriptChangeFormation(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    int sign = eEnemy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    bool hFlip = eEnemy->get<CPlayer>()->side == CPlayer::LEFT ? false:true;

    double tStart = 0.5;
    //double durEtelkaAnim = Assets::getAnimation("etelka-change-formation.png").duration;
    double animDuration = 3;

    Entity* eEtelka = e->get<CArmy>()->captain;
    eEtelka->get<CActor>()->addNode(new ASpeak(tStart, Assets::getString("SPEECH-ETELKA-FORMATION"), animDuration));
    eEtelka->get<CActor>()->addNode(new ASpriteAnimation(0.0, "etelka-change-formation.png"));
    eEtelka->get<CActor>()->addNode(new ASpriteAnimation(animDuration, eEtelka->get<CCaptain>()->aIdle));
    addActor(eEtelka);


    Entity* eCapEnemy = eEnemy->get<CArmy>()->captain;
    eEnemy->get<CArmy>()->armyEffects.erase(eEnemy->get<CArmy>()->armyEffects.find(eEnemy->get<CArmy>()->formation + 300));
    eEnemy->get<CArmy>()->formation = outcome.formation;

    //double dx = wWalkStep;
    double ux = uxFormation;
    double uy = uyFormation;
    double x0 = eEnemy->get<CArmy>()->x;
    double y0 = cyWindow - uy*hFormation/2;

    list<sf::Vector2i> P = positions[eEnemy->get<CArmy>()->formation];
    for(EntityListIt i = eEnemy->get<CArmy>()->allUnits.begin(); i != eEnemy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart + 0.5, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, 200));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                    getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 200),
                    eUnit->get<CUnit>()->aIdle));
        addActor(eUnit);
    }

    double turnFrequency = 2;
    eCapEnemy->get<CActor>()->addNode(new ASpeak(tStart + 0.5, "!!!!", animDuration));
    eCapEnemy->get<CActor>()->addNode(new AVariable(0.0, AVariable::H_FLIP, opposite(hFlip)));
    eCapEnemy->get<CActor>()->addNode(new AVariable(1.0/turnFrequency, AVariable::H_FLIP, hFlip));
    eCapEnemy->get<CActor>()->addNode(new AVariable(1.0/turnFrequency, AVariable::H_FLIP, opposite(hFlip)));
    eCapEnemy->get<CActor>()->addNode(new AVariable(1.0/turnFrequency, AVariable::H_FLIP, hFlip));
    eCapEnemy->get<CActor>()->addNode(new AVariable(1.0/turnFrequency, AVariable::H_FLIP, opposite(hFlip)));
    eCapEnemy->get<CActor>()->addNode(new AVariable(1.0/turnFrequency, AVariable::H_FLIP, hFlip));
    eCapEnemy->get<CActor>()->addNode(new AVariable(0.f, AVariable::ADD_EFFECT, eEnemy->get<CArmy>()->formation + 300));
    addActor(eCapEnemy);

}

void ScriptedAnimation::scriptHex(ActionOutcome& outcome, Entity* e) {
    bool hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    Entity* eObj;

    Entity* eCapNew = nullptr;
    Entity* eCapOld = eEnemy->get<CArmy>()->captain;

    Entity* eEtelka = e->get<CArmy>()->captain;
    ///ANIMATE ETELKA
    double tStart = 0.5;
    double durEtelkaAnimation = Assets::getAnimation("etelka-hex.png").duration;
    double tShot = tStart + durEtelkaAnimation/2;
    double shotSpeed = 200;
    double gravity = 200;
    double shotAngle = getAngleToHit(eEtelka->get<CPosition>()->x, eEtelka->get<CPosition>()->y,
                                     eCapOld->get<CPosition>()->x, eCapOld->get<CPosition>()->y,
                                     shotSpeed, gravity);
    double durShotTravel = getTravelTime(eCapOld->get<CPosition>()->x, 0, eEtelka->get<CPosition>()->x, 0, cos(shotAngle*M_RAD)*shotSpeed);
    double tHit = tShot + durShotTravel;

    eEtelka->get<CActor>()->addNode(new ASpeak(tStart, Assets::getString("SPEECH-ETELKA-HEX"), 2.5));

    eEtelka->get<CActor>()->addNode(new ASpriteAnimation(0.0, "etelka-hex.png"));
    eEtelka->get<CActor>()->addNode(new ASpriteAnimation(durEtelkaAnimation, eEtelka->get<CCaptain>()->aIdle));

    if (!outcome.bValue) {
        std::string strSpeech = "SPEECH-FAILED-HEX-" + toUpper(eCapOld->get<CCaptain>()->uniqueName) + "-" + int2str(randomInt(1,1), 2);
        eCapOld->get<CActor>()->addNode(new ASpeak(tHit, Assets::getString(strSpeech), 2.5));
        eCapOld->get<CActor>()->addNode(new AVariable(2.0f, AVariable::HIDDEN, eEtelka->get<CDraw>()->isHidden));
    }

    ///ANIMATE ARROW
    eObj = eManager->createEntity();
    eObj->add(new CPosition(eEtelka->get<CPosition>()->x, eEtelka->get<CPosition>()->y));
    eObj->add(new CDraw(CDraw::SKY));
    eObj->add(new CTexture("arrow-03.png"));
    eObj->add(new CRotation(true));
    eObj->add(new CVelocity());
    eObj->add(new CAcceleration());
    eObj->add(new CActor());

    eObj->get<CDraw>()->isHidden = true;
    eObj->get<CActor>()->addNode(new AVariable(tShot, AVariable::HIDDEN, false));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::AUTO_P, true));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_ACC, 0.0));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_ACC, gravity));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_VEL, cos(shotAngle*M_RAD)*shotSpeed));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_VEL, -sin(shotAngle*M_RAD)*shotSpeed));
    eObj->get<CActor>()->addNode(new ADestroy(durShotTravel));

    addActor(eObj);

    if (eCapOld->get<CCaptain>()->id != 14 && outcome.bValue) {
        eEnemy->get<CPlayer>()->heroDeck.remove((int)eCapOld->get<CCaptain>()->id);
        //eManager->removeEntity(eCapOld);
        map<CCaptain::ID, Entity*>::iterator it;
        it = eEnemy->get<CArmy>()->captains.find(eCapOld->get<CCaptain>()->id);
        eEnemy->get<CArmy>()->captains.erase(it);

        CCaptain::ID id = 14;
        eEnemy->get<CPlayer>()->heroDeck.push_back(id);

        eCapNew = eManager->createEntity();
        eCapNew->add(new CCaptain(CCaptain::Map[id]));
        eCapNew->add(new CAnimation(opposite(hFlip), CCaptain::Map[id].aIdle));
        eCapNew->add(new CDraw(CDraw::WORLD_1));
        eCapNew->add(new CVelocity());
        eCapNew->add(new CPosition(-200, -200));
        eCapNew->add(new CActor());
        eCapNew->add(new CDimensions(50, 50));
        eCapNew->add(new COwner(eEnemy));
        eCapNew->get<CVelocity>()->speed = 80;
        eCapNew->addObservedEntity("OriginalHero", eCapOld);

        eEnemy->get<CArmy>()->captains.insert(make_pair(id, eCapNew));
        //eEnemy->get<CArmy>()->captain = eCap;

        ///REPLACE HERO
        eCapOld->addObservedEntity("ReplaceHero", eCapNew);
        eCapOld->get<CActor>()->addNode(new ATeleport(tHit, -200, -200));
        eCapOld->get<CActor>()->addNode(new AVariable(0.0, AVariable::REPLACE_HERO));

        eCapNew->get<CActor>()->addNode(new ATeleport(tHit, eCapOld->get<CPosition>()->x, eCapOld->get<CPosition>()->y));
        eCapNew->get<CActor>()->addNode(new ASpeak(0.0, Assets::getString("SPEECH-FROG"), 2));

        addActor(eCapOld);
        addActor(eCapNew);
    } else {

    }

    ///PUFF
    string puffAnimation = "poof-02.png";
    string sfxPoofIn = "sfx-poof-04.wav";
    string sfxPoofOut = "sfx-poof-03.wav";
    double puffDuration = Assets::getAnimation(puffAnimation).duration;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(eCapOld->get<CPosition>()->x, eCapOld->get<CPosition>()->y));
    eObj->add(new CDraw(CDraw::WORLD_2, true, 255));
    eObj->add(new CDimensions(60, 60));
    eObj->add(new CAnimation(false, puffAnimation));
    eObj->add(new CActor());

    eObj->get<CActor>()->addNode(new ASound(tHit, sfxPoofOut));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
    eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
    eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

    addActor(eObj);

}

void ScriptedAnimation::scriptTarot(ActionOutcome& outcome, Entity* e) {
    Entity* eCap = e->get<CArmy>()->captain;

    ///CARDS
    Entity* eObj;
    double yOff = -90;
    double w = Assets::getAnimation("tarot-card-back.png").wSprite;
    //double h = Assets::getAnimation("tarot-card-back.png").hSprite;
    double spacing = w + 10;
    double x0 = eCap->get<CPosition>()->x - (spacing);
    double y0 = eCap->get<CPosition>()->y + yOff;
    string spriteCard;
    switch(outcome.iValue) {
    case 1:
        spriteCard = "tarot-luck";
        break;
    case 2:
        spriteCard = "tarot-neutral";
        break;
    case 3:
        spriteCard = "tarot-bad-luck";
        break;
    }

    //double t = 0;
    int index = randomInt(0, 2);
    double x = x0;
    double y = y0;
    for(int i = 0; i < 3; i++) {
        eObj = eManager->createEntity();
        eObj->add(new CPosition(x, y));
        eObj->add(new CDraw(CDraw::GUI1, double(0.0)));
        eObj->add(new CAnimation(false, "tarot-card-back.png"));
        eObj->add(new CActor());
        eObj->add(new CElipsoidalMovement(x, y0, 0, 5, 360, 0));
        eObj->add(new CVelocity());

        eObj->get<CActor>()->timeline.push_back(new AFade(0.0, 255, 255));

        if (index == i) {
            eObj->get<CActor>()->timeline.push_back(new ASpriteAnimation(2, spriteCard + "-spin.png"));
            eObj->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(spriteCard + "-spin.png").duration, spriteCard + ".png"));
        } else {
            eObj->get<CActor>()->timeline.push_back(new ASpriteAnimation(2, "tarot-card-back.png"));
            eObj->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(spriteCard + "-spin.png").duration, "tarot-card-back.png"));
        }

        eObj->get<CActor>()->timeline.push_back(new AFade(2, -255, 0.0));
        eObj->get<CActor>()->timeline.push_back(new ADestroy(1));

        x += spacing;

        addActor(eObj);
    }
    double tEndCardAnimation = 2 + Assets::getAnimation(spriteCard + "-spin.png").duration + 2 + 1;

    double when = tEndCardAnimation;
    Entity* eArmy = e;
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    for (auto i : outcome.idActors) {
        Entity* eUnit = getUnitByID(eArmy, i);

        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eUnit->get<CUnit>()->aDeath));
        eUnit->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eUnit->get<CUnit>()->aDeath).duration, eUnit->get<CUnit>()->aDead));
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));
        scriptDeathIcon(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, when, eUnit->get<CAnimation>()->hFlip, CUnit::N_DAMAGE_TYPES);

        addActor(eUnit);
    }
    for (auto i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(eEnemy, i);

        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eUnit->get<CUnit>()->aDeath));
        eUnit->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eUnit->get<CUnit>()->aDeath).duration, eUnit->get<CUnit>()->aDead));
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));
        scriptDeathIcon(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, when, eUnit->get<CAnimation>()->hFlip, CUnit::N_DAMAGE_TYPES);

        addActor(eUnit);
    }

    //hero
    Entity* eHero = e->get<CArmy>()->captain;
    std::string heroName = toUpper(eHero->get<CCaptain>()->uniqueName);
    std::string suffix = (outcome.iValue == 1 ? "GOOD" : (outcome.iValue == 2 ? "NEUTRAL" : "BAD"));
    std::string speech = Assets::getString("SPEECH-" + heroName + "-TAROT-" + suffix + "-" + int2str(randomInt(1, 1), 2));

    eHero->get<CActor>()->addNode(new ASpriteAnimation(0.0, "nagendra-tarot-01.png"));
    eHero->get<CActor>()->addNode(new ASpriteAnimation(3.5, "nagendra-tarot-02.png"));
    eHero->get<CActor>()->addNode(new ASpeak(0.f, speech, 2.5f));
    eHero->get<CActor>()->addNode(new ASpriteAnimation(3.5, eHero->get<CCaptain>()->aIdle));
}

void ScriptedAnimation::scriptPurify(ActionOutcome& outcome, Entity* e) {
    Entity* eHero = e->get<CArmy>()->captain;

    ///ANIMATE HERO
    double tStart = 0.5;
    double durEtelkaAnimation = Assets::getAnimation("goraidh-purification.png").duration;
    double tShot = tStart + durEtelkaAnimation/2;
    double shotSpeed = 200;
    double gravity = 200;
    sf::Vector2f armyCenter = getArmyMiddlePoint(e);
    double yTarget = cyWindow - hFormation*uyFormation/2 - 100;
    double shotAngle = getAngleToHit(eHero->get<CPosition>()->x, eHero->get<CPosition>()->y, armyCenter.x, yTarget, shotSpeed, gravity);
    double durShotTravel = getTravelTime(eHero->get<CPosition>()->x, 0, armyCenter.x, 0, cos(shotAngle*M_RAD)*shotSpeed);
    //double tHit = tShot + durShotTravel;

    eHero->get<CActor>()->addNode(new ASpeak(tStart, Assets::getString("SPEECH-GORAIDH-PURIFY-" + int2str(randomInt(1,1), 2)), 2.5));
    eHero->get<CActor>()->addNode(new ASpriteAnimation(0.0, "goraidh-purification.png"));
    eHero->get<CActor>()->addNode(new ASpriteAnimation(durEtelkaAnimation, eHero->get<CCaptain>()->aIdle));

    ///ANIMATE FLASK
    Entity* eObj;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(eHero->get<CPosition>()->x, eHero->get<CPosition>()->y));
    eObj->add(new CDraw(CDraw::SKY));
    eObj->add(new CTexture("purification-flask.png"));
    eObj->add(new CRotation((double)360));
    eObj->add(new CVelocity());
    eObj->add(new CAcceleration());
    eObj->add(new CActor());
    eObj->get<CRotation>()->velocity = 360;

    eObj->get<CDraw>()->isHidden = true;
    eObj->get<CActor>()->addNode(new AVariable(tShot, AVariable::HIDDEN, false));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::AUTO_P, true));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_ACC, 0.0));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_ACC, gravity));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_VEL, cos(shotAngle*M_RAD)*shotSpeed));
    eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_VEL, -sin(shotAngle*M_RAD)*shotSpeed));
    eObj->get<CActor>()->addNode(new ASound(durShotTravel, "sfx-chimes-01.ogg"));
    eObj->get<CActor>()->addNode(new ADestroy(0.0));

    scriptPurificationFlask(armyCenter.x, yTarget, tShot + durShotTravel);

    addActor(eObj);

    /// UNIT EFFECTS
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        for (auto it = eUnit->get<CUnit>()->effects.begin(); it != eUnit->get<CUnit>()->effects.end(); it++) {
            if (it->second.effectType == CAction::DEBUFF) {
                eUnit->get<CActor>()->timeline.push_back(new AVariable(tShot + durShotTravel, AVariable::REMOVE_EFFECT, it->first));
            }
        }
    }
    
    /// ARMY EFFECTS
    bool firstOne = true;
    for (auto it = e->get<CArmy>()->armyEffects.begin(); it != e->get<CArmy>()->armyEffects.end(); it++) {
        if (it->second.effectType == CAction::DEBUFF) {
            if (firstOne) {
                eHero->get<CActor>()->timeline.push_back(new AVariable(durShotTravel, AVariable::REMOVE_EFFECT, it->first));
            } else {
                eHero->get<CActor>()->timeline.push_back(new AVariable(0.f, AVariable::REMOVE_EFFECT, it->first));
            }
        }
    }
}

void ScriptedAnimation::scriptResurect(ActionOutcome& outcome, Entity* e) {
    EntityList eTargets;
    for(auto& i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(e, i);
        eTargets.push_back(eUnit);
    }

    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    //double dx = wWalkStep;
    double ux = uxFormation;
    double uy = uyFormation;
    double x0 = e->get<CArmy>()->x;
    double y0 = cyWindow - uy*hFormation/2;

    list<sf::Vector2i> P = positions[e->get<CArmy>()->formation];
    for(Entity* eUnit : e->get<CArmy>()->allUnits) {
        if (eUnit->get<CUnit>()->dead && !contains(eTargets, eUnit)) continue;
        sf::Vector2i p = popFront(P);

        if (contains(eTargets, eUnit)) {
            ///HEART
            double xHeart = eUnit->get<CPosition>()->x;
            double yHeart = randomDouble(-250, -20);
            double heartSpeed = 110;
            double durHeartTravel = getTravelTime(xHeart, yHeart, eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, heartSpeed);
            Entity* eObj = eManager->createEntity();
            eObj->add(new CPosition(xHeart, yHeart));
            eObj->add(new CDraw(CDraw::SKY));
            eObj->add(new CAnimation(false, "heart-flying.png"));
            eObj->add(new CActor());
            eObj->add(new CVelocity());

            eObj->get<CActor>()->addNode(new AMove(0.0, eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, heartSpeed));
            eObj->get<CActor>()->addNode(new ADestroy(durHeartTravel));

            ///UNIT
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(durHeartTravel, eUnit->get<CUnit>()->aWalk));
            eUnit->get<CActor>()->addNode(new AVariable(0.0, AVariable::DEAD, false));
            eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, 200));
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                        getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 200),
                        eUnit->get<CUnit>()->aIdle));

        } else {
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(2.0, eUnit->get<CUnit>()->aWalk));
            eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, 200));
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                        getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 200),
                        eUnit->get<CUnit>()->aIdle));
        }
        addActor(eUnit);
    }

    Entity* eCap = e->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Come back, my friends!", 2));
    addActor(eCap);

}

void ScriptedAnimation::scriptCurse(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    
    for (auto i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(eEnemy, i);
        eUnit->get<CActor>()->addNode(new AVariable(2.5f, AVariable::ADD_EFFECT, outcome.action));
        scriptCurseIcon(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, 2.5f, eUnit->get<CAnimation>()->hFlip);
    }

    Entity* eHero = e->get<CArmy>()->captain;
    eHero->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Ooga booga wooga!", 2));
    eHero->get<CActor>()->timeline.push_back(new ASpriteAnimation(4.0, eHero->get<CCaptain>()->aIdle));
    addActor(eHero);

    Entity* eSound = eManager->createEntity();
    eSound->add(new CSound("sfx-tribal-drums.ogg", CSound::REMOVE_ENTITY));
    notify(PLAY_SOUND, eSound);
}

void ScriptedAnimation::scriptSomniferous(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    Entity* eEnemyHero = eEnemy->get<CArmy>()->captain;

    eEnemyHero->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::ADD_EFFECT, (int)outcome.action));

    Entity* eHero = e->get<CArmy>()->captain;
    eHero->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Lalala!", 2));
    addActor(eHero);
}

void ScriptedAnimation::scriptSleep(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    EntityList targets;
    for(auto& i : outcome.idTargets) {
        targets.push_back(getUnitByID(eEnemy, i));
    }
    for(auto& eUnit : targets) {
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::ADD_EFFECT, (int)outcome.action));
    }

    Entity* eHero = e->get<CArmy>()->captain;
    eHero->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Lalala!", 2));
    addActor(eHero);
}

void ScriptedAnimation::scriptTimeDilatation(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    eEnemy->get<CPlayer>()->turnDuration -= outcome.iValue;

    Entity* eHero = e->get<CArmy>()->captain;
    eHero->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Lalala!", 2));
    addActor(eHero);
}

void ScriptedAnimation::scriptSlavesCall(ActionOutcome& outcome, Entity* e) {
    int hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;
    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:+1;

    CUnit::ID id = 20;
    int n = outcome.iValue;

    ///CREATE-SUMMONS
    EntityList summons;
    for(int i = 0; i < n; i++) {
        Entity* eUnit = eManager->createEntity();
        int subUnit = randomInt(1, 2);
        CUnit u = CUnit::Map[id];
        u.aIdle = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aWalk = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aDeath = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        u.aDead = "unit-" + int2str(id, 2) + "-" + int2str(subUnit, 2) + "-idle.png";
        eUnit->add(new CUnit(u));
        eUnit->add(new COwner(e));
        eUnit->add(new CAnimation(hFlip, u.aIdle));
        eUnit->add(new CDraw(CDraw::WORLD_1));
        eUnit->add(new CVelocity());
        eUnit->add(new CPosition(cxWindow + sign*wWindow/2 + sign*randomDouble(32, 200), cyWindow + randomDouble(-50, 50)));
        eUnit->get<CVelocity>()->speed = 80;
        eUnit->add(new CActor());
        eUnit->add(new CUnitHighlight2(Assets::colors[id]));
        eUnit->get<CUnit>()->armyID = e->get<CArmy>()->nextUnitID++;

        e->get<CArmy>()->allUnits.push_back(eUnit);
        summons.push_back(eUnit);
    }

    //double dx = wWalkStep;
    double ux = uxFormation;
    double uy = uyFormation;
    double x0 = e->get<CArmy>()->x;
    double y0 = cyWindow - uy*hFormation/2;

    ///ANIM
    list<sf::Vector2i> P = positions[e->get<CArmy>()->formation];
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        if (contains(summons, eUnit)) {
            eUnit->get<CActor>()->timeline.push_back(new AMove(1.0, x0 + sign*p.x*ux, y0 + p.y*uy, 200));
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
            double durTravel = getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 200);
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(durTravel, eUnit->get<CUnit>()->aIdle));
        } else {
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
            eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, 200));
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                        getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 200),
                        eUnit->get<CUnit>()->aIdle));
        }
        addActor(eUnit);
    }

    Entity* eCap = e->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Meet my new friends!", 2));

}

void ScriptedAnimation::scriptConversion(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    for(auto& i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(eEnemy, i);
        eUnit->get<COwner>()->e = e;
        e->get<CArmy>()->allUnits.push_back(eUnit);
        eEnemy->get<CArmy>()->allUnits.remove(eUnit);
        eUnit->get<CUnit>()->effects.clear();
    }

    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    //double dx = wWalkStep;
    double ux = uxFormation;
    double uy = uyFormation;
    double x0 = e->get<CArmy>()->x;
    double y0 = cyWindow - uy*hFormation/2;

    list<sf::Vector2i> P = positions[e->get<CArmy>()->formation];
    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, 200));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                    getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 200),
                    eUnit->get<CUnit>()->aIdle));
        addActor(eUnit);
    }

    Entity* eCap = e->get<CActionOutcome>()->actorArmy->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Wololoo", 2));
    eCap->get<CActor>()->timeline.push_back(new ASound(0.0, "wololo.wav"));
    addActor(eCap);

}

void ScriptedAnimation::scriptImmortality(ActionOutcome& outcome, Entity* e) {
    for(auto& i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(e, i);
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::ADD_EFFECT, (int)outcome.action));
    }

    Entity* eHero = e->get<CArmy>()->captain;
    eHero->get<CActor>()->timeline.push_back(new ASpeak(0.0, "BLalala!", 2));
    addActor(eHero);
}

void ScriptedAnimation::scriptDoom(ActionOutcome& outcome, Entity* e) {

}

void ScriptedAnimation::scriptTeleport(ActionOutcome& outcome, Entity* e) {
    Entity* eScenario = eManager->createEntity();
    eScenario->add(new CScenario(outcome.sValue));
    notify(LOAD_SCENARIO, eScenario);
}

void ScriptedAnimation::scriptSwap(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    Entity* eCap1 = e->get<CArmy>()->captain;
    Entity* eCap2 = eEnemy->get<CArmy>()->captain;

    e->get<CArmy>()->captains.erase(e->get<CArmy>()->captains.erase(eCap1->get<CCaptain>()->id));
    e->get<CArmy>()->captains.insert(make_pair(eCap2->get<CCaptain>()->id, eCap2));
    e->get<CArmy>()->captain = eCap2;

    eEnemy->get<CArmy>()->captains.erase(e->get<CArmy>()->captains.erase(eCap2->get<CCaptain>()->id));
    eEnemy->get<CArmy>()->captains.insert(make_pair(eCap1->get<CCaptain>()->id, eCap1));
    eEnemy->get<CArmy>()->captain = eCap1;

    eEnemy->get<CArmy>()->captain->get<CCaptain>()->actions.remove(outcome.action);


    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    double dx = wWalkStep;
    //double ux = uxFormation;
    //double uy = uyFormation;
    if (e->get<CArmy>()->x == 0) e->get<CArmy>()->x = cxWindow + sign*wWindow/2 -sign*x0Formation + sign*dx;
    double x0 = e->get<CArmy>()->x;
    //double y0 = cyWindow - uy*hFormation/2;

    Entity* eCap = e->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, x0 - sign*50, cyWindow, 200));
    double t1 = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, x0 - sign*50, cyWindow, 200);
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t1, eCap->get<CCaptain>()->aIdle));

    addActor(eCap);

    eEnemy = e->get<CPlayer>()->enemy;
    sign = eEnemy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    x0 = eEnemy->get<CArmy>()->x;

    eCap = eEnemy->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, x0 - sign*50, cyWindow, 200));
    t1 = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, x0 - sign*50, cyWindow, 200);
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t1, eCap->get<CCaptain>()->aIdle));

    addActor(eCap);
}

void ScriptedAnimation::scriptStampede(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    bool hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;

    EntityList targets;
    for (auto& i : outcome.idTargets) targets.push_back(getUnitByID(eEnemy, i));

    //double wField = 0.70*wWindow;
    //double dx = wField/((war.getMatchConfig().nTurns+1)*2);
    //double ux = 32;
    //double uy = 26;
    //double x0 = e->get<CArmy>()->x;
    //double y0 = cyWindow - uy*12/2;

    for(EntityListIt i = e->get<CArmy>()->allUnits.begin(); i != e->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        double xUnit = eUnit->get<CPosition>()->x;
        double yUnit = eUnit->get<CPosition>()->y;
        double xBuffalo = randomDouble(cxWindow + sign*1.5*wWindow + 100, cxWindow + sign*0.5*wWindow - 100);
        double yBuffalo = yUnit;
        double speed = 300;

        Entity* eBuffalo = eManager->createEntity();
        eBuffalo->add(new CPosition(xBuffalo, yBuffalo));
        eBuffalo->add(new CDraw(CDraw::WORLD_1));
        eBuffalo->add(new CAnimation(hFlip, "buffalo-run.png"));
        eBuffalo->add(new CVelocity());
        eBuffalo->add(new CActor());

        eBuffalo->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow - sign*(wWindow/2 + 100), yUnit, speed));
        eBuffalo->get<CActor>()->timeline.push_back(new ADestroy(getTravelTime(xBuffalo, yBuffalo, cxWindow - sign*(wWindow/2 + 100), yUnit, speed)));

        if (contains(targets, eUnit)) {
            eUnit->get<CActor>()->timeline.push_back(new AMove(getTravelTime(xBuffalo, yBuffalo, xUnit, yUnit, speed), cxWindow - sign*wWindow/2 - sign*100, yUnit, speed));
            double t = getTravelTime(xUnit, yUnit, cxWindow - sign*wWindow/2 - sign*100, yUnit, speed);
            eUnit->get<CActor>()->timeline.push_back(new ADestroy(t));
            addActor(eUnit);
        }

        addActor(eBuffalo);
    }

    Entity* eShake = eManager->createEntity();
    eShake->add(new CCameraShake(getTravelTime(0, 0, 2*wWindow, 0, 300)));
    notify(SHAKE_CAMERA, eShake);

    Entity* eCap = e->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Mooooooooooooooooooo!", 3));

    addActor(eCap);
    double duration = getTravelTime(0, 0, 2*wWindow, 0, 300);

    ///SAFE ZONES
    sf::FloatRect topSafeZone;
    sf::FloatRect botSafeZone;
    topSafeZone.width = uxFormation*wFormation;
    topSafeZone.height = 32;
    topSafeZone.left = e->get<CArmy>()->x + sign*topSafeZone.width;
    topSafeZone.top = cyWindow -(uyFormation*hFormation)/2 + 32/2;

    botSafeZone.width = uxFormation*wFormation;
    botSafeZone.height = 32;
    botSafeZone.left = e->get<CArmy>()->x + sign*botSafeZone.width;
    botSafeZone.top = cyWindow + (uyFormation*hFormation)/2 - 32/2 - botSafeZone.height;

    for (Entity* eUnit : e->get<CArmy>()->allUnits) {
        if (eUnit->get<CUnit>()->dead) continue;
        sf::FloatRect safeZone;
        if (abs(eUnit->get<CPosition>()->y - (topSafeZone.top + topSafeZone.height)) < abs(eUnit->get<CPosition>()->y - botSafeZone.top)) {
            safeZone = topSafeZone;
        } else {
            safeZone = botSafeZone;
        }
        double xTarget = eUnit->get<CPosition>()->x;
        double yTarget = randomDouble(safeZone.top, safeZone.top + safeZone.height);
        double speed = 200;

        double travelTime = getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, xTarget, yTarget, speed);

        eUnit->get<CActor>()->addNode(new AMove(0.0, xTarget, yTarget, speed));
        eUnit->get<CActor>()->addNode(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->addNode(new ASpriteAnimation(travelTime, eUnit->get<CUnit>()->aIdle));

        eUnit->get<CActor>()->addNode(new AMove(duration, eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, speed));
        eUnit->get<CActor>()->addNode(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->addNode(new ASpriteAnimation(travelTime, eUnit->get<CUnit>()->aIdle));
    }
}

void ScriptedAnimation::scriptBecomeHuman(ActionOutcome& outcome, Entity* e) {
    Entity* eCapOld = e->get<CArmy>()->captain;
    Entity* eCapNew = eCapOld->getObservedEntity("OriginalHero");

    e->get<CPlayer>()->heroDeck.remove((int)eCapOld->get<CCaptain>()->id);
    eManager->removeEntity(eCapOld);
    map<CCaptain::ID, Entity*>::iterator it;
    it = e->get<CArmy>()->captains.find(eCapOld->get<CCaptain>()->id);
    e->get<CArmy>()->captains.erase(it);

    CCaptain::ID id = eCapNew->get<CCaptain>()->id;
    e->get<CPlayer>()->heroDeck.push_back(id);

    e->get<CArmy>()->captains.insert(make_pair(id, eCapNew));
    e->get<CArmy>()->captain = eCapNew;


    Entity* eObj;

    ///PUFF
    string puffAnimation = "poof-02.png";
    string sfxPoofIn = "sfx-poof-04.wav";
    string sfxPoofOut = "sfx-poof-03.wav";
    double puffDuration = Assets::getAnimation(puffAnimation).duration;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(eCapOld->get<CPosition>()->x, eCapOld->get<CPosition>()->y));
    eObj->add(new CDraw(CDraw::WORLD_2));
    eObj->add(new CDimensions(85, 85));
    eObj->add(new CAnimation(false, puffAnimation));
    eObj->add(new CActor());

    eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
    eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
    eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

    eCapNew->get<CActor>()->addNode(new ATeleport(0.0, eCapOld->get<CPosition>()->x, eCapOld->get<CPosition>()->y));
    eCapNew->get<CActor>()->addNode(new ASpeak(0.0, Assets::getString("SPEECH-IMAMU"), 2));


    addActor(eObj);
    addActor(eCapNew);
}

void ScriptedAnimation::scriptCroak(ActionOutcome& outcome, Entity* e) {
    if (outcome.bValue) {
        Entity* eCapOld = e->get<CArmy>()->captain;
        Entity* eCapNew = eCapOld->getObservedEntity("OriginalHero");

        e->get<CPlayer>()->heroDeck.remove((int)eCapOld->get<CCaptain>()->id);
        eManager->removeEntity(eCapOld);
        map<CCaptain::ID, Entity*>::iterator it;
        it = e->get<CArmy>()->captains.find(eCapOld->get<CCaptain>()->id);
        e->get<CArmy>()->captains.erase(it);

        CCaptain::ID id = eCapNew->get<CCaptain>()->id;
        e->get<CPlayer>()->heroDeck.push_back(id);

        e->get<CArmy>()->captains.insert(make_pair(id, eCapNew));
        e->get<CArmy>()->captain = eCapNew;

        Entity* eObj;

        ///PUFF
        string puffAnimation = "poof-02.png";
        string sfxPoofIn = "sfx-poof-04.wav";
        string sfxPoofOut = "sfx-poof-03.wav";
        double puffDuration = Assets::getAnimation(puffAnimation).duration;
        eObj = eManager->createEntity();
        eObj->add(new CPosition(eCapOld->get<CPosition>()->x, eCapOld->get<CPosition>()->y));
        eObj->add(new CDraw(CDraw::WORLD_2));
        eObj->add(new CDimensions(85, 85));
        eObj->add(new CAnimation(false, puffAnimation));
        eObj->add(new CActor());

        eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
        eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

        eCapNew->get<CActor>()->addNode(new ATeleport(0.0, eCapOld->get<CPosition>()->x, eCapOld->get<CPosition>()->y));
        eCapNew->get<CActor>()->addNode(new ASpeak(0.0, Assets::getString("SPEECH-GRR"), 2));

        addActor(eObj);
        addActor(eCapNew);
    } else {
        Entity* eHero = e->get<CArmy>()->captain;
        eHero->get<CActor>()->addNode(new ASpeak(0.5, Assets::getString("SPEECH-CROAK-FAILED"), 2.5f));
        eHero->get<CActor>()->addNode(new AVariable(2.5f, AVariable::HIDDEN, eHero->get<CDraw>()->isHidden));
        addActor(eHero);
    }
}

void ScriptedAnimation::scriptHelp(ActionOutcome& outcome, Entity* e) {
    Entity* eCap = e->get<CArmy>()->captain;
    Entity* eHelp = e->get<CArmy>()->captains[outcome.hero];
    CAction::ID actID = outcome.iValue;

    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    bool hFlip = sign == -1 ? false:true;

    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, "Help!", 2));

    //double wFormation = 15;
    double hFormation = 23;
    //double uxFormation = 16;
    double uyFormation = 13;
    double x = cxWindow + sign*wWindow/2 + sign*60;
    double y = cyWindow + (hFormation+2)*uyFormation/2;

    eHelp->get<CActor>()->timeline.push_back(new ATeleport(0.0, x, y));
    eHelp->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, hFlip));
    eHelp->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eHelp->get<CCaptain>()->aIdle));
    eHelp->get<CActor>()->timeline.push_back(new AMove(0.0, x - 2*sign*60, y, 200));
    eHelp->get<CActor>()->timeline.push_back(new ASpriteAnimation(getTravelTime(x,y,x - 2*sign*60, y,200), eHelp->get<CCaptain>()->aIdle));

    e->get<CArmy>()->captain = eHelp;
    e->get<CArmy>()->nextAction = actID;
    //playAction(e);
    //notify(START_ANIMATION, eOut);
    e->get<CArmy>()->captain = eCap;

    eHelp->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eHelp->get<CCaptain>()->aIdle));
    eHelp->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, opposite(hFlip)));
    eHelp->get<CActor>()->timeline.push_back(new AMove(0.0, x, y, 200));
    eHelp->get<CActor>()->timeline.push_back(new ASpriteAnimation(getTravelTime(x,y,x - 2*sign*60, y,200), eHelp->get<CCaptain>()->aIdle));
    eHelp->get<CActor>()->timeline.push_back(new ATeleport(0.0, x, cyWindow));

}

void ScriptedAnimation::createProjectile(Entity* e, double x0, double y0, double x1, double y1, AShoot::Object obj) {
    if (obj == AShoot::ARROW) {
        double gravity = 60;
        double speed = 300;

        Entity* eProj = eManager->createEntity();
        eProj->add(new CAnimation(false, "arbalest-proj.png"));
        //eProj->add(new CDimensions(30, 5));
        eProj->add(new CPosition(x0, y0));
        eProj->add(new CDraw(CDraw::WORLD_2));
        eProj->add(new CAutoPilot());
        eProj->add(new CVelocity());
        eProj->add(new CAcceleration());
        eProj->add(new CRotation(true));

        double angle = getAngleToHit(x0, y0, x1, y1, speed);
        eProj->get<CVelocity>()->x = cos(angle*M_RAD)*speed;
        eProj->get<CVelocity>()->y = -sin(angle*M_RAD)*speed;
        eProj->get<CAcceleration>()->x = 0;
        eProj->get<CAcceleration>()->y = gravity;

        eProj->add(new CTimer(getTravelTime(x0, 0, x1, 0, eProj->get<CVelocity>()->x), REMOVE_ENTITY));

        ///FIRE

        Entity* eEffect = eManager->createEntity();
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(2, 2));
        shape.setFillColor(sf::Color::Red);
        eEffect->add(new CParticleEmmiter(15, shape, 0.5, 30, 30, -90, 0, 1));
        eEffect->add(new CAnchor(12, 0));
        eEffect->addObservedEntity("Anchor", eProj);
        eEffect->attachEmployer(eProj);
        eEffect->add(new CPosition(0,0));
        eEffect->get<CParticleEmmiter>()->on = true;
    } else if (obj == AShoot::ARROW_2) {
        double gravity = 80;
        double speed = 300;

        Entity* eProj = eManager->createEntity();
        eProj->add(new CAnimation(false, "arrow2.png"));
        eProj->add(new CDimensions(30, 5));
        eProj->add(new CPosition(x0, y0));
        eProj->add(new CDraw(CDraw::WORLD_2));
        eProj->add(new CAutoPilot());
        eProj->add(new CVelocity());
        eProj->add(new CAcceleration());
        eProj->add(new CRotation(true));

        double angle = getAngleToHit2(x0, y0, x1, y1, speed, gravity);
        eProj->get<CVelocity>()->x = cos(angle*M_RAD)*speed;
        eProj->get<CVelocity>()->y = -sin(angle*M_RAD)*speed;
        eProj->get<CAcceleration>()->x = 0;
        eProj->get<CAcceleration>()->y = gravity;

        eProj->add(new CTimer(getTravelTime(x0, 0, x1, 0, eProj->get<CVelocity>()->x), REMOVE_ENTITY));

    } else if (obj == AShoot::BOMB) {
        double gravity = 60;
        double speed = 300;

        Entity* eProj = eManager->createEntity();
        eProj->add(new CAnimation(false, "bomb.png"));
        eProj->add(new CPosition(x0, y0));
        eProj->add(new CDraw(CDraw::WORLD_2));
        eProj->add(new CAutoPilot());
        eProj->add(new CVelocity());
        eProj->add(new CAcceleration());
        eProj->add(new CRotation(false));
        eProj->get<CRotation>()->velocity = randomDouble(30, 300);

        double angle = getAngleToHit(x0, y0, x1, y1, speed);
        eProj->get<CVelocity>()->x = cos(angle*M_RAD)*speed;
        eProj->get<CVelocity>()->y = -sin(angle*M_RAD)*speed;
        eProj->get<CAcceleration>()->x = 0;
        eProj->get<CAcceleration>()->y = gravity;

        double travelTime = getTravelTime(x0, 0, x1, 0, eProj->get<CVelocity>()->x);
        eProj->add(new CTimer(travelTime, REMOVE_ENTITY));

        ///Explosion
        Entity* eExp = eManager->createEntity();
        string animation = "explosion-01.png";
        string explosionSound = "sfx-explosion-07-01.wav";
        eExp->add(new CAnimation(false, animation));
        eExp->add(new CPosition(x1, y1));
        eExp->add(new CActor());
        eExp->add(new CDimensions(40,40));
        eExp->add(new CDraw(CDraw::WORLD_2));
        eExp->get<CDraw>()->isHidden = true;

        eExp->get<CActor>()->timeline.push_back(new AVariable(travelTime, AVariable::HIDDEN, false));
        eExp->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
        eExp->get<CActor>()->timeline.push_back(new ASound(0.0, explosionSound));
        eExp->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));

        /*
        Entity* eEffect = eManager->createEntity();
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(2, 2));
        shape.setFillColor(sf::Color::Red);
        eEffect->add(new CParticleEmmiter(15, shape, 0.5, 30, 30, -90, 0, 1));
        eEffect->add(new CAnchor(eProj, 12, 0));
        eEffect->add(new CPosition(0,0));
        eEffect->get<CParticleEmmiter>()->on = true;
        */
    } else if (obj == AShoot::SCIMITAR) {
        double speed = 400;
        Entity* eArmy = e->get<COwner>()->e;
        int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

        Entity* eProj = eManager->createEntity();
        eProj->add(new CAnimation(false, "flying-scimitar.png"));
        eProj->add(new CPosition(x0, y0));
        eProj->add(new CDraw(CDraw::WORLD_2));
        eProj->add(new CVelocity());
        eProj->add(new CActor());
        double angle = angleBetweenPoints(x0, y0, x1, y1);
        double dx = sign == -1 ? wWindow - x0 : x0;
        double dy = -sign*tan(angle*M_RAD)*dx;
        double travelTime = getTravelTime(x0, y0, x0 - sign*dx, y0 + dy, speed);
        eProj->get<CActor>()->timeline.push_back(new AMove(0.0, x0 - sign*dx, y0 + dy, speed));
        eProj->get<CActor>()->timeline.push_back(new ADestroy(travelTime));

        //notify(PLAY_SOUND, eProj);

    } else if (obj == AShoot::ROCK) {
        double gravity = 60;
        double speed = 450;

        Entity* eProj = eManager->createEntity();
        eProj->add(new CTexture("rock.png"));
        eProj->add(new CDimensions(10, 10));
        eProj->add(new CPosition(x0, y0));
        eProj->add(new CDraw(CDraw::WORLD_2));
        eProj->add(new CAutoPilot());
        eProj->add(new CVelocity());
        eProj->add(new CAcceleration());
        eProj->add(new CRotation(false));
        eProj->get<CRotation>()->velocity = randomDouble(30, 300);

        double angle = getAngleToHit(x0, y0, x1, y1, speed);
        eProj->get<CVelocity>()->x = cos(angle*M_RAD)*speed;
        eProj->get<CVelocity>()->y = -sin(angle*M_RAD)*speed;
        eProj->get<CAcceleration>()->x = 0;
        eProj->get<CAcceleration>()->y = gravity;

        eProj->add(new CTimer(getTravelTime(x0, 0, x1, 0, eProj->get<CVelocity>()->x), REMOVE_ENTITY));
    } else if (obj == AShoot::SPEAR) {
        double gravity = 60;
        double speed = 450;

        Entity* eProj = eManager->createEntity();
        eProj->add(new CTexture("spear.png"));
        eProj->add(new CPosition(x0, y0));
        eProj->add(new CDraw(CDraw::WORLD_2));
        eProj->add(new CAutoPilot());
        eProj->add(new CVelocity());
        eProj->add(new CAcceleration());
        eProj->add(new CRotation(true));

        double angle = getAngleToHit(x0, y0, x1, y1, speed);
        eProj->get<CVelocity>()->x = cos(angle*M_RAD)*speed;
        eProj->get<CVelocity>()->y = -sin(angle*M_RAD)*speed;
        eProj->get<CAcceleration>()->x = 0;
        eProj->get<CAcceleration>()->y = gravity;

        eProj->add(new CTimer(getTravelTime(x0, 0, x1, 0, eProj->get<CVelocity>()->x), REMOVE_ENTITY));
    } else if (obj == AShoot::DART) {
        double gravity = 60;
        double speed = 450;

        Entity* eProj = eManager->createEntity();
        eProj->add(new CTexture("dart.png"));
        eProj->add(new CPosition(x0, y0));
        eProj->add(new CDraw(CDraw::WORLD_2));
        eProj->add(new CAutoPilot());
        eProj->add(new CVelocity());
        eProj->add(new CAcceleration());
        eProj->add(new CRotation(true));

        double angle = getAngleToHit(x0, y0, x1, y1, speed);
        eProj->get<CVelocity>()->x = cos(angle*M_RAD)*speed;
        eProj->get<CVelocity>()->y = -sin(angle*M_RAD)*speed;
        eProj->get<CAcceleration>()->x = 0;
        eProj->get<CAcceleration>()->y = gravity;

        eProj->add(new CTimer(getTravelTime(x0, 0, x1, 0, eProj->get<CVelocity>()->x), REMOVE_ENTITY));
    } else if (obj == AShoot::ERIDU_BLADE) {
        double gravity = 60;
        double speed = 300;

        Entity* eProj = eManager->createEntity();
        eProj->add(new CAnimation(false, "eridu-blade-spining.png"));
        eProj->add(new CPosition(x0, y0));
        eProj->add(new CDimensions(30, 30));
        eProj->add(new CDraw(CDraw::GUI2));
        eProj->add(new CAutoPilot());
        eProj->add(new CVelocity());
        eProj->add(new CAcceleration());

        double angle = getAngleToHit(x0, y0, x1, y1, speed);
        eProj->get<CVelocity>()->x = cos(angle*M_RAD)*speed;
        eProj->get<CVelocity>()->y = -sin(angle*M_RAD)*speed;
        eProj->get<CAcceleration>()->x = 0;
        eProj->get<CAcceleration>()->y = gravity;

        double t = getTravelTime(x0, 0, x1, 0, eProj->get<CVelocity>()->x);

        eProj->add(new CZoom((180-30)/t, (180-30)/t));
        eProj->add(new CTimer(t, REMOVE_ENTITY));
    }

}

void ScriptedAnimation::popFrontAnimationNode(Entity* e) {
    if (e->get<CActor>()->timeline.empty()) return;
    AnimationNode* a = e->get<CActor>()->timeline.front();
    if (a != nullptr) delete a;
    e->get<CActor>()->timeline.pop_front();
}

void ScriptedAnimation::onAddActor(Entity* e) {
    addActor(e);
}

void ScriptedAnimation::addActor(Entity* e) {
    e->get<CActor>()->acting = true;
    e->get<CActor>()->timer.restart();
    playingScene = true;
    war.setPlayingAnimation(true);
    actors.push_back(e);

}

void ScriptedAnimation::clearScene() {
    actors.clear();
}

void ScriptedAnimation::createFormations() {

    int F1[23][15] = {
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        9,  0, 19,  0, 29,  0, 39,	0, 49,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        7,  0, 17,  0, 27,  0, 37,	0, 47,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        5,  0, 15,  0, 25,  0, 35,	0, 45,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        3,  0, 13,  0, 23,  0, 33,	0, 43,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        1,  0, 11,  0, 21,  0, 31,	0, 41,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        2,  0, 12,  0, 22,  0, 32,	0, 42,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        4,  0, 14,  0, 24,  0, 34,	0, 44,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        6,  0, 16,  0, 26,  0, 36,	0, 46,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        8,  0, 18,  0, 28,  0, 38,	0, 48,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        10,  0, 20,  0, 30,  0, 40,	0, 50,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,	0,  0,  0,  0,  0,  0,  0,  0
    };

    int F2[23][15] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 35,  0, 46,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0, 24,  0, 33,  0, 44,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0, 15,  0, 22,  0, 31,  0, 42,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  8,  0, 13,  0, 20,  0, 29,  0, 40,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  3,  0,  6,  0, 11,  0, 18,  0, 27,  0, 38,  0, 49,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        1,  0,  2,  0,  5,  0, 10,  0, 17,  0, 26,  0, 37,  0, 48,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  4,  0,  7,  0, 12,  0, 19,  0, 28,  0, 39,  0, 50,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  9,  0, 14,  0, 21,  0, 31,  0, 41,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0, 16,  0, 23,  0, 32,  0, 43,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0, 25,  0, 34,  0, 45,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 36,  0, 47,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };

    int F3[23][15] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  32,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  44,  0,  0,  0,  46,  0,  0,  0,  0,  0,
        0,  0,  0,  31,  0,  0,  0,  11, 0,  0,  0,  34,  0,  0,  0,
        0,  0,  0,  0,  0,  24, 0,  0,  0,  25, 0,  0,  0,  0,  0,
        0,  0,  42,  0,  0,  0,  0,  5,  0,  0,  0,  0,  48,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  28,  0,  0,  13, 0,  0,  0,  0,  0,  14, 0,  0,  36,  0,
        0,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0,
        40,  0,  17, 0,  0,  0,  0,  0,  0,  0,  0,  0,  18, 0,  38,
        0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,
        27,  0,  21, 0,  0,  0,  1,  0,  2,  0,  0,  0,  22, 0,  50,
        0,  0,  0,  0,  9,  0,  0,  0,  0,  0,  10, 0,  0,  0,  0,
        41,  0,  19, 0,  0,  0,  0,  0,  0,  0,  0,  0,  20, 0,  39,
        0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,
        0,  29,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  37,  0,
        0,  0,  0,  0,  15, 0,  0,  0,  0,  0,  16, 0,  0,  0,  0,
        0,  0,  43,  0,  0,  0,  0,  6,  0,  0,  0,  0,  49,  0,  0,
        0,  0,  0,  0,  0,  23, 0,  0,  0,  26, 0,  0,  0,  0,  0,
        0,  0,  0,  30,  0,  0,  0,  12, 0,  0,  0,  35,  0,  0,  0,
        0,  0,  0,  0,  0,  45,  0,  0,  0,  47,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  33,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };
    int F4[23][15] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,
        0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,
        0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,
        0,  0,  0,  0,  0,  1,  0,  1,  0,  1,  0,  0,  0,  0,  0,
        1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,
        0,  0,  0,  0,  1,  0,  1,  0,  1,  0,  1,  0,  0,  0,  0,
        1,  0,  1,  0,  0,  0,  0,  1,  0,  0,  0,  0,  1,  0,  1,
        0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0,
        1,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  1,
        0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0,
        0,  1,  0,  1,  0,  0,  1,  0,  1,  0,  0,  1,  0,  1,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  1,  0,  1,  0,  1,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };
    setFormation(CArmy::F1, F1);
    setFormation(CArmy::F2, F2);
    setFormation(CArmy::F3, F3);
    setFormation(CArmy::F4, F4);
}

bool comparePairIntVector(pair<int, sf::Vector2i>& P1, pair<int, sf::Vector2i>& P2) {
    return P1.first < P2.first;
}

void ScriptedAnimation::setFormation(CArmy::Formation formation, int p[23][15]) {
    list< pair<int, sf::Vector2i> > F;

    int l = 23;
    int c = 15;
    for(int i = 0; i < l; i++) {
        for(int j = 0; j < c; j++) {
            if (p[i][j] != 0) {
                F.push_back(make_pair(p[i][j], sf::Vector2i(j, i)));
            }
        }
    }

    F.sort(comparePairIntVector);

    positions[formation].clear();
    for(list< pair<int, sf::Vector2i> >::iterator i = F.begin(); i != F.end(); i++) {
        positions[formation].push_back(i->second);
    }
}

double ScriptedAnimation::getAngleToHit(double ox, double oy, double tx, double ty, double& v, double gravity) {
    double x = (tx - ox);
    double y = -(ty - oy);
    double g = gravity;
    // you don't want the exact vMin because when calculating the angle if the velocity is not enough by an infinitesimal amount it returns nan
    double vMin = 1.05*getMinimumSpeedToHit(ox, oy, tx, ty, g);
    v = max(vMin, v);
    double angle = std::atan2(double(v*v - sqrt(pow(v,4) - g*(g*x*x + 2*y*v*v))), double(g*x));
    return angle*M_DEG;
}

double ScriptedAnimation::getAngleToHit2(double ox, double oy, double tx, double ty, double& v, double gravity) {
    /*
    double x = (tx - ox);
    double y = -(ty - oy);
    double g = gravity;
    double vMin = getMinimumSpeedToHit(ox, oy, tx, ty);
    v = max(vMin, v);
    return atan2(v*v + sqrt(pow(v,4) - g*(g*x*x + 2*y*v*v)),g*x)*M_DEG;
    */
    return 0.f;
}

double ScriptedAnimation::getMinimumSpeedToHit(double ox, double oy, double tx, double ty, double gravity) {
    double x = (tx - ox);
    double y = -(ty - oy);
    double g = gravity;
    return sqrt(g*y + g*sqrt(x*x + y*y));
}

void ScriptedAnimation::scriptArmyVsArmy(ActionOutcome& outcome, Entity* e) {
    Entity* eWin = war.getPlayer(outcome.idWinner);
    Entity* eLos = war.getPlayer(outcome.idLoser);
    ///================
    /// PLAYER WINNER
    ///================

    int sign = eWin->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    double wField = 0.70*wWindow;
    double dx = 2*wField/((3+1)*2);
    double ux = uxFormation;
    double uy = uyFormation;
    double wBattle = 230;
    double hBattle = 230;
    eWin->get<CArmy>()->x -= sign*dx;
    double x0 = eWin->get<CArmy>()->x;
    double y0 = cyWindow - uy*hFormation/2;
    double unitSpeed = 100;
    double tResolution = 4;

    EntityList casualties;
    for(auto& i : outcome.idTargets) {
        casualties.push_back(getUnitByID(eWin, i));
    }

    list<sf::Vector2i> P = positions[eWin->get<CArmy>()->formation];
    for(EntityListIt i = eWin->get<CArmy>()->allUnits.begin(); i != eWin->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, unitSpeed));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                    tResolution,
                    eUnit->get<CUnit>()->aIdle));
        eUnit->get<CActor>()->timeline.push_back(new ATeleport(0.0, randomDouble(cxWindow - wBattle/2, cxWindow + wBattle/2), randomDouble(cyWindow - hBattle/2, cyWindow + hBattle/2)));
        if (contains(casualties, eUnit)) {
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aDead));
            eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));
        }

        addActor(eUnit);
    }
    Entity* eCap = eWin->get<CArmy>()->captain;
    /*
        eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
        eCap->get<CActor>()->timeline.push_back(new AMove(0.0, x0 - sign*50, cyWindow, unitSpeed));
        double t1 = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, x0 - sign*50, cyWindow, unitSpeed);
        eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t1, eCap->get<CCaptain>()->aIdle));
        addActor(eCap);
    */
    ///================
    /// PLAYER LOSER
    ///================
    sign = eLos->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    eLos->get<CArmy>()->x -= sign*dx;
    x0 = eLos->get<CArmy>()->x;

    P = positions[eLos->get<CArmy>()->formation];
    for(EntityListIt i = eLos->get<CArmy>()->allUnits.begin(); i != eLos->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, unitSpeed));
        //printf("%f %f\n", dx, getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 80));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                    tResolution,
                    eUnit->get<CUnit>()->aIdle));
        eUnit->get<CActor>()->timeline.push_back(new ATeleport(0.0, randomDouble(cxWindow - wBattle/2, cxWindow + wBattle/2), randomDouble(cyWindow - hBattle/2, cyWindow + hBattle/2)));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aDead));
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        addActor(eUnit);
    }
    eCap = eLos->get<CArmy>()->captain;

    //double t1 = 3;
    bool hFlip = eLos->get<CPlayer>()->side == CPlayer::LEFT ? true:false;
    eCap->get<CActor>()->timeline.push_back(new ASpeak(tResolution, "Mercy, please!", 5));
    eCap->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, hFlip));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200));
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
    double t = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200);
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t, eCap->get<CCaptain>()->aIdle));
    eCap->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, opposite(hFlip)));

    addActor(eCap);

    ///===========
    /// Blindfold
    ///===========
    scriptFightSmokeCloud(4.5);
    /*
    Entity* eFold = eManager->createEntity();
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(wWindow, hWindow));
    shape.setFillColor(sf::Color(255,255,255));
    eFold->add(new CDraw(CDraw::BLINDFOLD, 0));
    eFold->add(new CRectShape(shape));
    eFold->add(new CPosition(cxWindow, cyWindow));
    eFold->add(new CActor());
    eFold->get<CActor>()->timeline.push_back(new AFade(0.0, 255, 255));
    eFold->get<CActor>()->timeline.push_back(new AFade(3, -200, 0));
    eFold->get<CActor>()->timeline.push_back(new ADestroy(2));

    addActor(eFold);
    */
}

void ScriptedAnimation::scriptFightSmokeCloud(double duration) {
    double wBattle = 310/2;
    double hBattle = 310/2;
    double radius = sqrt(wBattle*wBattle + hBattle*hBattle);
    int nClouds = 250;
    srand(11);
    for (int i = 0; i < nClouds; i++) {
        Entity* eCloud = eManager->createEntity();
        double r = randomDouble(0.0, radius);
        double ang = randomDouble(0.0, 360);
        double x = cxWindow + r*cos(ang*M_RAD);
        double y = cyWindow + r*sin(ang*M_RAD);
        string ani = "poof-05.png";

        eCloud->add(new CPosition(x, y));
        eCloud->add(new CAnimation(false, ani));
        eCloud->add(new CDraw(CDraw::SKY, 0));
        eCloud->add(new CDimensions(130, 120));
        eCloud->add(new CActor());

        eCloud->get<CActor>()->timeline.push_back(new AFade(0.0, 120, 255));
        //eCloud->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, ani));
        eCloud->get<CActor>()->timeline.push_back(new AFade(duration, -120, 0));
        eCloud->get<CActor>()->timeline.push_back(new ADestroy(255.f/120.f));
        eCloud->get<CAnimation>()->update = false;
        eCloud->get<CAnimation>()->frame = randomInt(0, Assets::getAnimation(ani).nFrames-1);
    }
    srand(time(NULL));
}

void ScriptedAnimation::scriptMeleeBattle(Entity* e) {
    ///================
    /// PLAYER WINNER
    ///================
    Entity* eArmy = e->get<CActionOutcome>()->eWinner;

    int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    double wField = 0.70*wWindow;
    double dx = 2*wField/((war.getMatchConfig().nTurns+1)*2);
    double ux = uxFormation;
    double uy = uyFormation;
    double wBattle = 230;
    double hBattle = 230;
    eArmy->get<CArmy>()->x -= sign*dx;
    double x0 = eArmy->get<CArmy>()->x;
    double y0 = cyWindow - uy*hFormation/2;
    double unitSpeed = 100;

    list<sf::Vector2i> P = positions[eArmy->get<CArmy>()->formation];
    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, unitSpeed));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                    0.2+getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, unitSpeed),
                    eUnit->get<CUnit>()->aIdle));
        eUnit->get<CActor>()->timeline.push_back(new ATeleport(0.0, randomDouble(cxWindow - wBattle/2, cxWindow + wBattle/2), randomDouble(cyWindow - hBattle/2, cyWindow + hBattle/2)));
        if (contains(e->get<CActionOutcome>()->targets, eUnit)) {
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aDead));
            eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));
        }

        addActor(eUnit);
    }
    Entity* eCap = eArmy->get<CArmy>()->captain;
    /*
        eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
        eCap->get<CActor>()->timeline.push_back(new AMove(0.0, x0 - sign*50, cyWindow, unitSpeed));
        double t1 = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, x0 - sign*50, cyWindow, unitSpeed);
        eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t1, eCap->get<CCaptain>()->aIdle));
        addActor(eCap);
    */
    ///================
    /// PLAYER LOSER
    ///================
    eArmy = e->get<CActionOutcome>()->eLoser;

    sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    eArmy->get<CArmy>()->x -= sign*dx;
    x0 = eArmy->get<CArmy>()->x;

    P = positions[eArmy->get<CArmy>()->formation];
    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, unitSpeed));
        //printf("%f %f\n", dx, getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 80));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                    0.2+getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, unitSpeed),
                    eUnit->get<CUnit>()->aIdle));
        eUnit->get<CActor>()->timeline.push_back(new ATeleport(0.0, randomDouble(cxWindow - wBattle/2, cxWindow + wBattle/2), randomDouble(cyWindow - hBattle/2, cyWindow + hBattle/2)));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aDead));
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        addActor(eUnit);
    }
    eCap = eArmy->get<CArmy>()->captain;
    /*
    */
    double t1 = 3;
    bool hFlip = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? true:false;
    eCap->get<CActor>()->timeline.push_back(new ASpeak(t1, "Mercy, please!", 5));
    eCap->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, hFlip));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200));
    double t = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200);
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t, eCap->get<CCaptain>()->aIdle));
    eCap->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, opposite(hFlip)));

    addActor(eCap);

    ///===========
    /// Blindfold
    ///===========
    Entity* eFold = eManager->createEntity();
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(wWindow, hWindow));
    shape.setFillColor(sf::Color(255,255,255));
    eFold->add(new CDraw(CDraw::BLINDFOLD, 0));
    eFold->add(new CRectShape(shape));
    eFold->add(new CPosition(cxWindow, cyWindow));
    eFold->add(new CActor());
    eFold->get<CActor>()->timeline.push_back(new AFade(0.0, 255, 255));
    eFold->get<CActor>()->timeline.push_back(new AFade(3, -200, 0));
    eFold->get<CActor>()->timeline.push_back(new ADestroy(2));

    addActor(eFold);
}
void ScriptedAnimation::scriptPresentArmy(Entity* e) {
    Entity* eArmy = e;
    int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    double dx = 550;
    double xTarget = cxWindow + sign*dx;
    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        double unitSpeed = randomDouble(150, 200);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, xTarget, eUnit->get<CPosition>()->y, unitSpeed));
        double t1 = getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, xTarget, eUnit->get<CPosition>()->y, unitSpeed);
        double x1 = randomDouble(xTarget, xTarget - sign*110);
        double y1 = randomDouble(cyWindow - 100, cyWindow + 100);
        eUnit->get<CActor>()->timeline.push_back(new AMove(t1, x1, y1, unitSpeed));

        double t2 = getTravelTime(xTarget, eUnit->get<CPosition>()->y, x1, y1, unitSpeed);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(t2, eUnit->get<CUnit>()->aIdle));

        addActor(eUnit);
    }
}

void ScriptedAnimation::scriptPresentCaptain(Entity* e) {
    Entity* eArmy = e;
    eArmy->get<CArmy>()->captain = eArmy->get<CArmy>()->captains[eArmy->get<CArmy>()->idCaptain];
    int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    double dx = 400;
    double xTarget = cxWindow + sign*dx;

    Entity* eCap = eArmy->get<CArmy>()->captain;
    std::string heroName = eCap->get<CCaptain>()->uniqueName;
    std::string speechText = "SPEECH-PRESENTATION-" + toUpper(heroName) + "-" + int2str(randomInt(1,2), 2);
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, xTarget, eCap->get<CPosition>()->y, 200));
    double t1 = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, xTarget, eCap->get<CPosition>()->y, 200);
    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, Assets::getString(speechText), 3));
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t1, eCap->get<CCaptain>()->aIdle));

    addActor(eCap);

}
void ScriptedAnimation::scriptAdvanceArmy(Entity* e) {
    Entity* eArmy = e;
    int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    double dx = wWalkStep;
    double ux = uxFormation;
    double uy = uyFormation;
    if (eArmy->get<CArmy>()->x == 0) eArmy->get<CArmy>()->x = cxWindow + sign*wWindow/2 -sign*x0Formation + sign*dx;
    eArmy->get<CArmy>()->x -= sign*dx;
    double x0 = eArmy->get<CArmy>()->x;
    double y0 = cyWindow - uy*hFormation/2;

    list<sf::Vector2i> P = positions[eArmy->get<CArmy>()->formation];
    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        sf::Vector2i p = popFront(P);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, x0 + sign*p.x*ux, y0 + p.y*uy, 200));
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(
                    getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, x0 + sign*p.x*ux, y0 + p.y*uy, 200),
                    eUnit->get<CUnit>()->aIdle));
        addActor(eUnit);
    }

    Entity* eCap = eArmy->get<CArmy>()->captain;
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, x0 - sign*50, cyWindow, 200));
    double t1 = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, x0 - sign*50, cyWindow, 200);
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t1, eCap->get<CCaptain>()->aIdle));

    addActor(eCap);
}

void ScriptedAnimation::scriptReturnArmy(Entity* e) {
    Entity* eArmy = e;
    int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    double wField = 0.70*wWindow;
    double dx = wField/((war.getMatchConfig().nTurns+1)*2);
    //double ux = 32;
    //double uy = 26;
    eArmy->get<CArmy>()->x -= sign*dx;
    //double x0 = eArmy->get<CArmy>()->x;
    //double y0 = cyWindow - uy*12/2;

    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(1.f, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, opposite(eUnit->get<CAnimation>()->hFlip)));
        double yDest = cyWindow + randomDouble(-15,15);
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*400, yDest, 200));
        double t = getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, cxWindow + sign*400, yDest, 200);
        eUnit->get<CActor>()->timeline.push_back(new AMove(t, cxWindow + sign*wWindow/2 + sign*100, yDest, 200));
        t = getTravelTime(cxWindow + sign*400, cyWindow, cxWindow + sign*wWindow/2 + sign*100, yDest, 200);
        eUnit->get<CActor>()->timeline.push_back(new AVariable(t, AVariable::REMOVE_FROM_ARMY));
        eUnit->get<CActor>()->timeline.push_back(new ADestroy(0));

        //addActor(eUnit);
    }

    Entity* eCap = eArmy->get<CArmy>()->captain;

    /*
        if (eArmy->get<CArmy>()->win){
            string speech = "SPEECH-VICTORY-" + int2str(randomInt(1,5));
            eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, Assets::getString(speech), 4));
        }else{
            string speech = "SPEECH-DEFEATED-" + int2str(randomInt(1,5));
            eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, Assets::getString(speech), 4));
        }
      */
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
    eCap->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, sign == -1 ? true:false));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200));
    double t = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->x, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200);
    eCap->get<CActor>()->timeline.push_back(new AVariable(t, AVariable::H_FLIP, sign == -1 ? false:true));

    //addActor(eCap);
}

void ScriptedAnimation::scriptBattleCleanup() {
    Entity* eWinner = war.getPlayer(war.getBattleWinner());
    Entity* eLoser = war.getPlayer(war.getBattleLoser());
    War::BattleClosure closure = war.getBattleClosure();

    if (closure == war.ARMY_VS_ARMY) {
        scriptReturnArmy(eWinner);
    } else if (closure == war.MAN_VS_MAN) {
        scriptReturnArmy(eWinner);
        scriptReturnArmy(eLoser);
    } else if (closure == war.MERCY) {
        scriptReturnArmy(eWinner);
        scriptReturnArmy(eLoser);
    } else if (closure == war.ALL_KILLED) {
        scriptReturnArmy(eWinner);
        scriptReturnArmy(eLoser);
    }

    if (!config.getDeadBodies()) {
        scriptRemoveDead(war.getPlayer(1));
        scriptRemoveDead(war.getPlayer(2));
    }

    eWinner->get<CArmy>()->allUnits.clear();
    eLoser->get<CArmy>()->allUnits.clear();
}

void ScriptedAnimation::scriptRemoveDead(Entity* e) {
    Entity* eArmy = e;
    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (!eUnit->get<CUnit>()->dead) continue;
        eUnit->get<CActor>()->timeline.push_back(new AFade(0, -255, 0));
        eUnit->get<CActor>()->timeline.push_back(new AVariable(1, AVariable::REMOVE_FROM_ARMY));
        eUnit->get<CActor>()->timeline.push_back(new ADestroy(0));
        addActor(eUnit);
    }
}

void ScriptedAnimation::scriptManVsMan(ActionOutcome& outcome, Entity* eActor) {
    double unitSpeed = 100;
    ///===========
    /// Fighters
    ///===========
    Entity* P1fighter = getUnitByID(war.getPlayer(1), outcome.idActors.front());
    Entity* P2fighter = getUnitByID(war.getPlayer(2), outcome.idActors.back());
    EntityList fightActors;
    fightActors.push_back(P1fighter);
    fightActors.push_back(P2fighter);
    for(EntityListIt i = fightActors.begin(); i != fightActors.end(); i++) {
        Entity* eUnit = *i;
        Entity* eArmy = eUnit->get<COwner>()->e;
        int sign = eArmy->get<CArmy>()->side == CPlayer::LEFT ? -1:1;
        double xTarget = cxWindow + sign*32;

        string speech = "SPEECH-VOLUNTEER-" + int2str(randomInt(1,5));
        eUnit->get<CActor>()->timeline.push_back(new ASpeak(0.0, Assets::getString(speech), 2));

        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(2.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, xTarget, cyWindow, unitSpeed));
        double t = getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, xTarget, cyWindow, unitSpeed);
        eUnit->get<CActor>()->timeline.push_back(new AMove(t, xTarget - sign*16, cyWindow, unitSpeed));
        t = getTravelTime(xTarget, cyWindow, xTarget - sign*16, cyWindow, unitSpeed);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(t, eUnit->get<CUnit>()->aIdle));

        if (eArmy->get<CPlayer>()->id == outcome.idLoser) {
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aDead));
            eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));
            eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::WIN, false));
        } else {
            eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::WIN, true));
        }

        addActor(eUnit);
    }

    Entity* eLoser = war.getPlayer(outcome.idLoser);
    Entity* eLoserFighter = outcome.idLoser == 1 ? P1fighter : P2fighter;
    eLoser->get<CArmy>()->prohibitedDamageType = eLoserFighter->get<CUnit>()->damage;

    ///===========
    /// Blindfold
    ///===========
    Entity* eFold = eManager->createEntity();
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(wWindow, hWindow));
    shape.setFillColor(sf::Color(255,255,255));
    eFold->add(new CDraw(CDraw::BLINDFOLD, 0));
    eFold->add(new CRectShape(shape));
    eFold->add(new CPosition(cxWindow, cyWindow));
    eFold->add(new CActor());
    eFold->get<CActor>()->timeline.push_back(new AFade(2.0, 255, 255));
    eFold->get<CActor>()->timeline.push_back(new AFade(3, -200, 0));
    eFold->get<CActor>()->timeline.push_back(new ADestroy(2));

    addActor(eFold);
}

void ScriptedAnimation::scriptIntimidate(ActionOutcome& outcome, Entity* eActor) {
    Entity* eArmy = eActor;
    Entity* eEnemy = eActor->get<CPlayer>()->enemy;
    int sign = eEnemy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    //double wField = 0.70*wWindow;
    //double dx = wField/((war.getMatchConfig().nTurns+1)*2);
    //double ux = 32;
    //double uy = 26;
    //double x0 = eArmy->get<CArmy>()->x;
    //double y0 = cyWindow - uy*12/2;

    int speechs = 0;
    int nSpeechs = 1;
    int nSuccesses = outcome.idTargets.size();

    for (auto& i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(eEnemy, i);
        double xUnit = eUnit->get<CPosition>()->x;
        double yUnit = eUnit->get<CPosition>()->y;
        if (speechs++ < nSpeechs) {
            eUnit->get<CActor>()->timeline.push_back(new ASpeak(intimSpeechDur, Assets::getString("SPEECH-INTIMIDATED-" + int2str(randomInt(1, 5))), 2));
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        } else {
            eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(intimSpeechDur, eUnit->get<CUnit>()->aWalk));
        }
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, opposite(eUnit->get<CAnimation>()->hFlip)));
        double yDest = cyWindow + randomDouble(-15,15);
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*wWindow/2 + sign*100, yDest, 200));
        double t = getTravelTime(xUnit, yUnit, cxWindow + sign*wWindow/2 + sign*100, yDest, 200);
        eUnit->get<CActor>()->timeline.push_back(new AVariable(t, AVariable::REMOVE_FROM_ARMY));
        eUnit->get<CActor>()->timeline.push_back(new ADestroy(0.f));

        addActor(eUnit);
    }

    Entity* eCap = eArmy->get<CArmy>()->captain;
    std::string heroName = eCap->get<CCaptain>()->uniqueName;
    std::string speechText = "SPEECH-INTIMIDATION-" + toUpper(heroName) + "-" + int2str(randomInt(1,2), 2);
    eCap->get<CActor>()->timeline.push_back(new ASpeak(0.0, Assets::getString(speechText), 3));
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aIntimidation));
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eCap->get<CCaptain>()->aIntimidation).duration, eCap->get<CCaptain>()->aIdle));

    Entity* eCapEnemy = eEnemy->get<CArmy>()->captain;
    if (nSuccesses > 0) {
        std::string capName = toUpper(eCapEnemy->get<CCaptain>()->uniqueName);
        std::string speech = Assets::getString("SPEECH-AFTER-DEFENSE-" + capName + "-EFFECTIVE-" + int2str(randomInt(1, 1), 2));
        eCapEnemy->get<CActor>()->addNode(new ASpeak(3.0f, speech, 2.5f));
    } else {
        std::string capName = toUpper(eCapEnemy->get<CCaptain>()->uniqueName);
        std::string speech = Assets::getString("SPEECH-AFTER-DEFENSE-" + capName + "-NOT-EFFECTIVE-" + int2str(randomInt(1, 1), 2));
        eCapEnemy->get<CActor>()->addNode(new ASpeak(3.0f, speech, 2.5f));
    }

    addActor(eCap);
}

void ScriptedAnimation::scriptMetheore(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///SOUND ANNOUNCEMENT
    Entity* eSound = eManager->createEntity();
    eSound->add(new CActor());
    eSound->get<CActor>()->timeline.push_back(new ASound(unitRest1-1, "sfx-rain-announcement.wav"));
    eSound->get<CActor>()->timeline.push_back(new ADestroy(0.0));

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        //eAtk->get<CActor>()->timeline.push_back(new ASound(0.0, "rollover5.wav"));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(unitRest1, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration, eAtk->get<CUnit>()->aIdle));

        ///METHEORE
        double speed = randomDouble(180, 500);
        double cathetus = eDef->get<CPosition>()->y + 60;
        double xMeth = eDef->get<CPosition>()->x + sign*cathetus;
        double yMeth = eDef->get<CPosition>()->y - cathetus;
        Entity* eMeth = eManager->createEntity();
        eMeth->add(new CAnimation(sign == -1 ? false:true, "comet.png"));
        eMeth->add(new CPosition(xMeth, yMeth));
        eMeth->add(new CVelocity());
        eMeth->add(new CActor());
        eMeth->add(new CDraw(CDraw::SKY));

        double travelTime = getTravelTime(eMeth->get<CPosition>()->x, eMeth->get<CPosition>()->y,
                                          eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, speed);

        eMeth->get<CActor>()->timeline.push_back(new AMove(unitRest1, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, speed));
        eMeth->get<CActor>()->timeline.push_back(new ADestroy(travelTime));

        ///SHAKE TIMER
        Entity* eTimer = eManager->createEntity();
        eTimer->add(new CTimer(unitRest1+travelTime, SHAKE_CAMERA, true));
        eTimer->add(new CCameraShake(0.5, 4));

        ///FIRE
        /*
        Entity* eEffect = eManager->createEntity();
        sf::CircleShape shape;
        shape.setRadius(4);
        shape.setFillColor(sf::Color::Red);
        eEffect->add(new CParticleEmmiter(10, shape, 0.5, 30, 30, -90, 0, 1));
        eEffect->add(new CAnchor(eMeth, 12, 0));
        eEffect->add(new CPosition(0,0));
        eEffect->get<CParticleEmmiter>()->on = true;
        */
        ///EXPLOSION
        Entity* eExp = eManager->createEntity();
        string animation = "explosion-01.png";
        string explosionSound = "sfx-explosion-07-01.wav";
        eExp->add(new CAnimation(false, animation));
        eExp->add(new CPosition(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y));
        eExp->add(new CActor());
        eExp->add(new CDimensions(40,40));
        eExp->add(new CDraw(CDraw::WORLD_2));
        eExp->get<CDraw>()->isHidden = true;

        eExp->get<CActor>()->timeline.push_back(new AVariable(unitRest1+travelTime, AVariable::HIDDEN, false));
        eExp->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
        eExp->get<CActor>()->timeline.push_back(new ASound(0.0, explosionSound));
        eExp->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));

        ///DEFENDER
        double tStart = unitRest1;
        double t0 = 0.0;
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tStart + travelTime + t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], tStart + travelTime + t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], tStart + travelTime + t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tStart + travelTime + t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

}

void ScriptedAnimation::scriptIceDragons(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;
    map<Entity*, double> tResolve;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    bool hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;

    double xOff = wWindow;
    double yOff = 200;
    for(Entity* eDef : eTargets) {
        xOff = max(abs(cxWindow - eDef->get<CPosition>()->x), xOff);
    }
    xOff = wWindow/2 + xOff;

    for(Entity* eDef : eTargets) {
        tResolve.insert(make_pair(eDef, 0.0));
        Entity* eDragon = eManager->createEntity();
        double tStart = randomDouble(0.0, 2);
        double yDragon = eDef->get<CPosition>()->y - yOff;
        double speed1 = 300;
        double speed2 = 500;
        double speed3 = 400;
        double tAux;

        eDragon->add(new CPosition(eDef->get<CPosition>()->x + sign*xOff, yDragon));
        eDragon->add(new CAnimation(hFlip, "ice-dragon-flying.png"));
        eDragon->add(new CDraw(CDraw::WORLD_2));
        eDragon->add(new CActor());
        eDragon->add(new CVelocity());
        eDragon->get<CActor>()->addNode(new ASpriteAnimation(tStart, "ice-dragon-flying.png"));
        eDragon->get<CActor>()->addNode(new AMove(0.0, eDef->get<CPosition>()->x + sign*yOff, yDragon, speed1));
        tAux = getTravelTime(eDef->get<CPosition>()->x + sign*xOff, yDragon,
                             eDef->get<CPosition>()->x + sign*yOff, yDragon, speed1);
        tResolve[eDef] += tAux;
        eDragon->get<CActor>()->addNode(new ASpriteAnimation(tAux, "ice-dragon-diving.png"));
        eDragon->get<CActor>()->addNode(new AMove(0.0, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, speed2));
        tAux = getTravelTime(eDef->get<CPosition>()->x + sign*yOff, yDragon,
                             eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, speed2);
        tResolve[eDef] += tAux;
        eDragon->get<CActor>()->addNode(new ASpriteAnimation(tAux, "ice-dragon-rolling.png"));
        eDragon->get<CActor>()->addNode(new AMove(0.0, cxWindow - sign*wWindow/2, eDef->get<CPosition>()->y, speed3));
        tAux = getTravelTime(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y,
                             cxWindow - sign*wWindow/2, eDef->get<CPosition>()->y, speed3);
        eDragon->get<CActor>()->addNode(new ADestroy(tAux));

        tResolve[eDef] += tStart;

        addActor(eDragon);
    }

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        //eAtk->get<CActor>()->timeline.push_back(new ASound(0.0, "rollover5.wav"));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(unitRest1, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration, eAtk->get<CUnit>()->aIdle));

        ///METHEORE
        /*
        double speed = randomDouble(180, 500);
        double cathetus = eDef->get<CPosition>()->y + 60;
        double xMeth = eDef->get<CPosition>()->x + sign*cathetus;
        double yMeth = eDef->get<CPosition>()->y - cathetus;
        Entity* eMeth = eManager->createEntity();
        eMeth->add(new CAnimation(sign == -1 ? false:true, "comet.png"));
        eMeth->add(new CPosition(xMeth, yMeth));
        eMeth->add(new CVelocity());
        eMeth->add(new CActor());
        eMeth->add(new CDraw(CDraw::SKY));

        double travelTime = getTravelTime(eMeth->get<CPosition>()->x, eMeth->get<CPosition>()->y,
                                          eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, speed);

        eMeth->get<CActor>()->timeline.push_back(new AMove(unitRest1, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, speed));
        eMeth->get<CActor>()->timeline.push_back(new ADestroy(travelTime));

        ///SHAKE TIMER
        Entity* eTimer = eManager->createEntity();
        eTimer->add(new CTimer(unitRest1+travelTime, SHAKE_CAMERA, true));
        eTimer->add(new CCameraShake(0.5, 4));

        ///EXPLOSION
        Entity* eExp = eManager->createEntity();
        string animation = "explosion-01.png";
        string explosionSound = "sfx-explosion-07-01.wav";
        eExp->add(new CAnimation(false, animation));
        eExp->add(new CPosition(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y));
        eExp->add(new CActor());
        eExp->add(new CDimensions(40,40));
        eExp->add(new CDraw(CDraw::WORLD_2));
        eExp->get<CDraw>()->isHidden = true;

        eExp->get<CActor>()->timeline.push_back(new AVariable(unitRest1+travelTime, AVariable::HIDDEN, false));
        eExp->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
        eExp->get<CActor>()->timeline.push_back(new ASound(0.0, explosionSound));
        eExp->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));
        */
        ///DEFENDER
        //double tStart = unitRest1;
        //double t0 = 0.0;
        //double travelTime = 0.0;
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tResolve[eDef]));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = tResolve[eDef];
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = tResolve[eDef];
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tResolve[eDef]));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

}

void ScriptedAnimation::scriptStalactite(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        double tStart = unitRest1 + randomDouble(0.0, 2);
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart, eAtk->get<CUnit>()->aAction02));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction02).duration, eAtk->get<CUnit>()->aIdle));

        ///STALAGMITE
        double wObj = 50;
        double hObj = 100;
        double t0 = tStart + Assets::getAnimation(eAtk->get<CUnit>()->aAction02).duration;
        Entity* eObj = eManager->createEntity();
        eObj->add(new CPosition(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y - hObj/2 + 33/2 + 10));
        eObj->add(new CDimensions(wObj, hObj));
        eObj->add(new CDraw(CDraw::WORLD_2));
        eObj->add(new CAnimation(false, ""));
        eObj->add(new CActor());
        eObj->get<CDraw>()->isHidden = true;
        eObj->get<CActor>()->addNode(new AVariable(tStart + Assets::getAnimation(eAtk->get<CUnit>()->aAction02).duration, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, "sfx-drill-02-02.wav"));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, "stalagmite-in.png"));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(Assets::getAnimation("stalagmite-in.png").duration, "stalagmite-still.png"));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.2, "stalagmite-out.png"));
        eObj->get<CActor>()->addNode(new ADestroy(Assets::getAnimation("stalagmite-out.png").duration));

        addActor(eObj);

        ///SHAKE TIMER
        /*
        Entity* eTimer = eManager->createEntity();
        eTimer->add(new CTimer(tShake, SHAKE_CAMERA, true));
        eTimer->add(new CCameraShake(shakeDuration, 8));

        ttShake = tShake;
        */
        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
    /*
        ///SOUNDS
        Entity* eSound = eManager->createEntity();
        eSound->add(new CActor());
        eSound->get<CActor>()->addNode(new ASound(unitRest1, "sfx-mega-jump.wav"));
        eSound->get<CActor>()->addNode(new ASound(ttShake - unitRest1, "sfx-rain-announcement.wav"));

        ///SHAKE
        Entity* eTimer = eManager->createEntity();
        eTimer->add(new CTimer(ttShake, SHAKE_CAMERA, true));
        eTimer->add(new CCameraShake(shakeDuration, 8));
    */
}

void ScriptedAnimation::scriptEarthquake(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    double ttShake = 0;
    double shakeDuration = 2;

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        double tStart = unitRest1;
        double yOff = 31;
        double speed1 = 32;
        double speed2 = 100;
        //double t0 = Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration;
        double tAux = 0;
        double tShake = tStart;
        eAtk->get<CActor>()->timeline.push_back(new AVariable(tStart, AVariable::DRAW_TAG, CDraw::WORLD_2));
        eAtk->get<CActor>()->timeline.push_back(new AMove(0.0, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y - yOff, speed1));
        tAux = getTravelTime(0, 0, 0, yOff, speed1);
        tShake += tAux;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration, eAtk->get<CUnit>()->aAction02));
        tAux -= Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tAux, eAtk->get<CUnit>()->aAction03));
        eAtk->get<CActor>()->timeline.push_back(new AMove(0.0, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, speed2));
        tAux = getTravelTime(0, 0, 0, yOff, speed2);
        eAtk->get<CActor>()->timeline.push_back(new AVariable(tAux, AVariable::DRAW_TAG, CDraw::WORLD_1));
        tShake += tAux;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(shakeDuration, eAtk->get<CUnit>()->aIdle));

        ///SHAKE TIMER
        Entity* eTimer = eManager->createEntity();
        eTimer->add(new CTimer(tShake, SHAKE_CAMERA, true));
        eTimer->add(new CCameraShake(shakeDuration, 8));

        ttShake = tShake;

        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tShake + randomDouble(0,shakeDuration)));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], tShake + randomDouble(0,shakeDuration));
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], tShake + randomDouble(0,shakeDuration));
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tShake + randomDouble(0,shakeDuration)));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    ///SOUNDS
    Entity* eSound = eManager->createEntity();
    eSound->add(new CActor());
    eSound->get<CActor>()->addNode(new ASound(unitRest1, "sfx-mega-jump.wav"));
    eSound->get<CActor>()->addNode(new ASound(ttShake - unitRest1, "sfx-rain-announcement.wav"));

    ///SHAKE
    Entity* eTimer = eManager->createEntity();
    eTimer->add(new CTimer(ttShake, SHAKE_CAMERA, true));
    eTimer->add(new CCameraShake(shakeDuration, 8));

}

void ScriptedAnimation::scriptBubbles(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;
    map<Entity*, double> tResolve;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    for (auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        tResolve.insert(make_pair(eDef, 0.0));
        double tStart = randomDouble(0.0, 1.0);
        double tTotal1, tTotal2, tTotal3;

        Entity* eBubble = eManager->createEntity();
        eBubble->add(new CPosition());
        eBubble->add(new CAnchor());
        eBubble->addObservedEntity("Anchor", eDef);
        eBubble->attachEmployer(eDef);
        eBubble->add(new CAnimation(false, "bubble.png"));
        eBubble->add(new CActor());
        eBubble->add(new CDraw(CDraw::WORLD_3));
        eBubble->get<CDraw>()->isHidden = true;
        eBubble->get<CActor>()->addNode(new AVariable(tStart, AVariable::HIDDEN, false));

        ///DEFENDER
        double ySpeed = 50;
        double xSpeed = 20;
        double speed = sqrt(xSpeed*xSpeed + ySpeed*ySpeed);
        double dropHeight = 200;
        double ascendTime = dropHeight/ySpeed;
        double xAmp = 4;
        double t0Aux = tStart;
        double ascensionTravelTime = dropHeight/ySpeed;
        tTotal1 = 0;
        int xSign = -1;
        double x0 = eDef->get<CPosition>()->x;
        double y0 = eDef->get<CPosition>()->y;
        eDef->get<CActor>()->addNode(new AVariable(tStart, AVariable::DRAW_TAG, CDraw::WORLD_2));
        t0Aux = 0.f;
        while (tTotal1 <= ascendTime) {
            double tTravel = getTravelTime(x0, 0, x0+xSign*xAmp, 0, xSpeed);
            eDef->get<CActor>()->addNode(new AMove(t0Aux, x0 + xSign*xAmp, y0 - tTravel*ySpeed, speed));
            t0Aux = tTravel;
            tTotal1 += tTravel;
            x0 = x0 + xSign*xAmp;
            y0 = y0 - tTravel*ySpeed;
            xSign *= -1;
        }

        double delay = 1.0;
        double popTime = tStart + ascensionTravelTime + delay;
        tTotal2 = 0.0;
        while (tTotal2 <= delay) {
            double tTravel = getTravelTime(x0, 0, x0+xSign*xAmp, 0, xSpeed);
            eDef->get<CActor>()->addNode(new AMove(t0Aux, x0 + xSign*xAmp, y0, xSpeed));
            t0Aux = tTravel;
            tTotal2 += tTravel;
            x0 = x0 + xSign*xAmp;
            xSign *= -1;
        }
        eDef->get<CActor>()->addNode(new AMove(t0Aux, eDef->get<CPosition>()->x, y0, xSpeed));
        t0Aux = getTravelTime(x0, 0, eDef->get<CPosition>()->x, 0, xSpeed);
        tTotal2 += t0Aux;
        eBubble->get<CActor>()->addNode(new ADestroy(tTotal1+tTotal2));

        double descentSpeed = 500;
        tTotal3 = 0.0;
        eDef->get<CActor>()->addNode(new AMove(t0Aux, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, descentSpeed));
        tTotal3 = getTravelTime(0.0, y0, 0.0, eDef->get<CPosition>()->y, descentSpeed);
        eDef->get<CActor>()->addNode(new ASpriteAnimation(tTotal3, eDef->get<CUnit>()->aIdle));
        eDef->get<CActor>()->addNode(new AVariable(0.0, AVariable::DRAW_TAG, CDraw::WORLD_1));

        tResolve[eDef] = tTotal1+tTotal2+tTotal3+tStart;

        ///SHADOW
        Entity* eShadow = eManager->createEntity();
        eShadow->add(new CPosition(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y));
        eShadow->add(new CTexture("small-shadow.png"));
        eShadow->add(new CDraw(CDraw::WORLD));
        eShadow->add(new CActor());
        eShadow->get<CDraw>()->isHidden = true;
        eShadow->get<CActor>()->addNode(new AVariable(tStart, AVariable::HIDDEN, false));
        eShadow->get<CActor>()->addNode(new ADestroy(tTotal1+tTotal2+tTotal3));

        // Dart
        double dartSpeed = 400.f;
        double gravity = 100.f;
        double shotAngle = getAngleToHit(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y - dropHeight, dartSpeed, gravity);
        double xDartSpeed = cos(shotAngle*M_RAD) * dartSpeed;
        double dartTravelDuration = getTravelTime(eAtk->get<CPosition>()->x, 0, eDef->get<CPosition>()->x, 0, xDartSpeed);
        double shotDate = popTime - dartTravelDuration;

        Entity* eObj = eManager->createEntity();
        eObj->add(new CPosition(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y));
        eObj->add(new CDraw(CDraw::SKY));
        eObj->add(new CTexture("dart.png"));
        eObj->add(new CRotation(true));
        eObj->add(new CVelocity());
        eObj->add(new CAcceleration());
        eObj->add(new CActor());

        eObj->get<CDraw>()->isHidden = true;
        eObj->get<CActor>()->addNode(new AVariable(shotDate, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::AUTO_P, true));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_ACC, 0.0));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_ACC, gravity));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_VEL, cos(shotAngle*M_RAD)*dartSpeed));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_VEL, -sin(shotAngle*M_RAD)*dartSpeed));
        eObj->get<CActor>()->addNode(new ASound(dartTravelDuration, "sfx-pop-bubble-01.wav"));
        eObj->get<CActor>()->addNode(new ADestroy(0.f));

        //Attacker animation
        double durAttackAnimation = Assets::getAnimation("unit-08-action-01.png").duration;
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(shotDate - durAttackAnimation, "unit-08-action-01.png"));
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(durAttackAnimation, "unit-08-idle.png"));
        eAtk->get<CActor>()->addNode(new ASound(0.0, "sfx-arrow-04.wav"));

        addActor(eObj);
    }

    for(auto& out : outcome.unitActionOutcomes) {
        //Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tResolve[eDef]));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = tResolve[eDef];
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = tResolve[eDef];
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tResolve[eDef]));
                blocked[eDef] = true;
            }
        }

        //addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
}

void ScriptedAnimation::scriptTelekinesis(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;
    map<Entity*, double> tResolve;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    for(Entity* eDef : eTargets) {
        tResolve.insert(make_pair(eDef, 0.0));
        double tStart = randomDouble(0.0, 1.0);
        //double tTotal1, tTotal2, tTotal3;

        double ascendingSpeed = 50;
        double descendingSpeed = 300;
        double dy = 15;
        double dropHeight = 150;
        double rotationVel = randomDouble(360, 600);
        double tAux;
        double tAir = tStart;

        if (!eDef->has(Component::ROTATION)) eDef->add(new CRotation(0.0));

        eDef->get<CActor>()->addNode(new AVariable(0.0, AVariable::AUTO_P, true));
        eDef->get<CActor>()->addNode(new AVariable(0.0, AVariable::ROTATION_VEL, rotationVel));
        eDef->get<CActor>()->addNode(new AVariable(0.0, AVariable::DRAW_TAG, (int) CDraw::WORLD_3));
        eDef->get<CActor>()->addNode(new AMove(tStart, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y - dropHeight, ascendingSpeed));
        tAux = dropHeight/ascendingSpeed;
        tAir += tAux;
        eDef->get<CActor>()->addNode(new AMove(tAux, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y - dropHeight + dy, ascendingSpeed));
        tAux = dy/ascendingSpeed;
        tAir += 4*tAux;
        eDef->get<CActor>()->addNode(new AMove(tAux, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y - dropHeight, ascendingSpeed));
        eDef->get<CActor>()->addNode(new AMove(tAux, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y - dropHeight + dy, ascendingSpeed));
        eDef->get<CActor>()->addNode(new AMove(tAux, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y - dropHeight, ascendingSpeed));
        eDef->get<CActor>()->addNode(new AMove(tAux, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, descendingSpeed));
        tAux = dropHeight/descendingSpeed;
        tAir += tAux;
        eDef->get<CActor>()->addNode(new AVariable(tAux, AVariable::AUTO_P, false));
        eDef->get<CActor>()->addNode(new AVariable(0.0, AVariable::ROTATION_VEL, 0.0));
        eDef->get<CActor>()->addNode(new AVariable(0.0, AVariable::ANGLE, 0.0));
        eDef->get<CActor>()->addNode(new AVariable(0.0, AVariable::DRAW_TAG, (int) CDraw::WORLD_1));

        tResolve[eDef] = tAir;

        ///SHADOW
        Entity* eShadow = eManager->createEntity();
        eShadow->add(new CPosition(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y));
        eShadow->add(new CTexture("small-shadow.png"));
        eShadow->add(new CDraw(CDraw::WORLD));
        eShadow->add(new CActor());
        eShadow->get<CDraw>()->isHidden = true;
        eShadow->get<CActor>()->addNode(new AVariable(tStart, AVariable::HIDDEN, false));
        eShadow->get<CActor>()->addNode(new ADestroy(tAir));
    }

    for(auto& out : outcome.unitActionOutcomes) {
        //Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tResolve[eDef]));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = tResolve[eDef];
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = tResolve[eDef];
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tResolve[eDef]));
                blocked[eDef] = true;
            }
        }

        //addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
}

void ScriptedAnimation::scriptFlamingArrows(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        double tStart = unitRest1 + randomDouble(0, 0.4);
        double t0 = Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new AShoot(t0, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y));
        eAtk->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-arrow-04.wav"));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction02));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction02).duration, eAtk->get<CUnit>()->aIdle));

        double projSpeed = 300;
        double angle = getAngleToHit(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, projSpeed);
        double travelTime = getTravelTime(eAtk->get<CPosition>()->x, 0, eDef->get<CPosition>()->x, 0, projSpeed*cos(angle*M_RAD));

        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tStart + travelTime + t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], tStart + travelTime + t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], tStart + travelTime + t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tStart + travelTime + t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
}

void ScriptedAnimation::scriptMakeItRain(ActionOutcome& outcome, Entity* e) {
    Entity* eArmy = e;
    Entity* eEnemy = eArmy->get<CPlayer>()->enemy;
    int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    Entity* eObj;
    double cxArmy = eEnemy->get<CArmy>()->x - sign*wFormation*uxFormation/2;

    ///CLOUD
    double yCloud = cyWindow - hFormation*uyFormation/2 - Assets::getTexture("cloud.png")->getSize().y/2;
    double xCloud = cxArmy + sign*500;
    double wCloud = Assets::getTexture("cloud.png")->getSize().x;
    Entity* eCloud = eManager->createEntity();
    eCloud->add(new CPosition(xCloud, yCloud));
    eCloud->add(new CTexture("cloud.png"));
    eCloud->add(new CActor());
    eCloud->add(new CDraw(CDraw::CLOUDS, 0));
    eCloud->add(new CVelocity());
    eCloud->add(new CParticleEffect());
    eCloud->add(new CElipsoidalMovement(0, yCloud, 0, 3, 180, 0, false, true));

    double xTarget = cxWindow - sign*wWindow/2 - sign * wCloud/2;
    eCloud->get<CActor>()->timeline.push_back(new AFade(0.0, 120, 255));
    eCloud->get<CActor>()->timeline.push_back(new AMove(0.0, cxArmy, yCloud, 100));
    eCloud->get<CActor>()->timeline.push_back(new AVariable(255.f/120.f, AVariable::PARTICLE_EFFECT, true));
    eCloud->get<CActor>()->timeline.push_back(new ASound(getTravelTime(xCloud, 0, cxArmy, 0, 100) - 255.f/120.f, "thunder-01.wav"));
    eCloud->get<CActor>()->timeline.push_back(new AMove(0.0, xTarget, yCloud, 100));
    eCloud->get<CActor>()->timeline.push_back(new ADestroy(getTravelTime(cxArmy, 0, xTarget, 0, 100)));
    addActor(eCloud);

    ///SOUNDS
    eObj = eManager->createEntity();
    eObj->add(new CSound("sfx-rain.wav", CSound::LOOP_THEN_REMOVE, 4, 2, 8.2));
    notify(PLAY_SOUND, eObj);

    eObj = eManager->createEntity();
    eObj->add(new CSound("sfx-rain-announcement.wav", CSound::REMOVE_ENTITY));
    notify(PLAY_SOUND, eObj);

    ///LIGHTNING
    Entity* eLight = eManager->createEntity();
    eLight->add(new CPosition(cxWindow, cyWindow));
    eLight->add(new CRectShape(wWindow, hWindow, sf::Color::White));
    eLight->add(new CDraw(CDraw::BLINDFOLD, 0));
    eLight->add(new CActor());
    eLight->get<CActor>()->timeline.push_back(new AVariable(getTravelTime(xCloud, 0, cxArmy, 0, 100), AVariable::ALPHA, 255));
    eLight->get<CActor>()->timeline.push_back(new AFade(0.0, -255, 0));
    eLight->get<CActor>()->timeline.push_back(new ADestroy(1.0));

    ///DROP EMMITERS
    double nEmmiters = 20;
    double space = 0.8*wCloud/nEmmiters;
    double xOff = -0.8*wCloud/2;
    for(int i = 0; i < nEmmiters; i++) {
        Entity* eEmmiter = eManager->createEntity();
        eEmmiter->add(new CPosition());
        eEmmiter->add(new CAnchor(xOff, 0));
        eEmmiter->addObservedEntity("Anchor", eCloud);
        eEmmiter->attachEmployer(eCloud);
        eEmmiter->add(new CParticleEmmiter(6, "raindrop.png", 1.0, 300, 350, 90, 0));
        xOff += space;
        eCloud->get<CParticleEffect>()->emmiters.push_back(eEmmiter);
    }

    ///DARKNESS
    Entity* eDark = eManager->createEntity();
    eDark->add(new CPosition(cxWindow, cyWindow));
    eDark->add(new CRectShape(wWindow, hWindow, sf::Color::Black));
    eDark->add(new CActor());
    eDark->add(new CDraw(CDraw::BLINDFOLD, 0));
    eDark->add(new CFade());
    eDark->get<CActor>()->timeline.push_back(new AFade(0.0, 255, 75));
    eDark->get<CActor>()->timeline.push_back(new AFade(getTravelTime(xCloud, 0, xTarget, 0, 100), -255, 0));
    eDark->get<CActor>()->timeline.push_back(new ADestroy(1.0));
    addActor(eDark);


    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        eAtk->get<CActor>()->timeline.push_back(new ASound(0.0, "rollover5.wav"));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration, eAtk->get<CUnit>()->aIdle));


        ///DEFENDER
        /*
        if (it->id == TargetOutcome::DIED){
            eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(getTravelTime(xCloud, 0, cxArmy, 0, 100), eDef->get<CUnit>()->aDeath));
            eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
            eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));
        }else{
            eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(getTravelTime(xCloud, 0, cxArmy, 0, 100), eDef->get<CUnit>()->aIdle));
        }
        */

        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, getTravelTime(xCloud, 0, cxArmy, 0, 100)));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], getTravelTime(xCloud, 0, cxArmy, 0, 100));
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], getTravelTime(xCloud, 0, cxArmy, 0, 100));
            } else {
                whoBlockedWhen.insert(make_pair(eDef, getTravelTime(xCloud, 0, cxArmy, 0, 100)));
                blocked[eDef] = true;
            }
        }

        //addActor(eDef);
        //addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
}

void ScriptedAnimation::scriptTornado(ActionOutcome& outcome, Entity* e) {
    Entity* eArmy = e;
    Entity* eEnemy = eArmy->get<CPlayer>()->enemy;
    int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    Entity* eObj;
    double cxArmy = eEnemy->get<CArmy>()->x;

    ///ARMY SECTIONS
    double columns = 5;
    double lines = 5;
    double wSection = wFormation*uxFormation/columns;
    double hSection = hFormation*uyFormation/lines;
    ///TORNADO
    //double wTornado = Assets::getAnimation("tornado.png").wSprite;
    double hTornado = Assets::getAnimation("tornado.png").hSprite;
    double yTornado = cyWindow + hFormation*uyFormation/2 - hTornado/2 + hSection/2;
    double xTornado = cxArmy - sign*wSection/2;
    double delayToMove = 1.f;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->add(new CAnimation(false, "tornado.png"));
    eObj->add(new CActor());
    eObj->add(new CDraw(CDraw::WORLD_2, 0));
    eObj->add(new CVelocity());
    eObj->add(new CSound("sfx-tornado.wav", CSound::LOOP, 2, 2, 10.f));
    notify(PLAY_SOUND, eObj);

    double speed = 200;
    double speed0 = 300;
    double y1 = yTornado;
    double y2 = yTornado - lines*hSection;
    double tAux = getTravelTime(xTornado, y1, xTornado - 1*sign*wSection, y2, speed);
    double tFirstTravel = getTravelTime(cxWindow, cyWindow, xTornado, yTornado, speed0);
    double fadeSpeed = 120;
    double alphaTarget = 200;
    double fadeDuration = alphaTarget / fadeSpeed;
    double durationLineTravel = tAux;
    double timeToLastLine = fadeDuration + delayToMove + tFirstTravel + 3*durationLineTravel;
    double x0LastLine = xTornado - 3*sign*wSection;
    double y0LastLine = y2;
    double x1LastLine = xTornado - 4*sign*wSection;
    double y1LastLine = y1;

    eObj->get<CActor>()->timeline.push_back(new AFade(0.0, fadeSpeed, alphaTarget));
    eObj->get<CActor>()->timeline.push_back(new AMove(fadeDuration + delayToMove, xTornado, yTornado, speed0));
    eObj->get<CActor>()->timeline.push_back(new AMove(tFirstTravel, xTornado - 1*sign*wSection, y2, speed));
    eObj->get<CActor>()->timeline.push_back(new AMove(tAux, xTornado - 2*sign*wSection, y1, speed));
    eObj->get<CActor>()->timeline.push_back(new AMove(tAux, xTornado - 3*sign*wSection, y2, speed));
    eObj->get<CActor>()->timeline.push_back(new AMove(tAux, xTornado - 4*sign*wSection, y1, speed));
    eObj->get<CActor>()->timeline.push_back(new AFade(0.0, -fadeSpeed, 0.f));
    eObj->get<CActor>()->timeline.push_back(new ADestroy(fadeDuration));
    addActor(eObj);


    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for (Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    std::list<double> hitTimes;
    for (int i = 0; (unsigned) i < eTargets.size(); ++i) {
        hitTimes.push_back(randomDouble(5.f, 7.f));
    }
    hitTimes.sort();
    if (sign == -1) eTargets.sort([]( Entity* a, Entity* b ) { return a->get<CPosition>()->x < b->get<CPosition>()->x; });
    else            eTargets.sort([]( Entity* a, Entity* b ) { return a->get<CPosition>()->x > b->get<CPosition>()->x; });

    std::map<Entity*, double> hitTimesMap;
    for (auto& eUnit : eTargets) {
        hitTimesMap.insert(std::make_pair(eUnit, hitTimes.front()));
        hitTimes.pop_front();
    }

    for (auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        //========================================

        ///ATTACKER
        if (!eAtk->has(Component::ACCELERATION)) eAtk->add(new CAcceleration);

        double tStart = randomDouble(0.f, 0.5f);
        double gravity = 200.f;
        double firstJumpStart = tStart;
        double firstJumpSpeed = 200.f;
        double xOff = -sign*10.f;
        double firstJumpAngle = getAngleToHit(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, cxWindow + xOff, cyWindow, firstJumpSpeed, gravity);
        double firstJumpDuration = getTravelTime(eAtk->get<CPosition>()->x, 0.f, cxWindow + xOff, 0.f, cos(firstJumpAngle*M_RAD) * firstJumpSpeed);

        double lastLineTimeOffset = randomDouble(0.f, durationLineTravel-0.25);
        double secondJumpStart = timeToLastLine - firstJumpDuration + lastLineTimeOffset;
        double x0SecondJump = x0LastLine + abs(x0LastLine - x1LastLine) * (lastLineTimeOffset / durationLineTravel);
        double y0SecondJump = y0LastLine + abs(y0LastLine - y1LastLine) * (lastLineTimeOffset / durationLineTravel);
        double secondJumpSpeed = 200.f;
        double secondJumpAngle = getAngleToHit(x0SecondJump, y0SecondJump, cxWindow, eAtk->get<CPosition>()->y, secondJumpSpeed, gravity);
        double secondJumpDuration = getTravelTime(x0SecondJump, 0.f, cxWindow, 0.f, cos(secondJumpAngle*M_RAD) * secondJumpSpeed);

        double walkSpeed = 400.f;
        double walkDistance = abs(cxWindow - eAtk->get<CPosition>()->x);
        double walkDuration = walkDistance / walkSpeed;

        eAtk->get<CActor>()->addNode(new AAddComponent(firstJumpStart, new CAutoPilot(), Component::AUTO_PILOT));
        eAtk->get<CActor>()->addNode(new AVariable(0.f, AVariable::DRAW_TAG, CDraw::WORLD_2));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_ACC, 0.0));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_ACC, gravity));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_VEL, cos(firstJumpAngle*M_RAD)*firstJumpSpeed));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_VEL, -sin(firstJumpAngle*M_RAD)*firstJumpSpeed));
        eAtk->get<CActor>()->addNode(new AVariable(firstJumpDuration, AVariable::HIDDEN, true));
        eAtk->get<CActor>()->addNode(new ARemoveComponent(0.f, Component::AUTO_PILOT));

        eAtk->get<CActor>()->addNode(new ATeleport(secondJumpStart, x0SecondJump, y0SecondJump));
        eAtk->get<CActor>()->addNode(new AVariable(0.f, AVariable::H_FLIP, !eAtk->get<CAnimation>()->hFlip));
        eAtk->get<CActor>()->addNode(new AVariable(0.f, AVariable::HIDDEN, false));
        eAtk->get<CActor>()->addNode(new AAddComponent(0.f, new CAutoPilot(), Component::AUTO_PILOT));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_ACC, 0.0));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_ACC, gravity));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_VEL, cos(secondJumpAngle*M_RAD)*secondJumpSpeed));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_VEL, -sin(secondJumpAngle*M_RAD)*secondJumpSpeed));
        eAtk->get<CActor>()->addNode(new ARemoveComponent(secondJumpDuration, Component::AUTO_PILOT));
        eAtk->get<CActor>()->addNode(new AMove(0.f, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, walkSpeed));
        eAtk->get<CActor>()->addNode(new AVariable(0.f, AVariable::DRAW_TAG, CDraw::WORLD_1));
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(0.f, eAtk->get<CUnit>()->aWalk));
        eAtk->get<CActor>()->addNode(new AVariable(walkDuration, AVariable::H_FLIP, eAtk->get<CAnimation>()->hFlip));
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(0.f, eAtk->get<CUnit>()->aIdle));

        ///SHADOW 1
        eObj = eManager->createEntity();
        eObj->add(new CPosition(*eAtk->get<CPosition>()));
        eObj->add(new CDraw(CDraw::WORLD));
        eObj->add(new CTexture("small-shadow.png"));
        eObj->add(new CActor());
        eObj->add(new CVelocity());
        eObj->get<CDraw>()->isHidden = true;

        double shadowSpeed = getDistance(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, cxWindow + xOff, cyWindow)/firstJumpDuration;

        eObj->get<CActor>()->addNode(new AVariable(firstJumpStart, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new AMove(0.0, cxWindow + xOff, cyWindow, shadowSpeed));
        eObj->get<CActor>()->addNode(new ADestroy(firstJumpDuration));

        ///SHADOW 2
        eObj = eManager->createEntity();
        eObj->add(new CPosition(x0SecondJump, y0SecondJump));
        eObj->add(new CDraw(CDraw::WORLD));
        eObj->add(new CTexture("small-shadow.png"));
        eObj->add(new CActor());
        eObj->add(new CVelocity());
        eObj->get<CDraw>()->isHidden = true;

        shadowSpeed = getDistance(x0SecondJump, y0SecondJump, cxWindow, eAtk->get<CPosition>()->y)/secondJumpDuration;

        eObj->get<CActor>()->addNode(new AVariable(firstJumpStart + firstJumpDuration + secondJumpStart, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new AMove(0.0, cxWindow, eAtk->get<CPosition>()->y, shadowSpeed));
        eObj->get<CActor>()->addNode(new ADestroy(secondJumpDuration));

        //========================================
        ///DEFENDER
        double tHit = hitTimesMap[eDef];

        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tHit));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], tHit);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], tHit);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tHit));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
}

void ScriptedAnimation::scriptThrowScimitar(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        double tStart = unitRest1 + randomDouble(0, 0.4);
        double t0 = Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new AShoot(t0, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, AShoot::SCIMITAR));
        eAtk->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-act-01.wav"));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction02));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction02).duration, eAtk->get<CUnit>()->aIdle));

        double projSpeed = 400;
        double travelTime = getTravelTime(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y,
                                          eDef->get<CPosition>()->x, eDef->get<CPosition>()->y,
                                          projSpeed);

        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tStart + travelTime + t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], tStart + travelTime + t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], tStart + travelTime + t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tStart + travelTime + t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }


    Entity* eSound = eManager->createEntity();
    eSound->add(new CSound("sfx-spinning-03-01.wav", CSound::LOOP_THEN_REMOVE, 0.2, 1, 4));
    eSound->add(new CTimer(unitRest1 + 0.5, PLAY_SOUND));
}

void ScriptedAnimation::scriptLongbow(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        double tStart = unitRest1 + randomDouble(0, 0.4);
        double t0 = Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-arrow-04.wav"));
        //eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction02));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration, eAtk->get<CUnit>()->aIdle));

        ///ARROW
        double projSpeed = 400;
        double tAux = 0;
        double tHit = tStart + t0;
        Entity* eObj = eManager->createEntity();
        eObj->add(new CPosition(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y));
        eObj->add(new CTexture("arrow2.png"));
        eObj->add(new CActor());
        eObj->add(new CDraw(CDraw::SKY));
        eObj->add(new CVelocity());

        eObj->get<CDraw>()->isHidden = true;

        eObj->get<CActor>()->addNode(new AVariable(tStart + t0, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new AMove(0.0, eAtk->get<CPosition>()->x, -50, projSpeed));
        tAux = getTravelTime(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, eAtk->get<CPosition>()->x, -50, projSpeed) + 1.0;
        tHit += tAux;
        eObj->get<CActor>()->addNode(new ATeleport(tAux, eDef->get<CPosition>()->x, -50));
        eObj->get<CActor>()->addNode(new AMove(0.0, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, projSpeed));
        tAux = getTravelTime(eDef->get<CPosition>()->x, -50, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, projSpeed);
        tHit += tAux;
        eObj->get<CActor>()->addNode(new ADestroy(tAux));

        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tHit));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], tHit);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], tHit);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tHit));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    /*
        Entity* eSound = eManager->createEntity();
        eSound->add(new CSound("sfx-spinning-03-01.wav", CSound::LOOP_THEN_REMOVE, 0.2, 1, 4));
        eSound->add(new CTimer(unitRest1 + 0.5, PLAY_SOUND));
    */
}

void ScriptedAnimation::scriptSonic(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    double soundDate = INFINITY;

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        double tStart = unitRest1 + randomDouble(0.0, 1);
        soundDate = min(soundDate, tStart);
        //eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction01));
        //eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart, eAtk->get<CUnit>()->aAction02));
        //eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction02).duration, eAtk->get<CUnit>()->aIdle));
        eAtk->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::HIDDEN, true));

        Entity* eObj;
        double gravity = 400;
        double speed1 = 100;
        double speed2 = 400;
        double speed3 = 200;
        //double xOff = 300;
        double tAux;
        double tEndStg1 = tStart;
        double tEndStg2 = 0.0;
        //double travelFromMid;
        ///STAGE 1: JUMP
        eObj = eManager->createEntity();
        eObj->add(new CPosition(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y));
        eObj->add(new CAnimation(false, eAtk->get<CUnit>()->aAction01));
        eObj->add(new CDraw(CDraw::WORLD_2));
        eObj->add(new CActor());
        eObj->add(new CAcceleration());
        eObj->add(new CVelocity());
        //eObj->add(new CAutoPilot());
        eObj->add(new CActor());
        double ang = getAngleToHit(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, cxWindow, eDef->get<CPosition>()->y, speed1, gravity);

        eObj->get<CVelocity>()->x = cos(ang*M_RAD)*speed1;
        eObj->get<CVelocity>()->y = -sin(ang*M_RAD)*speed1;
        eObj->get<CAcceleration>()->x = 0;
        eObj->get<CAcceleration>()->y = gravity;

        tAux = getTravelTime(eAtk->get<CPosition>()->x, 0, cxWindow, 0, eObj->get<CVelocity>()->x);
        tEndStg1 += tAux;

        eObj->get<CActor>()->addNode(new AVariable(tStart, AVariable::AUTO_P, true));
        eObj->get<CActor>()->addNode(new ADestroy(tAux));

        ///SHADOW
        eObj = eManager->createEntity();
        eObj->add(new CPosition(*eAtk->get<CPosition>()));
        eObj->add(new CDraw(CDraw::WORLD));
        eObj->add(new CTexture("small-shadow.png"));
        eObj->add(new CActor());
        eObj->add(new CVelocity());
        eObj->get<CDraw>()->isHidden = true;

        double shadowSpeed = getDistance(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, cxWindow, eDef->get<CPosition>()->y)/tAux;

        eObj->get<CActor>()->addNode(new AVariable(tStart, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new AMove(0.0, cxWindow, eDef->get<CPosition>()->y, shadowSpeed));
        eObj->get<CActor>()->addNode(new ADestroy(tAux));

        ///STATE 2: ATTACK and RETURN
        eObj = eManager->createEntity();
        eObj->add(new CPosition(cxWindow, eDef->get<CPosition>()->y));
        eObj->add(new CAnimation(false, eAtk->get<CUnit>()->aAction01));
        eObj->add(new CDraw(CDraw::WORLD_1));
        eObj->add(new CVelocity());
        eObj->add(new CActor());
        eObj->get<CDraw>()->isHidden = true;
        tEndStg2 = tEndStg1;

        eObj->get<CActor>()->addNode(new AVariable(tEndStg1, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction01));
        eObj->get<CActor>()->addNode(new AMove(0.0, cxWindow - sign*wWindow/2 - sign*60, eDef->get<CPosition>()->y, speed2));
        tAux = getTravelTime(cxWindow, eDef->get<CPosition>()->y, cxWindow - sign*wWindow/2 - sign*60, eDef->get<CPosition>()->y, speed2);
        tEndStg2 += tAux + 1;
        eObj->get<CActor>()->addNode(new ATeleport(tAux + 1, cxWindow + sign*wWindow/2 + sign*60, eAtk->get<CPosition>()->y));
        eObj->get<CActor>()->addNode(new AMove(0.0, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, speed3));
        tAux = getTravelTime(cxWindow + sign*wWindow/2 + sign*60, eAtk->get<CPosition>()->y, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, speed3);
        tEndStg2 += tAux;
        eObj->get<CActor>()->addNode(new ADestroy(tAux));

        eAtk->get<CActor>()->addNode(new AVariable(tEndStg2, AVariable::HIDDEN, false));

        /*
                double gravity = 60;
                double speed = 300;
                Entity* eProj = eManager->createEntity();
                eProj->add(new CAnimation(false, "arbalest-proj.png"));
                //eProj->add(new CDimensions(30, 5));
                eProj->add(new CPosition(x0, y0));
                eProj->add(new CDraw(CDraw::WORLD_2));
                eProj->add(new CAutoPilot());
                eProj->add(new CVelocity());
                eProj->add(new CAcceleration());
                eProj->add(new CRotation(true));

                double angle = getAngleToHit(x0, y0, x1, y1, speed);
                eProj->get<CVelocity>()->x = cos(angle*M_RAD)*speed;
                eProj->get<CVelocity>()->y = -sin(angle*M_RAD)*speed;
                eProj->get<CAcceleration>()->x = 0;
                eProj->get<CAcceleration>()->y = gravity;
                */
        ///STALAGMITE
        double t0 = tEndStg1 + getTravelTime(cxWindow, 0, eDef->get<CPosition>()->x, 0, speed2);


        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    Entity* eSound = eManager->createEntity();
    eSound->add(new CActor());
    eSound->get<CActor>()->addNode(new ASound(soundDate, "sfx-sonic-jumps-01.wav"));
    eSound->get<CActor>()->addNode(new ADestroy(0.0));
}

void ScriptedAnimation::scriptStrongWoman(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    //int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    Entity* eSound = eManager->createEntity();
    eSound->add(new CActor());
    eSound->get<CActor>()->addNode(new ASound(unitRest1, "sfx-jump-01-01.wav"));
    eSound->get<CActor>()->addNode(new ADestroy(0.0));

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        Entity* eObj;
        ///ATTACKER
        if (!eAtk->has(Component::ACCELERATION)) eAtk->add(new CAcceleration);

        double tStart = unitRest1 + randomDouble(0.0, 1);
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration, eAtk->get<CUnit>()->aAction02));
        eAtk->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::AUTO_P, true));

        double gravity = 400;
        double speed1 = 600;
        double speed2 = 300;
        double tAux;
        double tTravel;

        double ang = getAngleToHit(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, speed1, gravity);

        eAtk->get<CVelocity>()->x = cos(ang*M_RAD)*speed1;
        eAtk->get<CVelocity>()->y = -sin(ang*M_RAD)*speed1;
        //printf("x vel: %.4f, y vel: %.4f, angle: %.4f\n", eAtk->get<CVelocity>()->x, eAtk->get<CVelocity>()->y, ang*M_RAD);
        eAtk->get<CAcceleration>()->x = 0;
        eAtk->get<CAcceleration>()->y = gravity;

        tTravel = getTravelTime(eAtk->get<CPosition>()->x, 0, eDef->get<CPosition>()->x, 0, eAtk->get<CVelocity>()->x);

        eAtk->get<CActor>()->addNode(new AVariable(tTravel, AVariable::AUTO_P, false));
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aWalk));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::H_FLIP, opposite(eAtk->get<CAnimation>()->hFlip)));
        eAtk->get<CActor>()->addNode(new AMove(0.0, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, speed2));
        tAux = getTravelTime(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, speed2);
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(tAux, eAtk->get<CUnit>()->aIdle));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::H_FLIP, eAtk->get<CAnimation>()->hFlip));

        ///SHADOW
        eObj = eManager->createEntity();
        eObj->add(new CPosition(*eAtk->get<CPosition>()));
        eObj->add(new CDraw(CDraw::WORLD));
        eObj->add(new CTexture("small-shadow.png"));
        eObj->add(new CActor());
        eObj->add(new CVelocity());
        eObj->get<CDraw>()->isHidden = true;

        double shadowSpeed = getDistance(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y)/tTravel;

        eObj->get<CActor>()->addNode(new AVariable(tStart + Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new AMove(0.0, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, shadowSpeed));
        eObj->get<CActor>()->addNode(new ADestroy(tAux));

        ///POOF
        eObj = eManager->createEntity();
        eObj->add(new CPosition(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y));
        eObj->add(new CDraw(CDraw::WORLD_2));
        eObj->add(new CAnimation(false, "poof-02.png"));
        eObj->add(new CDimensions(40, 40));
        eObj->add(new CActor);
        eObj->get<CDraw>()->isHidden = true;
        eObj->get<CActor>()->addNode(new ASpriteAnimation(tStart+tTravel, "poof-02.png"));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, "sfx-punch-01-01.wav"));
        eObj->get<CActor>()->addNode(new ADestroy(Assets::getAnimation("poof-02.png").duration));

        double t0 = tStart + tTravel;


        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
}

void ScriptedAnimation::scriptSamurai(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    Entity* eSound = eManager->createEntity();
    eSound->add(new CActor());
    eSound->get<CActor>()->addNode(new ASound(unitRest1, "sfx-ice-02.wav"));
    eSound->get<CActor>()->addNode(new ADestroy(0.0));

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        Entity* eObj;
        ///ATTACKER
        if (!eAtk->has(Component::ACCELERATION)) eAtk->add(new CAcceleration);

        double tStart = unitRest1 + randomDouble(0.0, 1);
        double speed1 = 300;
        double jumpDist = 250;
        double tTravel = 0;
        //double tHit = tStart;
        double tWalk = 0;
        double tAir = 0;
        double tAux;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart, eAtk->get<CUnit>()->aWalk));
        eAtk->get<CActor>()->addNode(new AMove(0.0, eDef->get<CPosition>()->x + sign*jumpDist, eDef->get<CPosition>()->y, speed1));
        tAux = getTravelTime(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, eDef->get<CPosition>()->x + sign*jumpDist, eDef->get<CPosition>()->y, speed1);
        tTravel += tAux;
        tWalk = tAux;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tAux, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::AUTO_P, true));

        double gravity = 300;
        double speed2 = 200;

        double ang = getAngleToHit(eDef->get<CPosition>()->x + sign*jumpDist, eDef->get<CPosition>()->y, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, speed2, gravity);
        double xVel = cos(ang*M_RAD)*speed2;
        double yVel = -sin(ang*M_RAD)*speed2;
        double xAcc = 0;
        double yAcc = gravity;
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_VEL, xVel));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_VEL, yVel));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::X_ACC, xAcc));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::Y_ACC, yAcc));

        tAir = getTravelTime(eDef->get<CPosition>()->x + sign*jumpDist, 0, eDef->get<CPosition>()->x, 0, xVel);
        tTravel += tAir;

        eAtk->get<CActor>()->addNode(new AVariable(tAir, AVariable::AUTO_P, false));
        eAtk->get<CActor>()->addNode(new ASound(0.0, "sfx-sword-impact.wav"));
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aWalk));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::H_FLIP, opposite(eAtk->get<CAnimation>()->hFlip)));
        eAtk->get<CActor>()->addNode(new AMove(0.0, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, speed1));
        tAux = getTravelTime(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, speed1);
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(tAux, eAtk->get<CUnit>()->aIdle));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::H_FLIP, eAtk->get<CAnimation>()->hFlip));

        ///ICE
        eObj = eManager->createEntity();
        eObj->add(new CPosition(*eDef->get<CPosition>()));
        eObj->add(new CDraw(CDraw::WORLD_2));
        eObj->add(new CAnimation(false, "ice-01.png"));
        eObj->add(new CActor());

        eObj->get<CDraw>()->isHidden = true;
        eObj->get<CDraw>()->alpha = 150;
        eObj->get<CActor>()->addNode(new AVariable(unitRest1, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ADestroy(tTravel + tStart - unitRest1));

        ///SHADOW
        eObj = eManager->createEntity();
        eObj->add(new CPosition(eDef->get<CPosition>()->x + sign*jumpDist, eDef->get<CPosition>()->y));
        eObj->add(new CDraw(CDraw::WORLD));
        eObj->add(new CTexture("small-shadow.png"));
        eObj->add(new CActor());
        eObj->add(new CVelocity());
        eObj->get<CDraw>()->isHidden = true;

        double shadowSpeed = getDistance(eDef->get<CPosition>()->x + sign*jumpDist, eDef->get<CPosition>()->y, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y)/tAir;

        eObj->get<CActor>()->addNode(new AVariable(tStart + tWalk, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new AMove(0.0, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, shadowSpeed));
        eObj->get<CActor>()->addNode(new ADestroy(tAir));

        ///POOF
        /*
        eObj = eManager->createEntity();
        eObj->add(new CPosition(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y));
        eObj->add(new CDraw(CDraw::WORLD_2));
        eObj->add(new CAnimation(false, "poof-02.png"));
        eObj->add(new CDimensions(40, 40));
        eObj->add(new CActor);
        eObj->get<CDraw>()->isHidden = true;
        eObj->get<CActor>()->addNode(new ASpriteAnimation(tStart+tTravel, "poof-02.png"));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, "sfx-ice-01.wav"));
        eObj->get<CActor>()->addNode(new ADestroy(Assets::getAnimation("poof-02.png").duration));
        */
        double t0 = tStart + tTravel - unitRest1;



        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->addNode(new AVariable(unitRest1, AVariable::FREEZE, true));
        eDef->get<CActor>()->addNode(new AVariable(when, AVariable::FREEZE, false));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when + unitRest1, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->addNode(new AVariable(unitRest1, AVariable::FREEZE, true));
        eDef->get<CActor>()->addNode(new AVariable(when, AVariable::FREEZE, false));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when + unitRest1, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
}



void ScriptedAnimation::scriptNinja(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    /*
    Entity* eSound = eManager->createEntity();
    eSound->add(new CActor());
    eSound->get<CActor>()->addNode(new ASound(unitRest1, "sfx-ice-02.wav"));
    eSound->get<CActor>()->addNode(new ADestroy(0.0));
    */
    double darknessDuration = 0;

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        Entity* eObj;
        ///PUFF
        string puffAnimation = "poof-02.png";
        string sfxPoofIn = "sfx-poof-04.wav";
        string sfxPoofOut = "sfx-poof-03.wav";
        double puffDuration = Assets::getAnimation(puffAnimation).duration;
        eObj = eManager->createEntity();
        eObj->add(new CPosition(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y));
        eObj->add(new CDraw(CDraw::WORLD_2));
        eObj->add(new CDimensions(38, 38));
        eObj->add(new CAnimation(false, puffAnimation));
        eObj->add(new CActor());
        eObj->get<CDraw>()->isHidden = true;

        ///ATTACKER
        double tStart = unitRest1;
        double speed1 = 300;
        //double jumpDist = 250;
        double tTravel = 0;
        //double tHit = tStart;
        //double tWalk = 0;
        //double tAir = 0;
        double tAux;
        double xDist = abs(eAtk->get<CPosition>()->x - eDef->get<CPosition>()->x);
        double tHidden = 0.7;

        eObj->get<CActor>()->addNode(new AVariable(tStart, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
        eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

        eObj->get<CActor>()->addNode(new ATeleport(tHidden - puffDuration, eAtk->get<CPosition>()->x - sign*xDist/3, eDef->get<CPosition>()->y));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofIn));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
        eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

        eAtk->get<CActor>()->addNode(new AVariable(tStart, AVariable::HIDDEN, true));
        eAtk->get<CActor>()->addNode(new ATeleport(tHidden, eAtk->get<CPosition>()->x - sign*xDist/3, eDef->get<CPosition>()->y));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eAtk->get<CActor>()->addNode(new AMove(0.0, eDef->get<CPosition>()->x + sign*xDist/3, eDef->get<CPosition>()->y, speed1));
        tAux = getTravelTime(eAtk->get<CPosition>()->x - sign*xDist/3, eDef->get<CPosition>()->y, eDef->get<CPosition>()->x + sign*xDist/3, eDef->get<CPosition>()->y, speed1);
        tTravel += tAux + tHidden;

        eObj->get<CActor>()->addNode(new ATeleport(tAux - puffDuration, eDef->get<CPosition>()->x + sign*xDist/3, eDef->get<CPosition>()->y));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
        eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

        eObj->get<CActor>()->addNode(new ATeleport(tHidden - puffDuration, eDef->get<CPosition>()->x -sign*16, eDef->get<CPosition>()->y));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofIn));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
        eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

        eAtk->get<CActor>()->addNode(new AVariable(tAux, AVariable::HIDDEN, true));
        eAtk->get<CActor>()->addNode(new ATeleport(tHidden, eDef->get<CPosition>()->x -sign*16, eDef->get<CPosition>()->y));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::H_FLIP, opposite(eAtk->get<CAnimation>()->hFlip)));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aIdle));
        tAux = Assets::getAnimation(eAtk->get<CUnit>()->aIdle).duration + 1;
        tTravel += tAux + tHidden;

        eObj->get<CActor>()->addNode(new AVariable(tAux - puffDuration, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
        eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

        eObj->get<CActor>()->addNode(new ATeleport(tHidden - puffDuration, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y));
        eObj->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofIn));
        eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
        eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));
        eObj->get<CActor>()->addNode(new ADestroy(0.0));

        eAtk->get<CActor>()->addNode(new AVariable(tAux, AVariable::HIDDEN, true));
        eAtk->get<CActor>()->addNode(new ATeleport(tHidden, eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::H_FLIP, eAtk->get<CAnimation>()->hFlip));
        eAtk->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));
        eAtk->get<CActor>()->addNode(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aIdle));

        double t0 = tStart + tTravel;

        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
        darknessDuration = max(darknessDuration, t0 + 1);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    ///DARKNESS
    Entity* eDark = eManager->createEntity();
    eDark->add(new CPosition(cxWindow, cyWindow));
    eDark->add(new CRectShape(wWindow, hWindow, sf::Color::Black));
    eDark->add(new CActor());
    eDark->add(new CDraw(CDraw::BLINDFOLD, 0));
    eDark->add(new CFade());
    eDark->get<CActor>()->timeline.push_back(new AFade(0.0, 255, 75));
    eDark->get<CActor>()->timeline.push_back(new AFade(darknessDuration, -255, 0));
    eDark->get<CActor>()->timeline.push_back(new ADestroy(1.0));
    addActor(eDark);
}

void ScriptedAnimation::scriptThrowBomb(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;

    ///AUXILIAR STRUCTURES
    map<Entity*, double> whoBlockedWhen;
    map<Entity*, double> whoDiedWhen;

    map<Entity*, bool> blocked;
    map<Entity*, bool> died;
    EntityList eTargets = getTargets(outcome, e);

    for(Entity* eDef : eTargets) {
        blocked.insert(make_pair(eDef, false));
        died.insert(make_pair(eDef, false));
    }

    for(auto& out : outcome.unitActionOutcomes) {
        Entity* eAtk = getUnitByID(e, out.idCauser);
        Entity* eDef = getUnitByID(eEnemy, out.idTarget);

        ///ATTACKER
        double tStart = unitRest1 + randomDouble(0, 0.4);
        double t0 = Assets::getAnimation(eAtk->get<CUnit>()->aAction01).duration;
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(tStart, eAtk->get<CUnit>()->aAction01));
        eAtk->get<CActor>()->timeline.push_back(new AShoot(t0, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, AShoot::BOMB));
        eAtk->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-act-01.wav"));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eAtk->get<CUnit>()->aAction02));
        eAtk->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eAtk->get<CUnit>()->aAction02).duration, eAtk->get<CUnit>()->aIdle));

        double projSpeed = 300;
        double angle = getAngleToHit(eAtk->get<CPosition>()->x, eAtk->get<CPosition>()->y, eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, projSpeed);
        double travelTime = getTravelTime(eAtk->get<CPosition>()->x, 0, eDef->get<CPosition>()->x, 0, projSpeed*cos(angle*M_RAD));

        ///DEFENDER
        if (out.id == UnitActionOutcome::DIED) {
            if (!died[eDef]) {
                whoDiedWhen.insert(make_pair(eDef, tStart + travelTime + t0));
                died[eDef] = true;
                if (blocked[eDef]) {
                    whoBlockedWhen.erase(whoBlockedWhen.find(eDef));
                    blocked[eDef] = false;
                }
            }
            whoDiedWhen[eDef] = min(whoDiedWhen[eDef], tStart + travelTime + t0);
        } else if (!died[eDef]) {
            if (blocked[eDef]) {
                whoBlockedWhen[eDef] = min(whoBlockedWhen[eDef], tStart + travelTime + t0);
            } else {
                whoBlockedWhen.insert(make_pair(eDef, tStart + travelTime + t0));
                blocked[eDef] = true;
            }
        }

        addActor(eAtk);
    }

    for(auto& p : whoDiedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(when, eDef->get<CUnit>()->aDeath));
        eDef->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-hurt-02.wav"));
        eDef->get<CActor>()->timeline.push_back(new ASpriteAnimation(Assets::getAnimation(eDef->get<CUnit>()->aDeath).duration, eDef->get<CUnit>()->aDead));
        eDef->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::DEAD, true));

        scriptDeathIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }

    for(auto& p : whoBlockedWhen) {
        Entity* eDef = p.first;
        double when = p.second;

        eDef->get<CActor>()->timeline.push_back(new ASound(when, "sfx-impact-05-01.wav"));

        scriptBlockIcon(eDef->get<CPosition>()->x, eDef->get<CPosition>()->y, when, eDef->get<CAnimation>()->hFlip, outcome.dmgType);
        addActor(eDef);
    }
}

void ScriptedAnimation::scriptMercy(ActionOutcome& outcome, Entity* eActor) {
    Entity* eArmy = eActor;
    Entity* eEnemy = eArmy->get<CPlayer>()->enemy;

    eEnemy->get<CArmy>()->prohibitedFormation = eEnemy->get<CArmy>()->formation;
}
void ScriptedAnimation::scriptEnslave(ActionOutcome& outcome, Entity* eActor) {
    Entity* eArmy = eActor;
    int sign = eArmy->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;

    double wField = 0.70*wWindow;
    double dx = wField/((war.getMatchConfig().nTurns+1)*2);
    //double ux = 32;
    //double uy = 26;
    eArmy->get<CArmy>()->x -= sign*dx;
    //double x0 = eArmy->get<CArmy>()->x;
    //double y0 = cyWindow - uy*12/2;

    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        eUnit->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, opposite(eUnit->get<CAnimation>()->hFlip)));
        double yDest = cyWindow + randomDouble(-15,15);
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*400, yDest, 200));
        double t = getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, cxWindow + sign*400, yDest, 200);
        eUnit->get<CActor>()->timeline.push_back(new AMove(t, cxWindow + sign*wWindow/2 + sign*100, yDest, 200));
        t = getTravelTime(cxWindow + sign*400, cyWindow, cxWindow + sign*wWindow/2 + sign*100, yDest, 200);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(t, eUnit->get<CUnit>()->aIdle));
        //eUnit->get<CActor>()->timeline.push_back(new ADestroy(t));

        addActor(eUnit);
    }

    Entity* eCap = eArmy->get<CArmy>()->captain;

    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
    eCap->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, sign == -1? true:false));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200));
    double t = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200);
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t, eCap->get<CCaptain>()->aIdle));
    eCap->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::H_FLIP, sign == -1? false:true));

    addActor(eCap);

    ///--------------------------------------------------------

    eArmy = eArmy->get<CPlayer>()->enemy;

    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++) {
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->dead) continue;
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eUnit->get<CUnit>()->aWalk));
        double yDest = cyWindow + randomDouble(-15,15);
        eUnit->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*400, yDest, 200));
        double t = getTravelTime(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, cxWindow + sign*400, yDest, 200);
        eUnit->get<CActor>()->timeline.push_back(new AMove(t, cxWindow + sign*wWindow/2 + sign*100, yDest, 200));
        t = getTravelTime(cxWindow + sign*400, cyWindow, cxWindow + sign*wWindow/2 + sign*100, yDest, 200);
        eUnit->get<CActor>()->timeline.push_back(new ASpriteAnimation(t, eUnit->get<CUnit>()->aIdle));
        //eUnit->get<CActor>()->timeline.push_back(new ADestroy(t));

        addActor(eUnit);
    }

    eCap = eArmy->get<CArmy>()->captain;

    eCap->get<CActor>()->timeline.push_back(new AVariable(0.0, AVariable::HERO_CONFINED, true));
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, eCap->get<CCaptain>()->aWalk));
    eCap->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200));
    t = getTravelTime(eCap->get<CPosition>()->x, eCap->get<CPosition>()->y, cxWindow + sign*wWindow/2 + sign*100, cyWindow, 200);
    eCap->get<CActor>()->timeline.push_back(new ASpriteAnimation(t, eCap->get<CCaptain>()->aIdle));

    addActor(eCap);
    ///--------------------------------------------------------
}

void ScriptedAnimation::scriptThrowCoin(ActionOutcome& outcome, Entity* e) {
#if 0
    Entity* eWinner = war.getPlayer(outcome.idWinner);
    war.setActorID(outcome.idWinner);
    bool flip = false;
    if (eWinner->get<CPlayer>()->side == CPlayer::LEFT) flip = true;
    else flip = false;

    double wCoin = 50;
    double hCoin = 50;

    Entity* eCoin = eManager->createEntity();
    eCoin->add(new CPosition(cxWindow, -hCoin));
    eCoin->add(new CDimensions(wCoin, hCoin));
    eCoin->add(new CAnimation(false, "coin-official.png"));
    eCoin->add(new CDraw(CDraw::SKY));
    eCoin->add(new CActor());
    eCoin->add(new CVelocity(0,0));

    double speedCoin = 100;
    double tTravel = getDistance(cxWindow, -hCoin, cxWindow, cyWindow)/speedCoin;
    eCoin->get<CActor>()->timeline.push_back(new AMove(0.0, cxWindow, cyWindow, speedCoin));
    eCoin->get<CActor>()->timeline.push_back(new ASpriteAnimation(tTravel, flip ? "blue-coin-official.png" : "red-coin-official.png"));
    eCoin->get<CActor>()->timeline.push_back(new AFade(1.5, -255, 0));

    Entity* eHero = eWinner->get<CArmy>()->captain;
    eHero->get<CActor>()->addNode(new AVariable(tTravel + 1.0, AVariable::COIN, true));

    Entity* eTimer = eManager->createEntity();
    eTimer->add(new CActor());
    eTimer->get<CActor>()->timeline.push_back(new ADestroy(5));
    addActor(eTimer);
#else
    war.setActorID(1);
#endif
}

void ScriptedAnimation::scriptDebuffFire(ActionOutcome& outcome, Entity* e) {
    int hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;
    //int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:+1;
    Entity* eTarget = e->get<CPlayer>()->enemy;
    Entity* eObj;

    ///CAMP FIRE
    double campFadeSpeed = 255.f;
    double campDuration = 3;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->add(new CAnimation(false, "camp-fire.png"));
    eObj->add(new CDraw(CDraw::WORLD_1));
    eObj->add(new CActor());
    eObj->get<CDraw>()->alpha = 0.f;

    eObj->get<CActor>()->addNode(new AFade(0.f, campFadeSpeed, 255));
    eObj->get<CActor>()->addNode(new AFade(campDuration + 255/campFadeSpeed, -campFadeSpeed, 0.f));
    eObj->get<CActor>()->addNode(new ADestroy(255/campFadeSpeed));

    addActor(eObj);

    ///ABORIGENES
    int nCultists = 6;
    double angleStep = 360/(double)nCultists;
    double cultistAngularSpeed = 180;
    double xAmp = 50;
    double yAmp = 50;
    for (int i = 0; i < nCultists; i++) {
        eObj = eManager->createEntity();
        eObj->add(new CPosition());
        eObj->add(new CAnimation(false, "hero-10-walk.png"));
        eObj->add(new CDraw(CDraw::WORLD_1, 0.f));
        eObj->add(new CActor());
        eObj->add(new CElipsoidalMovement(cxWindow, cyWindow, xAmp, yAmp, cultistAngularSpeed, i*angleStep, true, true));

        eObj->get<CActor>()->addNode(new AFade(0.f, campFadeSpeed, 255));
        ///SET FIRST FLIP
        double tAux = 0.f;
        double angle = i*angleStep;
        double tFirstFlip;
        bool flipAux;
        if (angle < 180) {
            flipAux = true;
            tFirstFlip = (180-angle)/cultistAngularSpeed;
        } else {
            flipAux = false;
            tFirstFlip = (360-angle)/cultistAngularSpeed;
        }
        eObj->get<CAnimation>()->hFlip = flipAux;

        flipAux = opposite(flipAux);
        eObj->get<CActor>()->addNode(new AVariable(tFirstFlip, AVariable::H_FLIP, flipAux));
        tAux += tFirstFlip;
        while (tAux <= campDuration) {
            flipAux = opposite(flipAux);
            eObj->get<CActor>()->addNode(new AVariable(180/cultistAngularSpeed, AVariable::H_FLIP, flipAux));
            tAux += 180/cultistAngularSpeed;
        }
        eObj->get<CActor>()->addNode(new AFade(tAux - (campDuration + (255/campFadeSpeed))- tFirstFlip, -campFadeSpeed, 0.0));
        eObj->get<CActor>()->addNode(new ADestroy(255/campFadeSpeed));

    }

    ///TARGETS
    for(auto& i : outcome.idTargets) {
        Entity* eUnit = getUnitByID(eTarget, i);
        Entity* eGem = eManager->createEntity();
        eGem->add(new CPosition(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y));
        eGem->add(new CDraw(CDraw::WORLD_2));
        eGem->add(new CTexture("type-00-gem.png"));
        eGem->add(new CActor());
        eGem->add(new CVelocity());
        eGem->add(new CAcceleration());
        eGem->get<CDraw>()->isHidden = true;

        ///==============================================================

        double tStart = randomDouble(0.5, 1.5);
        double gravity = 400;
        double speed = 400;
        //double tAux;
        double tTravel;

        double ang = getAngleToHit(eGem->get<CPosition>()->x, eGem->get<CPosition>()->y, cxWindow, cyWindow, speed, gravity);
        eGem->get<CActor>()->addNode(new AVariable(tStart, AVariable::AUTO_P, true));
        eGem->get<CActor>()->addNode(new AVariable(0.0, AVariable::HIDDEN, false));

        eGem->get<CVelocity>()->x = cos(ang*M_RAD)*speed;
        eGem->get<CVelocity>()->y = -sin(ang*M_RAD)*speed;
        eGem->get<CAcceleration>()->x = 0;
        eGem->get<CAcceleration>()->y = gravity;

        tTravel = getTravelTime(eGem->get<CPosition>()->x, 0, cxWindow, 0, eGem->get<CVelocity>()->x);

        eGem->get<CActor>()->addNode(new ADestroy(tTravel));

        ///===============================================================

        eUnit->get<CActor>()->timeline.push_back(new AVariable(tStart + 2, AVariable::ADD_EFFECT, (int)outcome.action));

        scriptBlockIcon(eUnit->get<CPosition>()->x, eUnit->get<CPosition>()->y, tStart + 2, opposite(hFlip), CUnit::FIRE);

        addActor(eGem);
    }

    Entity* eHero = e->get<CArmy>()->captain;
    std::string heroName = toUpper(eHero->get<CCaptain>()->uniqueName);
    std::string speech = Assets::getString("SPEECH-" + heroName + "-FIRE-DEBUFF-" + int2str(randomInt(1,1), 2));
    eHero->get<CActor>()->addNode(new ASpeak(0.f, speech, 2.5f));
}

void ScriptedAnimation::scriptDebuffAir(ActionOutcome& outcome, Entity* e) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    int sign = e->get<CPlayer>()->side == CPlayer::LEFT ? -1:1;
    bool hFlip = e->get<CPlayer>()->side == CPlayer::LEFT ? false:true;

    double xOff = wWindow;
    double yOff = 200;
    for(auto& i : outcome.idTargets) {
        Entity* eTarget = getUnitByID(eEnemy, i);
        xOff = max(abs(cxWindow - eTarget->get<CPosition>()->x), xOff);
    }
    xOff = wWindow/2 + xOff;

    for(auto& i : outcome.idTargets) {
        Entity* eTarget = getUnitByID(eEnemy, i);
        double tHit = 0.0;
        Entity* eCrow = eManager->createEntity();
        double tStart = randomDouble(0.0, 2);
        double yCrow = eTarget->get<CPosition>()->y - yOff;
        double speed1 = 300;
        double speed2 = 500;
        //double speed3 = 400;
        double tAux;

        eCrow->add(new CPosition(eTarget->get<CPosition>()->x + sign*xOff, yCrow));
        eCrow->add(new CAnimation(hFlip, "crow1.png"));
        eCrow->add(new CDraw(CDraw::WORLD_2));
        eCrow->add(new CActor());
        eCrow->add(new CVelocity());
        eCrow->get<CActor>()->addNode(new ASpriteAnimation(tStart, "crow1.png"));
        eCrow->get<CActor>()->addNode(new AMove(0.0, eTarget->get<CPosition>()->x + sign*yOff, yCrow, speed1));
        tAux = getTravelTime(eTarget->get<CPosition>()->x + sign*xOff, yCrow,
                             eTarget->get<CPosition>()->x + sign*yOff, yCrow, speed1);
        tHit += tAux;
        eCrow->get<CActor>()->addNode(new ASpriteAnimation(tAux, "crow1-gliding.png"));
        eCrow->get<CActor>()->addNode(new AMove(0.0, eTarget->get<CPosition>()->x, eTarget->get<CPosition>()->y, speed2));
        tAux = getTravelTime(eTarget->get<CPosition>()->x + sign*yOff, yCrow,
                             eTarget->get<CPosition>()->x, eTarget->get<CPosition>()->y, speed2);
        tHit += tAux;
        eCrow->get<CActor>()->addNode(new AMove(tAux, eTarget->get<CPosition>()->x - sign*yOff, yCrow, speed2));
        tAux = getTravelTime(eTarget->get<CPosition>()->x, eTarget->get<CPosition>()->y,
                             eTarget->get<CPosition>()->x - sign*yOff, yCrow, speed2);

        eCrow->get<CActor>()->addNode(new AMove(tAux, cxWindow - sign*wWindow/2 - sign*32, yCrow, speed2));
        tAux = getTravelTime(eTarget->get<CPosition>()->x - sign*yOff, yCrow,
                             cxWindow - sign*wWindow/2, yCrow, speed2);

        eCrow->get<CActor>()->addNode(new ADestroy(tAux));

        tHit += tStart;

        addActor(eCrow);

        Entity* eGem = eManager->createEntity();
        eGem->add(new CPosition());
        eGem->add(new CTexture("type-01-gem.png"));
        eGem->add(new CDraw(CDraw::SKY));
        eGem->add(new CActor());
        eGem->add(new CAnchor(0.0, 10));
        eGem->get<CDraw>()->isHidden = true;
        eGem->get<CActor>()->addNode(new AVariable(tHit, AVariable::HIDDEN, false));

        eGem->addObservedEntity("Anchor", eCrow);
        eGem->attachEmployer(eCrow);


        eTarget->get<CActor>()->timeline.push_back(new AVariable(tHit, AVariable::ADD_EFFECT, (int)outcome.action));

        scriptBlockIcon(eTarget->get<CPosition>()->x, eTarget->get<CPosition>()->y, tHit, opposite(hFlip), CUnit::AIR);

    }

    Entity* eHero = e->get<CArmy>()->captain;
    std::string heroName = toUpper(eHero->get<CCaptain>()->uniqueName);
    std::string speech = Assets::getString("SPEECH-" + heroName + "-AIR-DEBUFF-" + int2str(randomInt(1,1), 2));
    eHero->get<CActor>()->addNode(new ASpeak(0.f, speech, 2.5f));
}

void ScriptedAnimation::scriptTrueForm(ActionOutcome& outcome, Entity* e) {
    Entity* eCapOld = e->get<CArmy>()->captain;
    Entity* eCapNew;

    e->get<CPlayer>()->heroDeck.remove((int)eCapOld->get<CCaptain>()->id);
    eCapOld->get<CActor>()->addNode(new ATeleport(0.0, -200, -200));
    map<CCaptain::ID, Entity*>::iterator it;
    it = e->get<CArmy>()->captains.find(eCapOld->get<CCaptain>()->id);
    e->get<CArmy>()->captains.erase(it);

    CCaptain::ID id = 13;
    e->get<CPlayer>()->heroDeck.push_back(id);

    eCapNew = eManager->createEntity();
    eCapNew->add(new CCaptain(CCaptain::Map[id]));
    eCapNew->add(new CAnimation(eCapOld->get<CAnimation>()->hFlip, CCaptain::Map[id].aIdle));
    eCapNew->add(new CDraw(CDraw::WORLD_1));
    eCapNew->add(new CVelocity());
    eCapNew->add(new CPosition(eCapOld->get<CPosition>()->x, eCapOld->get<CPosition>()->y));
    eCapNew->add(new CActor());
    eCapNew->add(new COwner(e));
    //eCapNew->add(new CDimensions(50, 50));
    eCapNew->get<CVelocity>()->speed = 80;
    eCapNew->addObservedEntity("OriginalHero", eCapOld);

    e->get<CArmy>()->captains.insert(make_pair(id, eCapNew));
    e->get<CArmy>()->captain = eCapNew;

    Entity* eObj;

    ///PUFF
    string puffAnimation = "poof-02.png";
    string sfxPoofIn = "sfx-poof-04.wav";
    string sfxPoofOut = "sfx-poof-03.wav";
    double puffDuration = Assets::getAnimation(puffAnimation).duration;
    eObj = eManager->createEntity();
    eObj->add(new CPosition(eCapNew->get<CPosition>()->x, eCapNew->get<CPosition>()->y));
    eObj->add(new CDraw(CDraw::WORLD_2));
    eObj->add(new CDimensions(85, 85));
    eObj->add(new CAnimation(false, puffAnimation));
    eObj->add(new CActor());

    eObj->get<CActor>()->addNode(new ASound(0.0, sfxPoofOut));
    eObj->get<CActor>()->addNode(new ASpriteAnimation(0.0, puffAnimation));
    eObj->get<CActor>()->addNode(new AVariable(puffDuration, AVariable::HIDDEN, true));

    eCapNew->get<CActor>()->addNode(new ASpeak(0.0, Assets::getString("SPEECH-BUFFALO"), 2));


    addActor(eObj);
    addActor(eCapNew);



}

void ScriptedAnimation::scriptPreAttackSpeech(ActionOutcome& outcome, Entity* e) {
    Entity* eCap = e->get<CArmy>()->captain;
    std::string capName = toUpper(eCap->get<CCaptain>()->uniqueName);
    std::string unitName = toUpper(CUnit::Map[outcome.action].uniqueName);
    std::string strSpeech = "SPEECH-PRE-ATTACK-" + capName + "-" + unitName + "-" + int2str(randomInt(1, 1), 2);
    eCap->get<CActor>()->addNode(new ASpeak(0.0, Assets::getString(strSpeech), 2.3));
}

void ScriptedAnimation::scriptPostAttackSpeech(ActionOutcome& outcome, Entity* e, double date) {
    Entity* eEnemy = e->get<CPlayer>()->enemy;
    Entity* eCap = eEnemy->get<CArmy>()->captain;
    std::string capName = toUpper(eCap->get<CCaptain>()->uniqueName);

    bool effective = false;
    int deathCount = getDeathCount(outcome);
    if (deathCount > 0.15 * eEnemy->get<CArmy>()->nAlive) {
        effective = true;
    }

    std::string strSpeech = "SPEECH-AFTER-DEFENSE-" + capName + (effective ? "" : "-NOT") + "-EFFECTIVE-" + int2str(randomInt(1, 1), 2);
    eCap->get<CActor>()->addNode(new ASpeak(date, Assets::getString(strSpeech), 2.3));
}

double ScriptedAnimation::getActingDuration() {
    double maxDuration = 0;
    for(EntityListIt i = actors.begin(); i != actors.end(); i++) {
        double duration = 0;
        Entity* e = *i;
        for(list<AnimationNode*>::iterator j = e->get<CActor>()->timeline.begin(); j != e->get<CActor>()->timeline.end(); j++) {
            AnimationNode* node = *j;
            duration += node->triggerTime;
        }
        maxDuration = max(maxDuration, duration);
    }
    return maxDuration;
}

Entity* ScriptedAnimation::getUnitByID(Entity* eArmy, int id) {
    for(auto& e : eArmy->get<CArmy>()->allUnits) {
        if (e->get<CUnit>()->armyID == id) return e;
    }
    printf("Unit not found! (player %d, unit %d)\n", eArmy->get<CPlayer>()->id, id);
    return nullptr;
}

void ScriptedAnimation::scriptGrantFormationBuff(ActionOutcome& outcome, Entity* e) {
//    e->get<CArmy>()->formation = outcome.formation;
    CArmy::Formation formation = e->get<CArmy>()->formation;
    CAction::ID effect;
    switch(formation) {
    case CArmy::F1:
        effect = 300;
        break;
    case CArmy::F2:
        effect = 301;
        break;
    case CArmy::F3:
        effect = 302;
        break;
    case CArmy::F4:
        effect = 303;
        break;
    default:
        break;
    }
    e->get<CArmy>()->armyEffects.insert(make_pair(effect, CAction::Map[effect]));
}

EntityList ScriptedAnimation::getActors(ActionOutcome& outcome, Entity* e) {
    EntityList l;
    for (auto& i : outcome.idActors) {
        l.push_back(getUnitByID(e, i));
    }
    return l;
}

EntityList ScriptedAnimation::getTargets(ActionOutcome& outcome, Entity* e) {
    EntityList l;
    for (auto& i : outcome.idTargets) {
        l.push_back(getUnitByID(e->get<CPlayer>()->enemy, i));
    }
    return l;
}

void ScriptedAnimation::scriptPurificationFlask(double xOrigin, double yOrigin, double tStart) {
    //Entity* eEffect = eManager->createEntity();
    //Entity* eEmmitter;

    vector<int> starWeight = {50, 30, 10, 5, 5};
    vector<int> stars;
    for(unsigned int i = 0; i < starWeight.size(); i++) {
        for (int c = 0; c < starWeight[i]; c++) {
            stars.push_back(i+1);
        }
    }

    int nOrigins = 8;
    double dimStar = 32;
    double starSpeed = 200;
    double fadeSpeed = 500;
    double durFade = 255/fadeSpeed;
    double dx = wFormation*uxFormation/(double)nOrigins;
    int nStars = hFormation*uyFormation/dimStar;
    double dy = dimStar;
    double x = xOrigin;
    double y = yOrigin;
    double tBetween = getTravelTime(0, 0, 0, dy, starSpeed);
    double yMin = cyWindow - hFormation*uyFormation/2;
    double yMax = cyWindow + hFormation*uyFormation/2;
    double tFirst = tStart;

    for (int i = 0; i < nOrigins/2; i++) {
        double tGo = tFirst;
        for(int j = 0; j < nStars; j++) {
            Entity* eStar;

            int starID = stars[randomInt(0, stars.size()-1)];
            double yTarget = randomDouble(yMin, yMax);
            double durTravel = getTravelTime(x, y, x, yTarget, starSpeed);
            double giggleSpeed = randomDouble(360, 700);

            eStar = eManager->createEntity();
            eStar->add(new CPosition(x, y));
            eStar->add(new CTexture("star-" + int2str(starID, 2) + ".png"));
            eStar->add(new CDraw(CDraw::SKY, false, 0.0));
            eStar->add(new CActor());
            eStar->add(new CVelocity());
            eStar->add(new CElipsoidalMovement(x, y, 3, 0, giggleSpeed, 0, true, false));

            eStar->get<CActor>()->addNode(new AFade(tGo, fadeSpeed, 255));
            eStar->get<CActor>()->addNode(new AMove(0.0, x, yTarget, starSpeed));
            eStar->get<CActor>()->addNode(new AFade(durTravel, fadeSpeed, 0));
            eStar->get<CActor>()->addNode(new ADestroy(durFade));

            if (i > 0) {
                x -= 2*i*dx;

                starID = stars[randomInt(0, stars.size()-1)];
                yTarget = randomDouble(yMin, yMax);
                durTravel = getTravelTime(x, y, x, yTarget, starSpeed);

                eStar = eManager->createEntity();
                eStar->add(new CPosition(x, y));
                eStar->add(new CTexture("star-" + int2str(starID, 2) + ".png"));
                eStar->add(new CDraw(CDraw::SKY, false, 0.0));
                eStar->add(new CActor());
                eStar->add(new CVelocity());
                eStar->add(new CElipsoidalMovement(x, y, 3, 0, giggleSpeed, 0, true, false));

                eStar->get<CActor>()->addNode(new AFade(tGo, fadeSpeed, 255));
                eStar->get<CActor>()->addNode(new AMove(0.0, x, yTarget, starSpeed));
                eStar->get<CActor>()->addNode(new AFade(durTravel, fadeSpeed, 0));
                eStar->get<CActor>()->addNode(new ADestroy(durFade));

                x += 2*i*dx;
            }

            tGo += tBetween;
        }
        tFirst += tBetween;
        y += dy;
        x += dx;
    }

}

void ScriptedAnimation::scriptBlockIcon(double x, double y, double timing, bool hFlip, CUnit::DamageType type) {
    double yOrigin = y - 32;
    double yTarget = yOrigin - 20;
    double speed = 60;
    string pic = "shield-" + int2str(type, 2) + ".png";
    double tAux;
    double angSpeed = 360;
    double startingAngle = 180;
    double duration = 1.8;
    double yAmp = 4;

    Entity* eIcon = eManager->createEntity();
    eIcon->add(new CPosition(x, yOrigin));
    eIcon->add(new CTexture(pic, hFlip));
    eIcon->add(new CDraw(CDraw::WORLD_3));
    eIcon->add(new CActor());
    eIcon->add(new CVelocity());

    eIcon->get<CDraw>()->isHidden = true;
    eIcon->get<CActor>()->timeline.push_back(new AVariable(timing, AVariable::HIDDEN, false));
    eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, x, yTarget, speed));
    tAux = getTravelTime(x, yOrigin, x, yTarget, speed);
    eIcon->get<CActor>()->addNode(new AAddComponent(tAux, new CElipsoidalMovement(x, yTarget, 0, yAmp, angSpeed, startingAngle), Component::ELIPSOIDAL_MOVEMENT));
    eIcon->get<CActor>()->timeline.push_back(new ADestroy(duration));

    addActor(eIcon);
}

void ScriptedAnimation::scriptCurseIcon(double x, double y, double timing, bool hFlip) {
    double yOrigin = y - 32;
    double yTarget = yOrigin - 20;
    double speed = 60;
    string pic = "curse-icon.png";
    double tAux;
    double angSpeed = 360;
    double startingAngle = 180;
    double duration = 1.8;
    double yAmp = 4;

    Entity* eIcon = eManager->createEntity();
    eIcon->add(new CPosition(x, yOrigin));
    eIcon->add(new CTexture(pic, hFlip));
    eIcon->add(new CDraw(CDraw::WORLD_3));
    eIcon->add(new CActor());
    eIcon->add(new CVelocity());

    eIcon->get<CDraw>()->isHidden = true;
    eIcon->get<CActor>()->timeline.push_back(new AVariable(timing, AVariable::HIDDEN, false));
    eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, x, yTarget, speed));
    tAux = getTravelTime(x, yOrigin, x, yTarget, speed);
    eIcon->get<CActor>()->addNode(new AAddComponent(tAux, new CElipsoidalMovement(x, yTarget, 0, yAmp, angSpeed, startingAngle), Component::ELIPSOIDAL_MOVEMENT));
    eIcon->get<CActor>()->timeline.push_back(new ADestroy(duration));

    addActor(eIcon);
}

void ScriptedAnimation::scriptDeathIcon(double x, double y, double timing, bool hFlip, CUnit::DamageType type) {
    double yOrigin = y - 32;
    double yTarget = yOrigin - 20;
    double speed = 60;
    string pic = "skull.png";
    double tAux;
    double angSpeed = 360;
    double startingAngle = 180;
    double duration = 1.5;
    double yAmp = 4;

    Entity* eIcon = eManager->createEntity();
    eIcon->add(new CPosition(x, yOrigin));
    eIcon->add(new CTexture(pic, hFlip));
    eIcon->add(new CDraw(CDraw::WORLD_3));
    eIcon->add(new CActor());
    eIcon->add(new CVelocity());

    eIcon->get<CDraw>()->isHidden = true;
    eIcon->get<CActor>()->timeline.push_back(new AVariable(timing, AVariable::HIDDEN, false));
    eIcon->get<CActor>()->timeline.push_back(new AMove(0.0, x, yTarget, speed));
    tAux = getTravelTime(x, yOrigin, x, yTarget, speed);
    eIcon->get<CActor>()->addNode(new AAddComponent(tAux, new CElipsoidalMovement(x, yTarget, 0, yAmp, angSpeed, startingAngle), Component::ELIPSOIDAL_MOVEMENT));
    eIcon->get<CActor>()->timeline.push_back(new ADestroy(duration));

    addActor(eIcon);
}

void ScriptedAnimation::scriptVictoryAnimation(double delay) {
    Entity* eObj;
    Entity* ePanel;
    CDraw::Tag drawTag = CDraw::GUI3;

    std::string playerName = war.getPlayer(war.getBattleWinner())->get<CPlayer>()->name;

    double wPanel = 350;
    double hPanel = 90;
    std::string texturePanel = war.getPlayer(war.getBattleWinner())->get<CPlayer>()->side == CPlayer::LEFT ? "9p-frame-01.png" : "9p-frame-02.png";
    double wMin = Assets::getTexture(texturePanel)->getSize().x;

    /// Panel
    ePanel = eManager->createEntity();
    ePanel->add(new CPosition(cxWindow, cyWindow - 80));
    ePanel->add(new CDimensions(wMin, hPanel));
    ePanel->add(new CDraw(drawTag, true, 255));
    ePanel->add(new CTexture(texturePanel));
    ePanel->add(new CActor());
    ePanel->add(new CAutoPilot());
    ePanel->add(new CElipsoidalMovement(cxWindow, cyWindow - 80, 0, 4, 180));

    ePanel->get<CActor>()->addNode(new AVariable(delay, AVariable::HIDDEN, false));
    ePanel->get<CActor>()->addNode(new AZoom(0.0, 700, 0, wPanel, hPanel));

    sf::Vector2f center(ePanel->get<CPosition>()->x, ePanel->get<CPosition>()->y);
    sf::Vector2f topLeft(ePanel->get<CPosition>()->x - wPanel/2,
                         ePanel->get<CPosition>()->y - hPanel/2);
    sf::Vector2f bottomRight(ePanel->get<CPosition>()->x + wPanel/2,
                             ePanel->get<CPosition>()->y + hPanel/2);

    /// Text
    eObj = eManager->createEntity();
    eObj->add(new CAnchor(0, 0));
    eObj->add(new CPosition(-100, -100));
    eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 18, sf::Color::White));
    eObj->add(new CDraw((CDraw::Tag)((int)drawTag+1)));
    eObj->add(new CActor());
    eObj->addObservedEntity("Anchor", ePanel);

    eObj->get<CActor>()->addNode(new AAddComponent(delay, new CTypingEffect(playerName + " Wins", 50), Component::TYPING_EFFECT));

    double tPanelReady = (wPanel - wMin) / 700 + delay;
    double objectsDelay = 0.18;

    /// Crown
    eObj = eManager->createEntity();
    eObj->add(new CAnchor(-wPanel/2 + 20, -hPanel/2 - 10));
    eObj->add(new CPosition(-100, -100));
    eObj->add(new CDraw((CDraw::Tag)((int)drawTag+1), true, 255));
    eObj->add(new CTexture("crown.png"));
    eObj->add(new CActor());
    eObj->add(new CRotation(0.f));
    //eObj->add(new CTilt(5, 15));
    eObj->addObservedEntity("Anchor", ePanel);

    eObj->get<CActor>()->addNode(new AVariable(tPanelReady + objectsDelay, AVariable::HIDDEN, false));
    scriptPoofAt(topLeft.x + 20, topLeft.y - 10, tPanelReady + objectsDelay);

    /// Drums
    eObj = eManager->createEntity();
    eObj->add(new CAnchor(+wPanel/2 - 45, +hPanel/2));
    eObj->add(new CPosition(-100, -100));
    eObj->add(new CDraw((CDraw::Tag)((int)drawTag+1), true, 255));
    eObj->add(new CTexture("drums.png"));
    eObj->add(new CActor());
    eObj->add(new CRotation(0.f));
    eObj->add(new CTilt(5, 60, 0, 30));
    eObj->addObservedEntity("Anchor", ePanel);

    eObj->get<CActor>()->addNode(new AVariable(tPanelReady + 2*objectsDelay, AVariable::HIDDEN, false));
    scriptPoofAt(bottomRight.x - 45, bottomRight.y, tPanelReady + 2*objectsDelay);

    /// Violin
    eObj = eManager->createEntity();
    eObj->add(new CAnchor(+wPanel/2 - 10, +hPanel/2 - 40));
    eObj->add(new CPosition(-100, -100));
    eObj->add(new CDraw((CDraw::Tag)((int)drawTag+2), true, 255));
    eObj->add(new CTexture("violin.png"));
    eObj->add(new CActor());
    eObj->add(new CRotation(0.f));
    eObj->add(new CTilt(5, 80, 0, 180));
    eObj->addObservedEntity("Anchor", ePanel);

    eObj->get<CActor>()->addNode(new AVariable(tPanelReady + 3*objectsDelay, AVariable::HIDDEN, false));
    scriptPoofAt(bottomRight.x - 10, bottomRight.y - 40, tPanelReady + 3*objectsDelay);

    /// Trumpet
    eObj = eManager->createEntity();
    eObj->add(new CAnchor(+wPanel/2, +hPanel/2 - 25));
    eObj->add(new CPosition(-100, -100));
    eObj->add(new CDraw((CDraw::Tag)((int)drawTag+3), true, 255));
    eObj->add(new CTexture("trumpet.png"));
    eObj->add(new CActor());
    eObj->add(new CRotation(0.f));
    eObj->add(new CTilt(5, 50, 0, 120));
    eObj->addObservedEntity("Anchor", ePanel);

    eObj->get<CActor>()->addNode(new AVariable(tPanelReady + 4*objectsDelay, AVariable::HIDDEN, false));
    scriptPoofAt(bottomRight.x, bottomRight.y - 25, tPanelReady + 4*objectsDelay);

    scriptConfettiRain(delay);

    /// SFX: FANFARE
    eObj = eManager->createEntity();
    eObj->add(new CActor());
    eObj->get<CActor>()->addNode(new ASound(0.0, "fanfare-medium.ogg"));

    notify(STOP_MUSIC);
    /// MUSIC WINNING LOOP
    eObj = eManager->createEntity();
    eObj->add(new CTimer(3.92, PLAY_MUSIC));
    eObj->add(new CMusic("winning-loop.ogg"));

    askEndOfMatchQuestions(delay);
}

void ScriptedAnimation::scriptConfettiRain(double delay) {
    std::vector<std::string> confettis = {"confetti-01.png",
                                          "confetti-02.png",
                                          "confetti-03.png",
                                          "confetti-04.png",
                                          "confetti-05.png",
                                          "confetti-06.png",
                                          "confetti-07.png",
                                          "confetti-08.png"
                                         };

    Entity* eEffect = eManager->createEntity();
    eEffect->add(new CParticleEffect());
    eEffect->add(new CActor());
    ///DROP EMMITERS
    double nEmmiters = 20;
    double space = wWindow/nEmmiters;
    double xEmmiter = 0;
    for(int i = 0; i <= nEmmiters; i++) {
        for (unsigned int j = 0; j < confettis.size(); j++) {
            Entity* eEmmiter = eManager->createEntity();
            double xPosition = randomDouble(xEmmiter-12, xEmmiter+12);
            eEmmiter->add(new CPosition(xPosition, -30));
            eEmmiter->attachEmployer(eEffect);
            eEmmiter->add(new CParticleEmmiter(randomDouble(1, 2), confettis[j], randomDouble(0.5, 2.4), 200, 300, 90, 0));
            eEmmiter->get<CParticleEmmiter>()->addElipsoidalMovement(CElipsoidalMovement(xPosition, 0, 5, 0, 500, 0, true, false));
            eEffect->get<CParticleEffect>()->emmiters.push_back(eEmmiter);
            eEmmiter->get<CParticleEmmiter>()->on = false;
            eEmmiter->get<CParticleEmmiter>()->drawTag = CDraw::SKY;
        }
        xEmmiter += space;
    }

    eEffect->get<CActor>()->addNode(new AVariable(delay, AVariable::PARTICLE_EFFECT, true));
}

void ScriptedAnimation::scriptPoofAt(double x, double y, double when) {
    Entity* eSmoke = eManager->createEntity();
    string animation = "poof-03.png";
    double w = Assets::getAnimation(animation).wSprite;
    double h = Assets::getAnimation(animation).hSprite;
    eSmoke->add(new CAnimation(false, animation));
    eSmoke->add(new CPosition(x, y));
    eSmoke->add(new CDraw(CDraw::GUI_05));
    eSmoke->add(new CDimensions(w, h));
    eSmoke->add(new CActor());

    eSmoke->get<CDraw>()->isHidden = true;
    eSmoke->get<CActor>()->timeline.push_back(new AVariable(when, AVariable::HIDDEN, false));
    eSmoke->get<CActor>()->timeline.push_back(new ASpriteAnimation(0.0, animation));
    eSmoke->get<CActor>()->timeline.push_back(new ASound(0.0, "sfx-poof-01.wav"));
    eSmoke->get<CActor>()->timeline.push_back(new ADestroy(Assets::getAnimation(animation).duration));
}

void ScriptedAnimation::askEndOfMatchQuestions(double delay) {
    Entity* eButton = eManager->createEntity();
    eButton->add(new CPosition(cxWindow, cyWindow));
    eButton->add(new CButtonHitbox(wWindow, hWindow));
    eButton->add(new CButtonState());
    eButton->add(new CButtonTrigger({END_MATCH, START_SCREEN_TRANSITION}));
    eButton->add(new CScreen(CScreen::MAIN_MENU_NO_ANIMATION, CScreen::FADE_BLACK));
}

sf::Vector2f ScriptedAnimation::getArmyMiddlePoint(Entity* e) {
    sf::Vector2f point(0,0);
    for (Entity* eUnit : e->get<CArmy>()->allUnits) {
        if (eUnit->get<CUnit>()->dead) continue;
        point.x += eUnit->get<CPosition>()->x;
        point.y += eUnit->get<CPosition>()->y;
    }
    point.x /= e->get<CArmy>()->nAlive;
    point.y /= e->get<CArmy>()->nAlive;
    return point;
}

void ScriptedAnimation::onNewScreen(Entity* e) {
    clearScene();
    playingScene = false;
    notify(SCENE_ENDED);
}

int ScriptedAnimation::getDeathCount(ActionOutcome& outcome) {
    int result = 0;
    for (auto& out : outcome.unitActionOutcomes) {
        if (out.id == UnitActionOutcome::DIED) {
            ++result;
        }
    }
    return result;
}
