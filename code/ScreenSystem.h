#ifndef SCREENSYSTEM_H
#define SCREENSYSTEM_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"

class ScreenSystem : public System {
    public:
        ScreenSystem();
        ~ScreenSystem();

        void update();

    private:
        void onCreateScreen(Entity* e);
        void onStartScreenTransition(Entity* e);
        void onBtNewGame(Entity* e);
        void onGameStarted(Entity* e);
        void onOpenMenu(Entity* e);
        void onResumeGame(Entity* e);
        void onNewCommandLine(Entity* e);
        void onKeyPressed(Entity* e);
        void onUpdateFullscreenWithToggleButton(Entity* e);
        void onUpdateResolutionWithDropList(Entity* e);
        void onChooseLanguage(Entity* e);

        void createMainMenu(Entity* e);
        void createSplash1(Entity* e);
        void createSplash2(Entity* e);
        void createMatch(Entity* e);
        void createCustomMatch(Entity* e);
        void createLanguageMenu(Entity* e);
        Entity* createFlagButton(std::string flag, std::string lan, double x, double y);


};

#endif // SCREENSYSTEM_H
