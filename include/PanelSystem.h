#ifndef PANELSYSTEM_H
#define PANELSYSTEM_H

#include "System.h"

class PanelSystem : public System{
    public:
        PanelSystem();
        ~PanelSystem();

        void update();

    private:
        void onBeginPanel(Entity* e);
        void onEndPanel(Entity* e);
        void onRemoveSiblings(Entity* e);
        void onRemoveCousins(Entity* e);
        void onRemoveNephews(Entity* e);
        void onRemoveParent(Entity* e);
        void onRemoveChildren(Entity* e);

        void onSetTooltip(Entity* e);
        void onSetWindow(Entity* e);
        void onAttachEntityToPanel(Entity* e);

        void onRemovePanel(Entity* e);
        void onClosePanel(Entity* e);

        Entity* ePanel;
};

#endif // PANELSYSTEM_H
