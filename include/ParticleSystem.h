#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "System.h"


class ParticleSystem : public System{
    public:
        ParticleSystem();
        ~ParticleSystem();

        void update();

    private:

        void emmitParticle(Entity* e);

};

#endif // PARTICLESYSTEM_H
