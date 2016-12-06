#include "DisplayerSystem.h"

DisplayerSystem::DisplayerSystem(){
    addRequirement(Component::DISPLAYER);
    addRequirement(Component::POSITION);
    addRequirement(Component::TEXT_BOX_2);
}

DisplayerSystem::~DisplayerSystem(){
    //dtor
}

void DisplayerSystem::update(){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        Entity* eValue = e->get<CDisplayer>()->eValue;

        string text;
        switch(e->get<CDisplayer>()->valueType){
            case CDisplayer::SPIN_BUTTON: text = int2str(eValue->get<CSpinButton>()->value); break;
            case CDisplayer::SECTOR_STRENGTH: text = int2str(eValue->get<CSector>()->strength); break;
            case CDisplayer::UNIT_COUNT: text = int2str(getUnitCount(eValue)); break;
            case CDisplayer::RESISTANCE_NEUTRAL: text = int2str(eValue->get<CUnit>()->resistance[CUnit::EARTH]); break;
            case CDisplayer::RESISTANCE_FIRE: text = int2str(eValue->get<CUnit>()->resistance[CUnit::FIRE]); break;
            case CDisplayer::RESISTANCE_WATER: text = int2str(eValue->get<CUnit>()->resistance[CUnit::WATER]); break;
            case CDisplayer::RESISTANCE_ELECTRIC: text = int2str(eValue->get<CUnit>()->resistance[CUnit::AIR]); break;
            case CDisplayer::INPUT_TEXT: text = eValue->get<CInputTextBox>()->content; break;
            case CDisplayer::BATTLE_STATE_DESCRIPTION: text = eValue->get<CBattle>()->stateDescription; break;
            case CDisplayer::DROP_LIST_VALUE: text = eValue->get<CDropList>()->value; break;
            case CDisplayer::ALIVE_COUNT: text = int2str(eValue->get<CArmy>()->nAlive); break;
            case CDisplayer::STR_TOGGLE_BUTTON: text = eValue->get<CStringToggleButton>()->values[eValue->get<CStringToggleButton>()->valueIndex]; break;
            case CDisplayer::GAME_STATE: text = getGameStateString(); break;
            default: break;
        }
        e->get<CTextbox2>()->content.setString(text);
        if (e->get<CDisplayer>()->valueType == CDisplayer::GAME_STATE && text.empty()){
            //e->get<CDraw>()->isHidden = true;
        }else{
            //e->get<CDraw>()->isHidden = false;
        }
    }
}

int DisplayerSystem::getUnitCount(Entity* e){
    Entity* eArmy = e->getObservedEntity("Player");
    CUnit::ID id = e->get<CUnit>()->id;
    int count = 0;
    for(EntityListIt i = eArmy->get<CArmy>()->allUnits.begin(); i != eArmy->get<CArmy>()->allUnits.end(); i++){
        Entity* eUnit = *i;
        if (eUnit->get<CUnit>()->id == id && !eUnit->get<CUnit>()->dead){
            count++;
        }
    }
    return count;
}

string DisplayerSystem::getGameStateString(){
    Entity* ePlayer = war.getActor();
    string playerName;
    string stateString;
    if (ePlayer) playerName = ePlayer->get<CPlayer>()->name;
    if (war.getSystemAction() == war.NONE){
        return "";
    }else if (war.getSystemAction() == war.COIN){
        return "Behold the almighty coin...";
    }else if (war.getSystemAction() == war.ASK_HERO_PICK){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_FORMATION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.PLAY_FORMATION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ADVANCE_ARMY){
        return "";
    }else if (war.getSystemAction() == war.PLAY_ACTION){
    }else if (war.getSystemAction() == war.BETWEEN_ROUNDS){
    }else if (war.getSystemAction() == war.ASK_CAPTAIN_ACTION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_ARMY_ACTION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_BATTLE_CLOSURE){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_ARMY_ASSEMBLE){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.ASK_CAPTAIN_SELECTION){
        return playerName + " is thinking...";
    }else if (war.getSystemAction() == war.PRESENT_ARMY){
    }else if (war.getSystemAction() == war.PRESENT_ARMIES){
        return "Presenting for duty...";
    }else if (war.getSystemAction() == war.RECOMPOSE_ARMY){
    }else if (war.getSystemAction() == war.SHOW_HERO_POOL){

    }else if (war.getSystemAction() == war.START_FIRST_BATTLE){
    }else if (war.getSystemAction() == war.START_BATTLE){
    }else if (war.getSystemAction() == war.FIELD_CLEANUP){
    }else if (war.getSystemAction() == war.BETWEEN_TURNS){
    }else if (war.getSystemAction() == war.ENDING){
    }else if (war.getSystemAction() == war.UPDATE_SCORE){
    }
    return "";
}

