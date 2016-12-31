#ifndef OPTIONBOXSYSTEM_H
#define OPTIONBOXSYSTEM_H

#include "System.h"

class OptionBoxSystem : public System{
    public:
        OptionBoxSystem();
        ~OptionBoxSystem();
        void update();

    private:
        void onExpandOptionBox(Entity* e);
        void onRetractOptionBox(Entity* e);
        void onSelectOptionBox(Entity* e);

        void setCellTexture(Entity* e, COptionBox::Data data);
        void setCellTooltip(Entity* e, COptionBox::Data data);
};

#endif // OPTIONBOXSYSTEM_H
