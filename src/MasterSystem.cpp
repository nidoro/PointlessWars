#include "MasterSystem.h"

MasterSystem::MasterSystem(){
    addSubscription(UPDATE_PLAYER_NICKNAME_WITH_INPUT_TEXTBOX);
}

MasterSystem::~MasterSystem(){
    //dtor
}

void MasterSystem::update(){}

void MasterSystem::onUpdatePlayerNicknameWithInputTextbox(Entity* e){
    profile.name = e->get<CInputTextBox>()->content;
}
