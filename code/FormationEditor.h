#ifndef FORMATIONEDITOR_H
#define FORMATIONEDITOR_H

#include "System.h"
#include "Assets.h"

class FormationEditor : public System {
    public:
        FormationEditor();
        ~FormationEditor();

        void update();
        void start();
        void shutdown();

    private:
        void onCreateScreen(Entity* e);
        void onLoadFormation(Entity* e);
        void onSaveFormation(Entity* e);
        void onBtFormationSave(Entity* e);
        void onClearFormationEditor(Entity* e);
        void onDeleteFormation(Entity* e);
        void onEditFormation(Entity* e);
        void onRestoreDefaultFormation(Entity* e);
        void onGameStarted(Entity* e);
        void onAppEnding(Entity* e);
        void onEditFormationPosition(Entity* e);

        void create();
        void createGrid();
        void createUnits();
        void createMenu();
        void updateLoadOptions();
        void clearPositions();
        void createSaveInputTextBox();
        void editFormation(CArmy::Formation formation);
        void setDefaultFormation(CArmy::Formation formation, int p[23][15]);
        void createDefaultFormations();

        Entity* createButton(string label, double w, double h, double x, double y, Message m = EMPTY_MESSAGE);
        Entity* createRectButton(string label, double fontSize, double h, double x, double y,
                                 sf::Color textColor, sf::Color fillColor, sf::Color outColor, double outThickness, Message m, CUILayer::Layer UILayer, bool unavailable = false);

        void clearGrid();

        vector< vector<Entity*> > positions;
        vector< vector<Entity*> > order;
        vector< vector<Entity*> > units;
        CArmy::Formation nowEditing;

        bool active;

        Entity* eLoad;
        Entity* eSave;
        
        nlohmann::json dataFile;

        std::map<CArmy::Formation, CFormation> formations;
        std::map<CArmy::Formation, CFormation> defaultFormations;
};

#endif // FORMATIONEDITOR_H
