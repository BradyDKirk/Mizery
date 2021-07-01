/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/

#include "fgen.h"

namespace Mizery {
    
    void ForceRegistry::updateForces(real32 dt)
    {
        Registry::iterator i = registrations.begin();
        for (; i != registrations.end(); i++)
        {
            i->fg->updateForce(i->body, dt);
        }
    }
    
    void ForceRegistry::add(RigidBody* body, ForceGenerator* fg)
    {
        ForceRegistry::ForceRegistration registration;
        registration.body = body;
        registration.fg = fg;
        registrations.push_back(registration);
    }
    
    Gravity::Gravity(const glm::vec3& gravity) : gravity(gravity) {}
    
    void Gravity::updateForce(RigidBody* body, real32 dt)
    {
        // Return if infinite mass
        if (!body->hasFiniteMass()) { return; }
        
        // Apply mass-scaled force
        body->addForce(gravity * body->getMass());
    }
    
    Spring::Spring(const glm::vec3& localConnectionPoint, RigidBody* other, const glm::vec3& otherConnectionPoint, real32 springConstant, real32 restLength) 
        : connectionPoint(localConnectionPoint), other(other), otherConnectionPoint(otherConnectionPoint), springConstant(springConstant), restLength(restLength) {}
    
    void Spring::updateForce(RigidBody* body, real32 dt)
    {
        // Calculate the two ends in world space
        glm::vec3 lws = body->transformMatrix * glm::vec4(connectionPoint, 1.0f);
        glm::vec3 ows = other->transformMatrix * glm::vec4(otherConnectionPoint, 1.0f);
        
        // Calculate vector of spring
        glm::vec3 force = lws - ows;
        
        // Calculate magnitude of force
        real32 magnitude = glm::length(force);
        magnitude = fabsf(magnitude - restLength);
        magnitude *= springConstant;
        
        // Calculate the final force
        force = glm::normalize(force);
        force *= -magnitude;
        body->addForceAtPoint(force, lws);
    }
    
}