#ifndef PATHSYSTEM_H
#define PATHSYSTEM_H

#include "System.h"
#include "Standard.h"

class PathSystem : public System{
    public:
        PathSystem();
        ~PathSystem();

        void update();
    private:

        sf::Vector2f pollNextNode(Entity* e);
};

#endif // PATHSYSTEM_H
