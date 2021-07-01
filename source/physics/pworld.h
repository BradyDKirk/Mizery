/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#pragma once

#include "physics/pcontacts.h"
#include "physics/pfgen.h"

namespace Mizery {

	class ParticleWorld
	{
	public:
		ParticleWorld(uint32 maxContacts, uint32 iterations = 0);

		~ParticleWorld();

		// Init world for simulation frame
		void startFrame();

		// Calls each of the registered contact generators to report their contacts. Returns number of generated contacts
		uint32 generateContacts();

		// Integrates all particles in the world
		void integrate(real32 dt);

		// Processes all the physics for the particle world
		void runPhysics(real32 dt);

	private:

		bool calculateIterations;

		// Used in linked list to hold Particle information
		struct ParticleRegistration
		{
			Particle* particle;
			ParticleRegistration* next;
		};

		// List of particles
		ParticleRegistration* firstParticle;

		// Force generators for the particles in the world
		ParticleForceRegistry registry;

		// Resolver for contacts
		ParticleContactResolver resolver;

		// Hold one registered contact generator
		struct ContactGenRegistration
		{
			ParticleContactGenerator* gen;
			ContactGenRegistration* next;
		};

		// List of contact generators
		ContactGenRegistration* firstContactGen;

		// List of contacts
		ParticleContact* contacts;

		uint32 maxContacts;
	};

}