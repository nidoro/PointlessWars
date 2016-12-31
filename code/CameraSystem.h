#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "System.h"


class CameraSystem : public System{
    public:
        CameraSystem();
        ~CameraSystem();

        void update();
        void start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS);

    private:
        void onShakeCamera(Entity* e);

        double shakeIntensity;
        sf::Clock shakeClock;
        double shakeTime;

        bool shaking;
};

#endif // CAMERASYSTEM_H
