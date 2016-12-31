#ifndef INGAMEMENU_H
#define INGAMEMENU_H

#include "System.h"
#include "Assets.h"

class InGameMenu : public System{
    public:
        InGameMenu();
        ~InGameMenu();


    private:
        Entity* ePanel;
        vector< list<Entity*> > pages;

        void onOpenMenu(Entity* e);
        void onResumeGame(Entity* e);
        void onChangeMenuPage(Entity* e);
        void onHasBeenToggled(Entity* e);
        void onHasChangedValue(Entity* e);

        CInGameMenuPage::ID getPage(Entity* e);

        void create();
        Entity* createButton(string label, double w, double h, double x, double y, Message m = EMPTY_MESSAGE);
        Entity* createRectButton(string label, double fontSize, double h, double x, double y, sf::Color textColor, sf::Color outColor, double outThickness, Message m);
        void addToPage(Entity* e, CInGameMenuPage::ID);
        void showPage(CInGameMenuPage::ID id);
        void clearPage(CInGameMenuPage::ID id);

        CUILayer::Layer UILayer;

};

#endif // INGAMEMENU_H
