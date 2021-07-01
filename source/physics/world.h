/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#pragma once

#include "physics/body.h"
#include "physics/fgen.h"

namespace Mizery {
    
    class World
    {
    public:
        void startFrame();
        
        void runPhysics(real32 dt);
        
    private:
        struct BodyRegistration
        {
            RigidBody* body;
            BodyRegistration* next;
        };
        BodyRegistration* firstBody;
        
        // Force generators for the particles in the world
		ForceRegistry registry;
        
    };
    
}