#include "pcontacts.h"

namespace Mizery {
    
    void ParticleContact::resolve(real32 dt)
    {
        resolveVelocity(dt);
        resolveInterpenetration(dt);
    }

    real32 ParticleContact::calculateSeparatingVelocity() const
    {
        glm::vec3 relativeVelocity = particle[0]->velocity;
        if (particle[1])
        {
            relativeVelocity = relativeVelocity - particle[1]->velocity;
        }
        return glm::dot(relativeVelocity, contactNormal);
    }

    void ParticleContact::resolveVelocity(real32 dt)
    {
        // Find the velocity in the direction of the contact
        real32 separatingVelocity = calculateSeparatingVelocity();

        // Check whether it needs to be resolved
        if (separatingVelocity > 0) { return; }

        // Calculate the new seperating velocity
        real32 newSepVelocity = -separatingVelocity * restitution;

        // Check the velocity build-up due to acceleration only
        glm::vec3 accCausedVelocity = particle[0]->acceleration;
        if (particle[1])
        {
            accCausedVelocity = accCausedVelocity - particle[1]->acceleration;
        }
        real32 accCausedSepVelocity = glm::dot(accCausedVelocity, contactNormal) * dt;

        // If we've got a closing velocity due to acceleration build-up, remove it from the new separating velocity
        if (accCausedSepVelocity < 0)
        {
            newSepVelocity += restitution * accCausedSepVelocity;

            // Make sure we haven't removed more than was there to remove
            if (newSepVelocity < 0) { newSepVelocity = 0; }
        }

        real32 deltaVelocity = newSepVelocity - separatingVelocity;

        // Apple the change in velocity to each object in proportion to its inverse mass
        real32 totalInverseMass = particle[0]->inverseMass;
        if (particle[1])
        {
            totalInverseMass += particle[1]->inverseMass;
        }

        // If all particles have infinite mass, then impulse have no effect
        if (totalInverseMass <= 0) { return; }

        // Calcualte the impulse
        real32 impulse = deltaVelocity / totalInverseMass;

        // Find the amount of impulse per unit of inverse mass
        glm::vec3 impulsePerIMass = contactNormal * impulse;

        // Apply impulses: they are applied in the direction of the contact, and are proportional to the inverse mass
        particle[0]->velocity = particle[0]->velocity + impulsePerIMass * particle[0]->inverseMass;
        if (particle[1])
        {
            particle[1]->velocity = particle[1]->velocity + impulsePerIMass * -particle[1]->inverseMass;
        }
    }
    
    void ParticleContact::resolveInterpenetration(real32 dt)
    {
        // If we don't have any penetration
        if (penetration <= 0) { return; }

        // Calculate total inverse mass
        real32 totalInverseMass = particle[0]->inverseMass;
        if (particle[1])
        {
            totalInverseMass += particle[1]->inverseMass;
        }

        // If both particles have infinite mass then do nothing
        if (totalInverseMass <= 0) { return; }

        // Find amount of penetration resolution per unit of inverse mass
        glm::vec3 movePerIMass = contactNormal * (-penetration / totalInverseMass);

        // Apply the penetration resolution
        particle[0]->position = particle[0]->position + movePerIMass * particle[0]->inverseMass;
        if (particle[1])
        {
            particle[1]->position = particle[1]->position + movePerIMass * particle[1]->inverseMass;
        }
    }

    ParticleContactResolver::ParticleContactResolver(uint32 iterations) : iterations(iterations) {}

    void ParticleContactResolver::setIterations(uint32 iterations)
    {
        ParticleContactResolver::iterations = iterations;
    }

    void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, uint32 numContacts, real32 dt)
    {
        iterationsUsed = 0;
        while (iterationsUsed < iterations)
        {
            // Find the contact with the largest closing velocity
            real32 max = 0;
            uint32 maxIndex = numContacts;
            for (uint32 i = 0; i < numContacts; i++)
            {
                real32 sepVel = contactArray[i].calculateSeparatingVelocity();
                if (sepVel < max)
                {
                    max = sepVel;
                    maxIndex = i;
                }
            }

            // Resolve this contact
            contactArray[maxIndex].resolve(dt);
            iterationsUsed++;
        }
    }

}