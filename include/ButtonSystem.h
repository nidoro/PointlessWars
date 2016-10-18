#ifndef BUTTONSYSTEM_H
#define BUTTONSYSTEM_H

#include "System.h"
#include "Standard.h"
using namespace sf;

class ButtonSystem : public System{
    public:
        ButtonSystem();
        ~ButtonSystem();

        void update();

    private:
        void onButtonPressed(Entity* e);
        void onButtonReleased(Entity* e);
        void onButtonGainedFocus(Entity* e);
        void onKeyReleased(Entity* e);
        void onStartScreenTransition(Entity* e);
        void onLockAllButtons(Entity* e);
        void onUnlockAllButtons(Entity* e);
        void onNewScreen(Entity* e);
        void onToggleCheckBox(Entity* e);
        void onBringUILayerForward(Entity* e);
        void onWindowLostFocus(Entity* e);
        void onWindowGainedFocus(Entity* e);
        void onDoToggleAction(Entity* e);

        void updateButtonState(Entity* e);
        void updateButtonTexture(Entity* e);
        void updateButtonColor(Entity* e);
        void updateButtonRect(Entity* e);
        void updateButtonCircle(Entity* e);

        vector<EntityList> buttonsLocked;
        void lockButton(Entity* e);

        CUILayer::Layer topUILayer;

        bool windowFocused;
};

#endif // BUTTONSYSTEM_H
