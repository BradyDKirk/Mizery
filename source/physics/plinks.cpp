#include "plinks.h"

namespace Mizery {
    
    real32 ParticleLink::currentLength() const
    {
        glm::vec3 relativePos = particle[0]->position - particle[1]->position;
        return glm::length(relativePos);
    }
    
    uint32 ParticleCable::addContact(ParticleContact* contact, uint32 limit) const
    {
        // Find length of cable
        real32 length = currentLength();

        // Check whether we're overextended
        if (length < maxLength) { return 0; }

        // Otherwise return the contact
        contact->particle[0] = particle[0];
        contact->particle[1] = particle[1];

        // Calculate the normal
        glm::vec3 normal = particle[1]->position - particle[0]->position;
        normal = glm::normalize(normal);
        contact->contactNormal = normal;

        contact->penetration = length - maxLength;
        contact->restitution = restitution;

        return 1;
    }

    uint32 ParticleRod::addContact(ParticleContact* contact, uint32 limit) const
    {
        // Find the length of the rod
        real32 currentLen = currentLength();

        // Check whether we're overextended
        if (currentLen == length) { return 0; }

        // Otherwise return the contact
        contact->particle[0] = particle[0];
        contact->particle[1] = particle[1];

        // Calculate the normal
        glm::vec3 normal = particle[1]->position - particle[0]->position;
        normal = glm::normalize(normal);

        // The contact normal depends on whether we're extending or compressed
        if (currentLen > length)
        {
            contact->contactNormal = normal;
            contact->penetration = currentLen - length;
        }
        else
        {
            contact->contactNormal = normal * -1.0f;
            contact->penetration = length - currentLen;
        }

        // Always use zero restitution
        contact->restitution = 0;

        return 1;
    }

}