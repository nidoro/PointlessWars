#include "CameraSystem.h"

CameraSystem::CameraSystem(){
    addRequirement(Component::CAMERA_MAN);
    addSubscription(SHAKE_CAMERA);
    shakeTime = 0.5;
    shaking = false;
    shakeIntensity = 8;
}

CameraSystem::~CameraSystem(){
    //dtor
}

void CameraSystem::start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS){
    this->eManager = eManager;
    this->window = window;
    this->targetUPS = targetUPS;
/*
    wWindow = window->getView().getSize().x;
    hWindow = window->getView().getSize().y;
    cxWindow = wWindow/2;
    cyWindow = hWindow/2;
*/
    wWindow = 1280;
    hWindow = 720;
    cxWindow = wWindow/2;
    cyWindow = hWindow/2;

    for(list<Message>::iterator i = subscripts.begin(); i != subscripts.end(); i++){
        subscribe(*i);
    }
}

void CameraSystem::update(){
    double wView = 1280;
    double hView = 720;

    if (shaking){
        sf::View view = sf::View(sf::FloatRect(0, 0, wView, hView));
        double w = wWindow-20;
        double h = w*9.f/16.f;
        view.setSize(w, h);
        sf::Vector2f center = sf::Vector2f(wWindow/2, hWindow/2);
        double d = shakeIntensity;
        view.setCenter(randomDouble(center.x - d/2, center.x + d/2), randomDouble(center.y - d/2, center.y + d/2));
        window->setView(view);

        if (shakeClock.getElapsedTime().asSeconds() >= shakeTime){
            shaking = false;
            view.setCenter(center.x, center.y);
            view.setSize(wWindow, hWindow);
            window->setView(view);
        }
    }else if (!entities.empty()){
        Entity* eCamMan = entities.front();
        if (!eManager->isDead(eCamMan)){
            if (eCamMan->has(Component::POSITION)){
                sf::View v(sf::FloatRect(0, 0, wView, hView));
                v.setCenter(eCamMan->get<CPosition>()->x, eCamMan->get<CPosition>()->y);
                window->setView(v);
            }
        }
    }else{
        sf::View view = sf::View(sf::FloatRect(0, 0, wView, hView));
        view.setCenter(cxWindow, cyWindow);
        window->setView(view);
    }
}

void CameraSystem::onShakeCamera(Entity* e){
    if (shaking){
        shakeIntensity = max(e->get<CCameraShake>()->intensity, shakeIntensity);
        shakeTime = max(e->get<CCameraShake>()->duration, shakeTime);
    }else{
        shaking = true;
        shakeIntensity = e->get<CCameraShake>()->intensity;
        shakeTime = e->get<CCameraShake>()->duration;
    }
    shakeClock.restart();
}
