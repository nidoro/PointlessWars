#ifndef ANNOUNCERSYSTEM_H
#define ANNOUNCERSYSTEM_H

#include "System.h"
#include "Assets.h"

class AnnouncerSystem : public System{
    public:
        AnnouncerSystem();
        ~AnnouncerSystem();

        void update();

    private:
        void createAnnouncer();
        void startTextTransition();

        void onInitializeWar(Entity* e);
        string getGameStateString();
        string currentString;
        string nextString;

        Entity* eText;
        Entity* eBanner;
        bool active;

};

#endif // ANNOUNCERSYSTEM_H
