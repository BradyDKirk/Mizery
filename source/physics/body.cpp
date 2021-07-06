/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/

#include "body.h"

namespace Mizery {
    
    static inline void _calculateTransformMatrix(glm::mat4& transformMatrix, const glm::vec3& position, const glm::quat& orientation);
    static inline void _transformInertiaTensor(glm::mat3& iitWorld, const glm::quat& q, const glm::mat3& iitBody, glm::mat4& rotmat);
    
    void RigidBody::calculateDerivedData()
    {
        orientation = glm::normalize(orientation);
        
        // Calculate the transform matrix for the body
        _calculateTransformMatrix(transformMatrix, position, orientation);
        
        // Calculate the inertia tensor in world space
        _transformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);
    }
    
    void RigidBody::integrate(real32 dt)
    {
        // Calculate linear acceleratoion from force
        lastFrameAcceleration = acceleration;
        lastFrameAcceleration = lastFrameAcceleration + (inverseMass * forceAccum);
        
        // Calculate angular acceleration from torque
        glm::vec3 angularAcceleration = inverseInertiaTensorWorld * torqueAccum;
        
        // Adjust velocities
        velocity = velocity + (lastFrameAcceleration * dt);
        velocity = velocity * powf(linearDamping, dt);
        
        rotation = rotation + (angularAcceleration * dt);
        rotation = rotation * powf(angularDamping, dt);
        
        // Update position and orientation
        position = position + (velocity * dt);
        
        // @TODO: Better way to do this probably?
        glm::quat q(0.0f, rotation.x * dt, rotation.y * dt, rotation.z * dt);
        q *= orientation;
        orientation.w += q.w * 0.5f;
        orientation.x += q.x * 0.5f;
        orientation.y += q.y * 0.5f;
        orientation.z += q.z * 0.5f;
        
        // Update dervied data
        calculateDerivedData();
        
        // Clear accumulators
        clearAccumulators();
    }
    
    void RigidBody::addForce(const glm::vec3& force)
    {
        forceAccum += force;
    }
    
    void RigidBody::addForceAtPoint(const glm::vec3& force, const glm::vec3& point)
    {
        glm::vec3 pt = point - position;
        
        forceAccum += force;
        torqueAccum += glm::cross(pt, force);
    }
    
    void RigidBody::addForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point)
    {
        glm::vec3 pt = transformMatrix * glm::vec4(point, 1.0f);
        addForceAtPoint(force, pt);
    }
    
    void RigidBody::clearAccumulators()
    {
        forceAccum = glm::vec3(0.0f);
        torqueAccum = glm::vec3(0.0f);
    }
    
    bool32 RigidBody::hasFiniteMass() const
    {
        return inverseMass >= 0.0f;
    }
    
    void RigidBody::setMass(const real32 mass)
    {
        assert(mass != 0);
        inverseMass = 1.0f / mass;
    }
    
    real32 RigidBody::getMass() const
    {
        if (inverseMass == 0)
        {
            return FLT_MAX;
        }
        else
        {
            return 1.0f / inverseMass;
        }
    }
    
    void RigidBody::setInertiaTensor(const glm::mat3& inertiaTensor)
    {
        inverseInertiaTensor = glm::inverse(inertiaTensor);
    }
    
    glm::mat3 RigidBody::getInertiaTensor() const
    {
        return glm::inverse(inverseInertiaTensor);
    }
    
    static inline void _calculateTransformMatrix(glm::mat4& transformMatrix, const glm::vec3& position, const glm::quat& orientation)
    {
        // Create transform matrix from position and orientation
        transformMatrix = glm::mat4(1.0f);
        transformMatrix = glm::translate(transformMatrix, position);
        transformMatrix = transformMatrix * glm::mat4_cast(orientation);
    }
    
    static inline void _transformInertiaTensor(glm::mat3& iitWorld, const glm::quat& q, const glm::mat3& iitBody, glm::mat4& rotmat)
    {
        // Change of basis transformation
        glm::mat3 rotmat_mat3 = glm::mat3(rotmat);
        iitWorld = rotmat_mat3 * iitBody * glm::inverse(rotmat_mat3);
    }
    
}