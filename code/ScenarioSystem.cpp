#include "ScenarioSystem.h"

ScenarioSystem::ScenarioSystem() {
    addRequirement(Component::SCENARIO_OBJECT);
    addSubscription(INITIALIZE_WAR);
    addSubscription(LOAD_SCENARIO);
    addSubscription(NEW_COMMAND_LINE);
    regularScenarios = {"desert.sce", "snow.sce", "woods.sce", "beach.sce"};
}

ScenarioSystem::~ScenarioSystem() {

}

void ScenarioSystem::update() {
    
}

void ScenarioSystem::load(string name, double xOff, double yOff) {
    if (toUpper(name) == "RANDOM") name = getRandom(current);

    CScenario scenario = CScenario::Map[name];

    Entity* e = eManager->createEntity();
    e->add(new CTexture(scenario.background));
    e->add(new CPosition(1280/2 + xOff, 720/2 + yOff));
    //e->add(new CDimensions(wWindow, hWindow));
    e->add(new CDraw(CDraw::BACKGROUND));
    e->add(new CScenarioObject());

    Entity* eMusic = eManager->createEntity();
    eMusic->add(new CMusic(scenario.bgMusic));
    notify(PLAY_MUSIC, eMusic);

    EntityList objectsWithBindedAnim;
    for(list<CScenarioObject>::iterator i = scenario.objects.begin(); i != scenario.objects.end(); i++) {
        Entity* eObj = eManager->createEntity();
        eObj->add(new CAnimation(false, CScenarioObject::Map[i->id].aDefault));
        //eObj->add(new CTexture(CScenarioObject::Map[i->id].aDefault));
        eObj->add(new CDraw(CDraw::WORLD));
        eObj->add(new CPosition(i->xRelative*1280 + xOff, i->yRelative*720 + yOff));
        if (CScenarioObject::Map[i->id].aDefault != CScenarioObject::Map[i->id].aClick) {
            eObj->add(new CButtonTrigger(TRIGGER_OBJECT_ANIMATION));
            eObj->add(new CButtonHitbox(CScenarioObject::Map[i->id].width, CScenarioObject::Map[i->id].height));
            eObj->add(new CButtonState());
        }
        eObj->add(new CActor());
        eObj->add(new CScenarioObject(CScenarioObject::Map[i->id]));
        //eObj->add(new CScenarioObject(*i));
        eObj->get<CScenarioObject>()->bindedAnimationID = i->bindedAnimationID;
        eObj->get<CAnimation>()->frame = randomInt(0, Assets::getAnimation(eObj->get<CAnimation>()->current).nFrames - 1);
        eObj->get<CAnimation>()->sprite = Assets::getAnimation(eObj->get<CAnimation>()->current).frames[eObj->get<CAnimation>()->frame];
        eObj->get<CAnimation>()->update = false;
        if (eObj->get<CScenarioObject>()->bindedAnimationID != 0) {
            objectsWithBindedAnim.push_back(eObj);
        }
    }

    std::map<int, Entity*> bindedAnimationsMap;
    for (Entity* eObj : objectsWithBindedAnim) {
        if (bindedAnimationsMap.find(eObj->get<CScenarioObject>()->bindedAnimationID) == bindedAnimationsMap.end()) {
            bindedAnimationsMap.insert(std::make_pair(eObj->get<CScenarioObject>()->bindedAnimationID, eObj));
        }
    }

    for (auto& p : bindedAnimationsMap) {
        for (Entity* eObj : objectsWithBindedAnim) {
            if (eObj != bindedAnimationsMap[p.first] && eObj->get<CScenarioObject>()->bindedAnimationID == p.first) {
                eObj->addObservedEntity("TiedAnimation", bindedAnimationsMap[p.first]);
            }
        }
    }

    current = name;
}

void ScenarioSystem::onLoadScenario(Entity* e) {
    clear();
    load(e->get<CScenario>()->name, e->get<CScenario>()->xOff, e->get<CScenario>()->yOff);
}

void ScenarioSystem::onInitializeWar(Entity* e) {
    clear();
    load(war.getMatchConfig().scenario);
}

void ScenarioSystem::clear() {
    current.clear();
    for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
        if (!eManager->isDead(*i)) {
            eManager->removeEntity(*i);
        }
    }
}

string ScenarioSystem::getRandom(string except) {
    std::list<std::string> candidates = regularScenarios;
    candidates.remove(except);
    if (candidates.empty()) return "";
    std::list<std::string>::iterator it = candidates.begin();
    int i = randomInt(0, candidates.size()-1);
    std::advance(it, i);
    return *it;
}

void ScenarioSystem::onNewCommandLine(Entity* e) {
    if (e->get<CCommandLine>()->command == "load-scenario") {
        if (e->get<CCommandLine>()->hasParam("name")) {
            string name = e->get<CCommandLine>()->params["name"] + ".sce";
            clear();
            load(name);
        } else {
            printf("Command play-music help:\n");
            printf("load-scenario -name <NAME>\n");
        }
    }
}


