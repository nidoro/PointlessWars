#pragma once
#include "System.h"
#include "Assets.h"

class GUIGroupSystem : public System {
    public:
        GUIGroupSystem();

        void update();
    private:

        void onCreateGUIGroup(Entity* e);
        void onChangeWindowPage(Entity* e);
        void onRemoveGUIGroup(Entity* e);
        void onMouseButtonPressed(Entity* e);
        void onDisconnectFromServer(Entity* e);
        void onUpdatePlayerNicknameWithInputTextbox(Entity* e);

        Entity* createLeaveMatchConfirmationWindow(Entity* e);
        Entity* createEndMultiplayerMatchWindow(Entity* e, std::string message);
        void initializeGameOptionsPage(Entity* eGUI, bool closeOnEsc);
        void createWindowSinglePlayer(Entity* e);
        Entity* createWindowInGameMenu(Entity* e);
        Entity* createWindowMultiplayer(Entity* e);
        Entity* createCredits(Entity* e);
        Entity* createHelp(Entity* e, bool centralized);
        void createWindowGameOptions(Entity* e);
        Entity* createRectButton(string label, double fontSize, double h, double x, double y,
                                 sf::Color textColor, sf::Color fillColor, sf::Color outColor, double outThickness, Message m, CUILayer::Layer UILayer);

        void hidePage(Entity* e);
        void showPage(Entity* e);
        
        // @note: helper
        void anchorize(Entity* eAnchor, EntityList eList);

};
