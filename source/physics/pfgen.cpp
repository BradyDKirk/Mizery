/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#include "pfgen.h"

namespace Mizery {
    
    // REGISTRY
    // =================================================================================
    
    void ParticleForceRegistry::updateForces(real32 dt)
    {
        Registry::iterator i = registrations.begin();
        for (; i != registrations.end(); i++)
        {
            i->fg->updateForce(i->particle, dt);
        }
    }
    
    void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
    {
        ParticleForceRegistry::ParticleForceRegistration registration;
        registration.particle = particle;
        registration.fg = fg;
        registrations.push_back(registration);
    }
    
    // GENERATORS
    // =================================================================================
    
    // GRAVITY
    ParticleGravity::ParticleGravity(const glm::vec3& gravity) : gravity(gravity) {}
    
    void ParticleGravity::updateForce(Particle* particle, real32 dt)
    {
        if (!particle->hasFiniteMass()) { return; }
        
        particle->addForce(gravity * particle->getMass());
    }
    
    // DRAG
    ParticleDrag::ParticleDrag(real32 k1, real32 k2) : k1(k1), k2(k2) {}
    
    void ParticleDrag::updateForce(Particle* particle, real32 dt)
    {
        glm::vec3 force = particle->velocity;
        
        // Calculate the total drag coefficient
        real32 dragCoeff = glm::length(force);
        dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;
        
        // Calculate the final force and apply it
        force = glm::normalize(force);
        force *= -dragCoeff;
        particle->addForce(force);
    }
    
    // Spring
    ParticleSpring::ParticleSpring(Particle* other, real32 springConstant, real32 restLength) : other(other), springConstant(springConstant), restLength(restLength) {}
    
    void ParticleSpring::updateForce(Particle* particle, real32 dt)
    {
        // Calculate vector of spring
        glm::vec3 force = particle->position - other->position;
        
        // Calculate the magnitude of the force
        real32 magnitude = glm::length(force);
        magnitude = fabsf(magnitude - restLength);
        magnitude *= springConstant;
        
        // Calculate the final force
        force = glm::normalize(force);
        force *= -magnitude;
        particle->addForce(force);
    }
    
    // Anchored Spring
    ParticleAnchoredSpring::ParticleAnchoredSpring(glm::vec3* anchor, real32 springConstant, real32 restLength) : anchor(anchor), springConstant(springConstant), restLength(restLength) {}
    
    void ParticleAnchoredSpring::updateForce(Particle* particle, real32 dt)
    {
        // Calculate vector of spring
        glm::vec3 force = particle->position - *anchor;
        
        // Calculate the magnitude of the force
        real32 magnitude = glm::length(force);
        magnitude = (restLength - magnitude) * springConstant;
        
        // Calculate the final force
        force = glm::normalize(force);
        force *= magnitude;
        particle->addForce(force);
    }
    
    // Bungee
    ParticleBungee::ParticleBungee(Particle* other, real32 springConstant, real32 restLength) : other(other), springConstant(springConstant), restLength(restLength) {}
    
    void ParticleBungee::updateForce(Particle* particle, real32 dt)
    {
        // Calculate vector of spring
        glm::vec3 force = particle->position - other->position;
        
        // Check if the bungee is compressed
        real32 magnitude = glm::length(force);
        if (magnitude <= restLength) { return; }
        
        // Calculate the magnitude of the force
        magnitude = springConstant * (restLength - magnitude);
        
        // Calculate the final force
        force = glm::normalize(force);
        force *= -magnitude;
        particle->addForce(force);
    }
    
    // Anchored Bungee
    ParticleAnchoredBungee::ParticleAnchoredBungee(glm::vec3* anchor, real32 springConstant, real32 restLength) : ParticleAnchoredSpring(anchor, springConstant, restLength) {}

    void ParticleAnchoredBungee::updateForce(Particle* particle, real32 dt)
    {
        // Calculate vector of spring
        glm::vec3 force = particle->position - *anchor;

        // Check if the bungee is compressed
        real32 magnitude = glm::length(force);
        if (magnitude < restLength) { return; }

        magnitude = springConstant * (magnitude - restLength);

        // Calculate the final force
        force = glm::normalize(force);
        force *= -magnitude;
        particle->addForce(force);
    }

    // Buoyancy
    ParticleBuoyancy::ParticleBuoyancy(real32 maxDepth, real32 volume, real32 waterHeight, real32 liquidDensity) : maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) {}
    
    void ParticleBuoyancy::updateForce(Particle* particle, real32 dt)
    {
        // Calculate submersion depth
        real32 depth = particle->position.y;
        
        // Check if we're out of the water
        if (depth >= waterHeight + maxDepth) { return; }
        glm::vec3 force(0.0f);
        
        // Check if we're at the maximum depth
        if (depth <= waterHeight - maxDepth)
        {
            force.y = liquidDensity * volume;
            particle->addForce(force);
            return;
        }
        
        // Otherwise we are particle submerged
        force.y = liquidDensity * volume * (depth - maxDepth - waterHeight) / (2 * maxDepth);
        particle->addForce(force);
    }
    
    // Fake spring (for stiff springs)
    ParticleFakeSpring::ParticleFakeSpring(glm::vec3* anchor, real32 springConstant, real32 damping) : anchor(anchor), springConstant(springConstant), damping(damping) {}
    
    void ParticleFakeSpring::updateForce(Particle* particle, real32 dt)
    {
        // Check if the mass is infinite
        if (!particle->hasFiniteMass()) { return; }
        
        // Calculate the relative position of the particle to the anchor
        glm::vec3 position = particle->position - *anchor;
        
        // Calculate the constants and check whether they are in bounds
        real32 gamma = 0.5f * sqrtf((4 * springConstant) - (damping * damping));
        if (gamma == 0.0f) { return; }
        glm::vec3 c = position * (damping / (2.0f * gamma)) + particle->velocity * (1.0f / gamma);
        
        // Calculate target position
        glm::vec3 target = position * cosf(gamma * dt) + c * sinf(gamma * dt);
        target = target * expf(-0.5f * dt * damping);
        
        // Calculate the resulting acceleration and therefore the force
        glm::vec3 accel = (target - position) * (1.0f / (dt * dt)) - particle->velocity * (1.0f / dt);
        particle->addForce(accel * particle->getMass());
    }
    
}