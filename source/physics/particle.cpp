#include "particle.h"

namespace Mizery {
    
    void Particle::integrate(real32 dt)
    {
        // Calculate new position
        position += velocity * dt;
        
        // Calculate new velocity
        glm::vec3 newAcc = acceleration;
        newAcc += inverseMass * forceAccum;
        velocity += newAcc * dt;
        
        // Account for damping
        velocity *= powf(damping, dt);
        
        // Clear forces
        clearAccumulator();
    }
    
    void Particle::addForce(glm::vec3 force)
    {
        forceAccum += force;
    }
    
    void Particle::clearAccumulator()
    {
        forceAccum = glm::vec3(0.0f);
    }
    
    bool32 Particle::hasFiniteMass() const
    {
        return inverseMass >= 0.0f;
    }
    
    void Particle::setMass(const real32 mass)
    {
        assert(mass != 0);
        inverseMass = 1.0f / mass;
    }
    
    real32 Particle::getMass() const
    {
        if (inverseMass == 0)
        {
            return FLT_MAX;
        }
        else
        {
            return 1.0f / inverseMass;
        }
    }
    
}