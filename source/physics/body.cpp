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
        transformMatrix = glm::mat4(1.0f);
        transformMatrix = glm::translate(transformMatrix, position);
        transformMatrix = transformMatrix * glm::mat4_cast(orientation);
    }
    
    static inline void _transformInertiaTensor(glm::mat3& iitWorld, const glm::quat& q, const glm::mat3& iitBody, glm::mat4& rotmat)
    {
        // @TODO: Verify this is correct
        real32 t4  = (rotmat[0][0] * iitBody[0][0]) + (rotmat[1][0] * iitBody[0][1]) + (rotmat[2][0] * iitBody[0][2]);
        real32 t9  = (rotmat[0][0] * iitBody[1][0]) + (rotmat[1][0] * iitBody[1][1]) + (rotmat[2][0] * iitBody[1][2]);
        real32 t14 = (rotmat[0][0] * iitBody[2][0]) + (rotmat[1][0] * iitBody[2][1]) + (rotmat[2][0] * iitBody[2][2]);
        real32 t28 = (rotmat[0][1] * iitBody[0][0]) + (rotmat[1][1] * iitBody[0][1]) + (rotmat[2][1] * iitBody[0][2]);
        real32 t33 = (rotmat[0][1] * iitBody[1][0]) + (rotmat[1][1] * iitBody[1][1]) + (rotmat[2][1] * iitBody[1][2]);
        real32 t38 = (rotmat[0][1] * iitBody[2][0]) + (rotmat[1][1] * iitBody[2][1]) + (rotmat[2][1] * iitBody[2][2]);
        real32 t52 = (rotmat[0][2] * iitBody[0][0]) + (rotmat[1][2] * iitBody[0][1]) + (rotmat[2][2] * iitBody[0][2]);
        real32 t57 = (rotmat[0][2] * iitBody[1][0]) + (rotmat[1][2] * iitBody[1][1]) + (rotmat[2][2] * iitBody[1][2]);
        real32 t62 = (rotmat[0][2] * iitBody[2][0]) + (rotmat[1][2] * iitBody[2][1]) + (rotmat[2][2] * iitBody[2][2]);
        
        iitWorld[0][0] = (t4 *  rotmat[0][0]) + (t9 *  rotmat[1][0]) + (t14 *  rotmat[2][0]);
        iitWorld[1][0] = (t4 *  rotmat[0][1]) + (t9 *  rotmat[1][1]) + (t14 *  rotmat[2][1]);
        iitWorld[2][0] = (t4 *  rotmat[0][2]) + (t9 *  rotmat[1][2]) + (t14 *  rotmat[2][2]);
        
        iitWorld[0][1] = (t28 * rotmat[0][0]) + (t33 * rotmat[1][0]) + (t38 * rotmat[2][0]);
        iitWorld[1][1] = (t28 * rotmat[0][1]) + (t33 * rotmat[1][1]) + (t38 * rotmat[2][1]);
        iitWorld[2][1] = (t28 * rotmat[0][2]) + (t33 * rotmat[1][2]) + (t38 * rotmat[2][2]);
        
        iitWorld[0][2] = (t52 * rotmat[0][0]) + (t57 * rotmat[1][0]) + (t62 * rotmat[2][0]);
        iitWorld[1][2] = (t52 * rotmat[0][1]) + (t57 * rotmat[1][1]) + (t62 * rotmat[2][1]);
        iitWorld[2][2] = (t52 * rotmat[0][2]) + (t57 * rotmat[1][2]) + (t62 * rotmat[2][2]);
    }
    
}