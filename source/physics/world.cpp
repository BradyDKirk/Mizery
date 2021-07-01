/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/

#include "world.h"

namespace Mizery {
    
    void World::startFrame()
    {
        BodyRegistration* reg = firstBody;
        while (reg)
        {
            reg->body->clearAccumulators();
            reg->body->calculateDerivedData();
            
            reg = reg->next;
        }
    }
    
    void World::runPhysics(real32 dt)
    {
        registry.updateForces(dt);
        
        BodyRegistration* reg = firstBody;
        while (reg)
        {
            reg->body->integrate(dt);
            
            reg = reg->next;
        }
    }
    
}