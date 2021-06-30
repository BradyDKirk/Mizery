#pragma once

#include "core/math.h"
#include "core/typedefs.h"

namespace Mizery {
    
    class Particle
    {
    public:
        real32 damping;
        real32 inverseMass;
        
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        
        void integrate(real32 dt);
        
        void addForce(const glm::vec3 force);
        void clearAccumulator();
        
        bool32 hasFiniteMass() const;
        
        void setMass(const real32 mass);
        real32 getMass() const;
        
    private:
        glm::vec3 forceAccum = glm::vec3(0.0f);
    };

}