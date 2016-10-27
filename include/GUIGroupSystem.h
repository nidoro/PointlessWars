#pragma once
#include "System.h"

class GUIGroupSystem : public System{
    public:
        GUIGroupSystem();

        void update();
    private:

        void onCreateGUIGroup(Entity* e);
        void onChangeWindowPage(Entity* e);
        void onRemoveGUIGroup(Entity* e);
        void onMouseButtonReleased(Entity* e);

        void createWindowSinglePlayer(Entity* e);
        Entity* createRectButton(string label, double fontSize, double h, double x, double y,
                                sf::Color textColor, sf::Color fillColor, sf::Color outColor, double outThickness, Message m, CUILayer::Layer UILayer);

        void hidePage(Entity* e);
        void showPage(Entity* e);

};
