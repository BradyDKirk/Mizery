/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#pragma once

#include "physics/particle.h"

namespace Mizery {
    
    class ParticleContactResolver;

    class ParticleContact
    {
    public:
        Particle* particle[2];   // particles involved in contact. The second can be NULL for contact with scenery
        real32 restitution;       // normal restitution coefficient
        glm::vec3 contactNormal; // direction of contact in world coordinates
        real32 penetration;      // depth of penetration at contact

    protected:
        // Resolves this contact, for both velocity and interpenetration
        void resolve(real32 dt);

        // Calculates the separating velocity at this contact
        real32 calculateSeparatingVelocity() const;

    private:
        friend class ParticleContactResolver;

        // Handles the impulse calcuations for this collision
        void resolveVelocity(real32 dt);

        // Handles the interpenetration resolution for this contact
        void resolveInterpenetration(real32 dt);

    };

    class ParticleContactResolver
    {
    protected:
        uint32 iterations;     // Number of iterations allowed
        uint32 iterationsUsed; // Actual number of iterations used

    public:
        ParticleContactResolver(uint32 iterations);

        // Set number of iterations
        void setIterations(uint32 iterations);

        // Resolves a set of particle contacts for both penetration and velocity
        void resolveContacts(ParticleContact* contactArray, uint32 numContacts, real32 dt);

    };
    
    class ParticleContactGenerator
    {
    public:
        virtual uint32 addContact(ParticleContact* contact, uint32 limit) const = 0;
    };

}