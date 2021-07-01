/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#include "pworld.h"

namespace Mizery {

	ParticleWorld::ParticleWorld(uint32 maxContacts, uint32 iterations) : resolver(iterations), maxContacts(maxContacts)
	{
		contacts = new ParticleContact[maxContacts];
		calculateIterations = (iterations == 0);
	}

	ParticleWorld::~ParticleWorld()
	{
		delete[] contacts;
	}

	void ParticleWorld::startFrame()
	{
		ParticleRegistration* reg = firstParticle;
		while (reg)
		{
			reg->particle->clearAccumulator();
			reg = reg->next;
		}
	}

	uint32 ParticleWorld::generateContacts()
	{
		uint32 limit = maxContacts;
		ParticleContact* nextContact = contacts;

		ContactGenRegistration* reg = firstContactGen;
		while (reg)
		{
			uint32 used = reg->gen->addContact(nextContact, limit);
			limit -= used;
			nextContact += used;

			// We've run out of contacts to fill
			if (limit <= 0) { break; }

			reg = reg->next;
		}

		// Return nubmer of contacts used
		return maxContacts - limit;
	}

	void ParticleWorld::integrate(real32 dt)
	{
		ParticleRegistration* reg = firstParticle;
		while (reg)
		{
			reg->particle->integrate(dt);
			reg = reg->next;
		}
	}

	void ParticleWorld::runPhysics(real32 dt)
	{
		registry.updateForces(dt);

		integrate(dt);

		uint32 usedContacts = generateContacts();

		if (calculateIterations) resolver.setIterations(usedContacts * 2);
		resolver.resolveContacts(contacts, usedContacts, dt);
	}

}