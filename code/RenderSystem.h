#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"
#include <SFML/Graphics.hpp>
#include "Standard.h"
#include "Assets.h"
#include "DebugServices.h"

class RenderSystem : public System{
    public:
        RenderSystem();
        ~RenderSystem();

        void update();
        void start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS);
        void updateEntities();
        void draw(Entity* e);
        void drawNinePatch(Entity* e);
        CDraw::Tag getTag(EntityList& eList);

    private:
        void onKeyReleased(Entity* e);

        vector<EntityList> eLists;
        EntityList eWorld;

        bool isDrawingDebug;

};

#endif // RENDERSYSTEM_H
