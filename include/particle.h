#ifndef PARTICLE_H
#define PARTICLE_H

#include "precision.h"
#include "vector.h"

typedef struct {
    V3 position;
    V3 velocity;
    V3 acceleration;
    V3 force;

    // How fast object loses speed (1 = 0 loss)
    real damping;

    /*
        It's more useful to hold the inverse mass because integration is simpler and because in real-time
        simulation it is more useful to have objects with infinite mass (immovable) than zero mass
        (completely unstable in numerical simulation).
    */
    real inverseMass; // look at page 48 if we haven't created setMass/setInverseMass
} Particle;

void particle_integrate(Particle* particle, real duration);

#endif