#pragma once
#include "System.h"

class GUIGroupSystem : public System{
    public:
        void update();
    private:
        void onCreateGUIGroup(Entity* e);
        void onChangeWindowPage(Entity* e);
        void createWindowSinglePlayer(Entity* e);
};
