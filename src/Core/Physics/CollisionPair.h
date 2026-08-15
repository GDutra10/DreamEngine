#ifndef CORE_PHYSICS_COLLISION_PAIR_H
#define CORE_PHYSICS_COLLISION_PAIR_H

#include "CoreExport.h"

namespace DreamEngine::Core::Physics
{
struct CORE_API CollisionPair
{
    unsigned int entityA;
    unsigned int entityB;
    bool isTrigger;

    bool operator==(const CollisionPair& other) const noexcept 
    { 
        return entityA == other.entityA && 
            entityB == other.entityB; 
    }

    static CollisionPair MakePair(unsigned int a, unsigned int b) noexcept
    {
        if (a < b)
            return {a, b};

        return {b, a};
    }
};
}  // namespace DreamEngine::Core::Physics
#endif