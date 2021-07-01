/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#pragma once

#include "core/math.h"

namespace Mizery {
    
    class RigidBody
    {
    public:
        // Inverse mass of rigid body
        real32 inverseMass;
        
        // Inverse of the body's inertia tensor
        glm::mat3 inverseInertiaTensor;
        
        // Position in world space
        glm::vec3 position;
        
        // Angular orientation
        glm::quat orientation;
        
        // Linear velocity
        glm::vec3 velocity;
        
        // Angular velocity
        glm::vec3 rotation;
        
        glm::vec3 acceleration;
        
        glm::vec3 lastFrameAcceleration;
        
        // Holds amount of damping applied to linear motion
        real32 linearDamping;
        
        // Holds amount of damping applied to angular motion
        real32 angularDamping;
        
        // Inverse inertia tensor of the body in world space
        glm::mat3 inverseInertiaTensorWorld;
        
        // Transform matrix for convert from model space to world space
        glm::mat4 transformMatrix;
        
        // Calculates internal data from state data
        void calculateDerivedData();
        
        void integrate(real32 dt);
        
        // Add force to rigid body
        void addForce(const glm::vec3& force);
        
        // Adds a force to a given point on the rigid body (world space)
        void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);
        
        // Adds a force to a given point on the rigid body (model space)
        void addForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point);
        
        // Clear force and torque accumulator
        void clearAccumulators();
        
        // Return whether the body has a finite mass or not
        bool32 hasFiniteMass() const;
        
        // Setters and getters
        void setMass(const real32 mass);
        real32 getMass() const;
        
        void setInertiaTensor(const glm::mat3& inertiaTensor);
        glm::mat3 getInertiaTensor() const;
        
    private:
        glm::vec3 forceAccum = glm::vec3(0.0f);
        glm::vec3 torqueAccum = glm::vec3(0.0f);
    };
    
}