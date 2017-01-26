#ifndef EDITORSYSTEM_H
#define EDITORSYSTEM_H

#include "System.h"
#include "Assets.h"

class EditorSystem : public System {
    public:
        EditorSystem();
        ~EditorSystem();

    private:

        void onInitializeEditor(Entity* e);
        void onSetPreviewBackground(Entity* e);
        void onAddObjectToPreview(Entity* e);
        void onDrop(Entity* e);
        void onBtEditorTest(Entity* e);
        void onBtEditorLoad(Entity* e);
        void onBtEditorClear(Entity* e);
        void onBtEditorSave(Entity* e);
        void onLoadScenarioPreview(Entity* e);
        void onSaveScenario(Entity* e);
        void onTestScenario(Entity* e);
        void onEndScenarioTest(Entity* e);

        void createBackgroundOptions();
        void createObjectOptions();
        void createMenuOptions();
        void saveScenario(string name);
        bool loadScenario(string name);
        void createScenario(Entity* e);
        void createPreview(CScenario scenario);
        void clearPreview();
        void createLoadInputBox();
        void createSaveInputBox();

        Entity* eBack;
        EntityList objects;

        double x0, y0;
        double xc, yc;
        double width, height;
        double xScale, yScale;

        Entity* eLoadPanel;
        Entity* eLoadInput;

        Entity* eSavePanel;
        Entity* eSaveInput;
};

#endif // EDITORSYSTEM_H
