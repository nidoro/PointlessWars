#ifndef SPINBUTTONSYSTEM_H
#define SPINBUTTONSYSTEM_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

struct SpinButtonStyle {
    enum Align {VERTICAL, HORIZONTAL};

    string btIncreaseDef;
    string btIncreaseHov;
    string btIncreaseAct;

    string btDecreaseDef;
    string btDecreaseHov;
    string btDecreaseAct;

    double width, height;
    double distance;
    Align align;
};

class SpinButtonSystem : public System {
    public:
        SpinButtonSystem();
        ~SpinButtonSystem();

        void update();

    private:
        void onCreateSpinButtons(Entity* e);
        void onChangeSpinValue(Entity* e);
        void onIncreaseSpinButtonValue(Entity* e);
        void onDecreaseSpinButtonValue(Entity* e);

        vector<SpinButtonStyle> styles;
};

#endif // SPINBUTTONSYSTEM_H
