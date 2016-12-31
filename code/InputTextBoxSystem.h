#ifndef INPUTTEXTBOXSYSTEM_H
#define INPUTTEXTBOXSYSTEM_H

#include "System.h"

class InputTextBoxSystem : public System{
    public:
        InputTextBoxSystem();
        ~InputTextBoxSystem();

        void update();

    private:
        void onTextEntered(Entity* e);
        void onActivateInputTextBox(Entity* e);
        void onMouseButtonPressed(Entity* e);
        void onDeactivateInputTextBox(Entity* e);

        Entity* eActive;
};

#endif // INPUTTEXTBOXSYSTEM_H
