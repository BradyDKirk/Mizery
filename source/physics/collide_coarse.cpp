#include "collide_coarse.h"

namespace Mizery {
    
    // Bounding Sphere code
    BoundingSphere::BoundingSphere(const glm::vec3& center, real32 radius)
    {
        BoundingSphere::center = center;
        BoundingSphere::radius = radius;
    }
    
    BoundingSphere::BoundingSphere(const BoundingSphere& one, const BoundingSphere& two)
    {
        glm::vec3 centerOffset = two.center - one.center;
        
        real32 distance = glm::length(centerOffset);
        distance *= distance;
        
        real32 radiusDiff = two.radius - one.radius;
        
        // Check if the larger sphere encloses the small one
        if (radiusDiff * radiusDiff >= distance)
        {
            if (one.radius > two.radius)
            {
                center = one.center;
                radius = one.radius;
            }
            else
            {
                center = two.center;
                radius = two.radius;
            }
        }
        else
        {
            distance = sqrtf(distance);
            radius = (distance + one.radius + two.radius) * 0.5f;
            
            // The new center is based on one's center, moved towards two's center
            // by anamount proportional to the sphere's radii
            center = one.center;
            if (distance > 0.0f)
            {
                center += centerOffset * ((radius - one.radius) / distance);
            }
        }
    }
    
    bool32 BoundingSphere::overlaps(const BoundingSphere* other) const
    {
        real32 distanceSquared = glm::length(center - other->center);
        distanceSquared *= distanceSquared;
        return distanceSquared < ((radius + other->radius) * (radius + other->radius));
    }
    
    real32 BoundingSphere::getGrowth(const BoundingSphere& other) const
    {
        BoundingSphere newSphere(*this, other);
        return (newSphere.radius * newSphere.radius) - (radius * radius);
    }
    
    // BVH Code
    template<class BoundingVolumeClass>
    BVHNode<BoundingVolumeClass>::BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, RigidBody* body) : parent(parent), volume(volume), body(body)
    {
        children[0] = children[1] = nullptr;
    }
    
    template<class BoundingVolumeClass>
    BVHNode<BoundingVolumeClass>::~BVHNode()
    {
        // If we don't have a parent, then we ignore the sibling processing
        if (parent)
        {
            // Find our sibling
            BVHNode<BoundingVolumeClass>* sibling;
            if (parent->children[0] == this)
            {
                sibling = parent->children[1];
            }
            else
            {
                sibling = parent->children[0];
            }
            
            // Write its data to our parent
            parent->volume = sibling->volume;
            parent->body = sibling->body;
            parent->children[0] = sibling->children[0];
            parent->children[1] = sibling->children[1];
            
            // Delete the sibling
            sibling->parent = nullptr;
            sibling->body = nullptr;
            sibling->children[0] = nullptr;
            sibling->children[1] = nullptr;
            delete sibling;
            
            // Recalculate parent's bounding volume
            parent->recalculateBoundingVolume();
        }
        
        // Delete our children
        if (children[0])
        {
            children[0]->parent = nullptr;
            delete children[0];
        }
        if (children[1])
        {
            children[1]->parent = nullptr;
            delete children[1];
        }
    }
    
    template<class BoundingVolumeClass>
    bool32 BVHNode<BoundingVolumeClass>::overlaps(const BVHNode<BoundingVolumeClass>* other) const
    {
        return volume->overlaps(other->volume);
    }
    
    template<class BoundingVolumeClass>
    void BVHNode<BoundingVolumeClass>::insert(RigidBody* newBody, const BoundingVolumeClass& newVolume)
    {
        // If we're a leaf then the only option is to spawn two new children and place the new body in one
        if (isLeaf())
        {
            // Child one is a copy of us
            children[0] = new BVHNode<BoundingVolumeClass>(this, volume, body);
            
            // Child two holds the new body
            children[1] = new BVHNode<BoundingVolumeClass>(this, newVolume, newBody);
            
            // And we now loosen the body
            this->body = nullptr;
            
            // We need to recalculate our bounding volume
            recalculateBoundingVolume();
        }
        else
        {
            if (children[0]->volume.getGrowth(newVolume) < children[1]->volume.getGrowth(newVolume))
            {
                children[0]->insert(newBody, newVolume);
            }
            else
            {
                children[1]->insert(newBody, newVolume);
            }
        }
    }
    
    template<class BoundingVolumeClass>
    uint32 BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact* contacts, uint32 limit) const
    {
        // Early out if we don't have the room for contacts of if leaf node
        if (isLeaf() || limit == 0) { return 0; }
        
        return children[0]->getPotentialContactsWith(children[1], contacts, limit);
    }
    
    template<class BoundingVolumeClass>
    uint32 BVHNode<BoundingVolumeClass>::getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, uint32 limit) const
    {
        // Early out if we don't overlap or if we have no room to report contacts
        if (!overlaps(other) || limit == 0) { return 0; }
        
        // If we're both a leaf nodes then we have a potential contact
        if (isLeaf() && other->isLeaf())
        {
            contacts->body[0] = body;
            contacts->body[1] = other->body;
            return 1;
        }
        
        // Determine which node to descend into. If either is a leaf then we descend the other.
        // If both are branches then we use the one with the largest size
        if (other->isLeaf() || (!isLeaf() && volume->getSize() >= other->volume->getSize()))
        {
            // Recurse into ourself
            uint32 count = children[0]->getPotentialContactsWith(other, contacts, limit);
            
            // Check whether we have enough slots to do the other side too
            if (limit > count)
            {
                return count + children[1]->getPotentialContactsWith(other, contacts + count, limit - count);
            }
            else
            {
                return count;
            }
        }
        else
        {
            // Recurse into the other node
            uint32 count = getPotentialContactsWith(other->children[0], contacts, limit);
            
            // Check whether we have enough slots to do the other side too
            if (limit > count)
            {
                return count + getPotentialContactsWith(other->children[1], contacts + count, limit - count);
            }
            else
            {
                return count;
            }
        }
    }
    
    template<class BoundingVolumeClass>
    void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume(bool32 recurse)
    {
        if (isLeaf()) { return; }
        
        // Use the bounding volume combining constructor
        volume = BoundingVolumeClass(children[0]->volume, children[1]->volume);
        
        // Recurse up the tree
        if (parent) { parent->recalculateBoundingVolume(true); }
    }
    
}