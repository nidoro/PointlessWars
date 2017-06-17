#ifndef FORMATIONEDITOR_H
#define FORMATIONEDITOR_H

#include "System.h"
#include "Assets.h"

class FormationEditor : public System {
    public:
        FormationEditor();
        ~FormationEditor();

        void update();

    private:
        void onCreateScreen(Entity* e);
        void onLoadFormation(Entity* e);
        void onSaveFormation(Entity* e);
        void onBtFormationSave(Entity* e);
        void onClearFormationEditor(Entity* e);
        void onDeleteFormation(Entity* e);

        void create();
        void createGrid();
        void createUnits();
        void createMenu();
        void updateLoadOptions();
        void clearPositions();
        void createSaveInputTextBox();

        Entity* createButton(string label, double w, double h, double x, double y, Message m = EMPTY_MESSAGE);

        void clearGrid();

        vector< vector<Entity*> > positions;
        vector< vector<Entity*> > order;
        vector< vector<Entity*> > units;
        map<CArmy::Formation, Entity*> formationButtons;

        bool active;

        Entity* eLoad;
        Entity* eSave;

        list<CFormation> formations;
};

#endif // FORMATIONEDITOR_H
