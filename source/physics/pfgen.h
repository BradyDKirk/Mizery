#pragma once

#include "core/typedefs.h"
#include "physics/particle.h"

#include <vector>

namespace Mizery {
    
    class ParticleForceGenerator
    {
    public:
        virtual void updateForce(Particle* particle, real32 dt) = 0;
    };
    
    class ParticleForceRegistry
    {
    public:
        void updateForces(real32 dt);
        
        void add(Particle* particle, ParticleForceGenerator* fg);
        void remove(Particle* particle, ParticleForceGenerator* fg);
        void clear();
        
    protected:
        struct ParticleForceRegistration
        {
            Particle* particle;
            ParticleForceGenerator* fg;
        };
        
        typedef std::vector<ParticleForceRegistration> Registry;
        Registry registrations;
    };
    
    class ParticleGravity : public ParticleForceGenerator
    {
    public:
        ParticleGravity(const glm::vec3& gravity);
        
        virtual void updateForce(Particle* particle, real32 dt);
        
    private:
        glm::vec3 gravity; // acceleration due to gravity
    };
    
    class ParticleDrag : public ParticleForceGenerator
    {
    public:
        ParticleDrag(real32 k1, real32 k2);
        
        virtual void updateForce(Particle* particle, real32 dt);
        
    private:
        real32 k1; // linear drag coefficient
        real32 k2; // squared drag coefficient
    };
    
    class ParticleSpring : public ParticleForceGenerator
    {
    public:
        ParticleSpring(Particle* other, real32 springConstant, real32 restLength);
        
        virtual void updateForce(Particle* particle, real32 dt);
        
    private:
        Particle* other;       // particle at other end of spring
        real32 springConstant; // spring constant
        real32 restLength;     // rest length of spring
    };
    
    class ParticleAnchoredSpring : public ParticleForceGenerator
    {
    public:
        ParticleAnchoredSpring(glm::vec3* anchor, real32 springConstant, real32 restLength);
        
        virtual void updateForce(Particle* particle, real32 dt);
        
        const glm::vec3* getAnchor() const { return anchor; }
        
    protected:
        glm::vec3* anchor;     // fixed anchor at one end of the spring
        real32 springConstant; // spring constant
        real32 restLength;     // rest length of spring
    };
    
    class ParticleBungee : public ParticleForceGenerator
    {
    public:
        ParticleBungee(Particle* other, real32 springConstant, real32 restLength);
    
        virtual void updateForce(Particle* particle, real32 dt);
    
    private:
        Particle* other;       // particle at other end of spring
        real32 springConstant; // spring constant
        real32 restLength;     // length at which bungee will behave like a spring
    };
    
    class ParticleAnchoredBungee : public ParticleAnchoredSpring
    {
    public:
        ParticleAnchoredBungee(glm::vec3* anchor, real32 springConstant, real32 restLength);

        virtual void updateForce(Particle* particle, real32 dt);
    };

    class ParticleBuoyancy : public ParticleForceGenerator
    {
    public:
        ParticleBuoyancy(real32 maxDepth, real32 volume, real32 waterHeight, real32 liquidDensity = 1000.0f);
    
        virtual void updateForce(Particle* particle, real32 dt);
    
    private:
        real32 maxDepth;      // The maximum submersion depth of the object before it generates maximum buoyancy force
        real32 volume;        // Volume of object
        real32 waterHeight;   // Height of water plane above y=0
        real32 liquidDensity; // Density of liquid
    };
    
    class ParticleFakeSpring : public ParticleForceGenerator
    {
    public:
        ParticleFakeSpring(glm::vec3* anchor, real32 springConstant, real32 damping);
        
        virtual void updateForce(Particle* particle, real32 dt);
        
    private:
        glm::vec3* anchor;     // anchored end of the spring
        real32 springConstant; // spring constant
        real32 damping;        // damping on the oscillation of the spring
    };
    
}