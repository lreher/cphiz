#include <stdio.h>
#include "precision.h"
#include "vector.h"
#include "particle.h"

void particle_integrate (Particle* particle, real duration) {
    // assert(duration > 0.0); ??

    // Update linear position
    v3_add_scaled(&particle->position, particle->velocity, duration);

    // Calculate acceleration from force vector
    V3 resultingAcc = particle->acceleration;
    v3_add_scaled(&resultingAcc, particle->force, particle->inverseMass);

    // Update resulting velocity from acceleration
    v3_add_scaled(&particle->velocity, resultingAcc, duration);

    // Add dampening
    v3_multiply(&particle->velocity, real_pow(particle->damping, duration));
}