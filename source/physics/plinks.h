/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#pragma once

#include "physics/pcontacts.h"

namespace Mizery {
    
    class ParticleLink : public ParticleContactGenerator
    {
    public:
        Particle* particle[2]; // pair of particles connected

        // Fill contact structure
        virtual uint32 addContact(ParticleContact* contact, uint32 limit) const = 0;

    protected:
        real32 currentLength() const; // length of link (cable, rod, etc.)
    };
    
    class ParticleCable : public ParticleLink
    {
    public:
        real32 maxLength;   // max length of cable
        real32 restitution; // restitution of cable

        virtual uint32 addContact(ParticleContact* contact, uint32 limit) const;
    };

    class ParticleRod : public ParticleLink
    {
    public:
        real32 length; // length of rod

        virtual uint32 addContact(ParticleContact* contact, uint32 limit) const;
    };

}