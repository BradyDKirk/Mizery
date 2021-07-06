/***********************************************************
 * From "Game Physics Engine Development" by Ian Millington
 ***********************************************************/
#pragma once

#include "physics/body.h"

namespace Mizery {
    
    struct BoundingSphere
    {
        glm::vec3 center;
        real32 radius;
        
        // Creates a new bounding sphere
        BoundingSphere(const glm::vec3& center, real32 radius);
        
        // Creates a bounding sphere to enclose the two given bounding spheres
        BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);
        
        // Checks if the bounding sphere overlaps with the other given bounding sphere
        bool32 overlaps(const BoundingSphere* other) const;
        
        // Reports how much this bounding sphere would have to grow
        real32 getGrowth(const BoundingSphere& other) const;
        
        real32 getSize() const
        {
            return (4.0f / 3.0f) * PI * radius * radius * radius;
        }
    };
    
    // Stores a potential contact to check later
    struct PotentialContact
    {
        RigidBody* body[2];
    };
    
    // A base class for nodes in a bounding volume hierarchy
    // This class uses a binary tree to store the bounding volumes
    template<class BoundingVolumeClass>
    class BVHNode
    {
    public:
        // Holds the node immediately above in the tree
        BVHNode* parent;
        
        // Holds child nodes
        BVHNode* children[2];
        
        // Holds a single bounding volume encompassing all descendents of this node
        BoundingVolumeClass volume;
        
        // Holds the rigid body at this node (only leaf nodes have a rigid body defined)
        RigidBody* body;
        
        BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, RigidBody* body = nullptr);
        
        // Delete this node, removing it from the hierarchy
        ~BVHNode();
        
        // Checks if this node is at the bottom of the hierarchy
        bool32 isLeaf() const
        {
            return body != nullptr;
        }
        
        // Insert rigid body with the given bounding volume into the hierarchy
        void insert(RigidBody* newBody, const BoundingVolumeClass& newVolume);
        
        uint32 getPotentialContacts(PotentialContact* contacts, uint32 limit) const;
        
    private:
        // Checks for overlapping between nodes
        bool32 overlaps(const BVHNode<BoundingVolumeClass>* other) const;
        
        uint32 getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, uint32 limit) const;
    
        void recalculateBoundingVolume(bool32 recurse = true);
    
    };
    
}