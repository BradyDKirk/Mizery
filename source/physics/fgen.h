/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#pragma once

#include <vector>

#include "body.h"

namespace Mizery {
    
    class ForceGenerator
    {
    public:
        virtual void updateForce(RigidBody* body, real32 dt) = 0;
    };
    
    class ForceRegistry
    {
    public:
        void updateForces(real32 dt);
        
        void add(RigidBody* body, ForceGenerator* fg);
        void remove(RigidBody* body, ForceGenerator* fg);
        void clear();
        
    private:
        struct ForceRegistration
        {
            RigidBody* body;
            ForceGenerator* fg;
        };
        
        typedef std::vector<ForceRegistration> Registry;
        Registry registrations;
    };
    
    class Gravity : public ForceGenerator
    {
    public:
        Gravity(const glm::vec3& gravity);
        
        virtual void updateForce(RigidBody* body, real32 dt);
        
    private:
        glm::vec3 gravity;
    };
    
    class Spring : public ForceGenerator
    {
    public:
        Spring(const glm::vec3& localConnectionPoint, RigidBody* other, const glm::vec3& otherConnectionPoint, real32 springConstant, real32 restLength);
        
        virtual void updateForce(RigidBody* body, real32 dt);
    
    private:
        glm::vec3 connectionPoint;
        
        glm::vec3 otherConnectionPoint;
        RigidBody* other;
        
        real32 springConstant;
        real32 restLength;
    };
    
}