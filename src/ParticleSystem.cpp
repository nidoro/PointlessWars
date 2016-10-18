#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
    addRequirement(Component::PARTICLE_EMMITER);
}

ParticleSystem::~ParticleSystem(){
    //dtor
}

void ParticleSystem::update(){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        if (!e->get<CParticleEmmiter>()->on) continue;
        if (e->get<CParticleEmmiter>()->clock.getElapsedTime().asSeconds() >= 1.f/e->get<CParticleEmmiter>()->rate){
            for(int c = 0; c < e->get<CParticleEmmiter>()->nParticles; c++){
                emmitParticle(e);
            }
            e->get<CParticleEmmiter>()->clock.restart();
        }
    }
}

void ParticleSystem::emmitParticle(Entity* e){
    Entity* eParticle = eManager->createEntity();
    eParticle->add(new CPosition(e->get<CPosition>()->x, e->get<CPosition>()->y));
    eParticle->add(new CDraw(CDraw::PARTICLES));
    eParticle->add(new CAutoPilot());
    eParticle->add(new CTimer(e->get<CParticleEmmiter>()->lifeSpan, REMOVE_ENTITY));

    double midAngle = e->get<CParticleEmmiter>()->angle;
    double angleGap = e->get<CParticleEmmiter>()->angleGap;
    double angle = randomDouble(midAngle-angleGap/2, midAngle+angleGap/2)*M_RAD;
    double speed = randomDouble(e->get<CParticleEmmiter>()->minSpeed, e->get<CParticleEmmiter>()->maxSpeed);

    eParticle->add(new CVelocity(0, 0, speed*cos(angle), speed*sin(angle)));
    eParticle->add(new CAcceleration(0, 0, e->get<CParticleEmmiter>()->xAcc, e->get<CParticleEmmiter>()->yAcc));

    switch(e->get<CParticleEmmiter>()->drawable){
        case CParticleEmmiter::RECTANGLE: eParticle->add(new CRectShape(e->get<CParticleEmmiter>()->rectShape)); break;
        case CParticleEmmiter::CIRCLE: eParticle->add(new CCircleShape(e->get<CParticleEmmiter>()->circleShape)); break;
        case CParticleEmmiter::TEXTURE: eParticle->add(new CTexture(e->get<CParticleEmmiter>()->texture)); break;
        default: break;
    }
}



