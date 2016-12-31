#ifndef MAINMENUSYSTEM_H
#define MAINMENUSYSTEM_H

#include "System.h"
#include "Assets.h"

class MainMenuSystem : public System{
    public:
        MainMenuSystem();
        ~MainMenuSystem();

        void update();

    private:
        void onCreateScreen(Entity* e);
        void onChangeMenuPage(Entity* e);
        void onCreatePlayerEditor(Entity* e);
        void onCreateProfile(Entity* e);
        void onSelectCharacter(Entity* e);
        void onCreateProfileMenu(Entity* e);
        void onEditProfile(Entity* e);
        void onDeleteProfile(Entity* e);
        void onSelectProfile(Entity* e);

        vector< list<Entity*> > pages;
        void create();
        void createMenu(bool animated);
        Entity* createButton(string label, double w, double h, double x, double y, Message m = EMPTY_MESSAGE);
        void addToPage(Entity* e, CMainMenuPage::ID);
        void showPage(CMainMenuPage::ID id);
        void updatePlayers();
        void clearPage(CMainMenuPage::ID id);
        void createPlayerSelect();
        void saveProfile(Entity* e);
        void deleteProfile(Entity* e);
        void loadProfiles();

        list<CProfile> profiles;

};

#endif // MAINMENUSYSTEM_H
