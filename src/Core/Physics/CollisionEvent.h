#ifndef CORE_PHYSICS_COLLISION_EVENT_H
#define CORE_PHYSICS_COLLISION_EVENT_H

#include "CoreExport.h"
#include "CollisionEventType.h"

namespace DreamEngine::Core::Physics
{
struct CORE_API CollisionEvent
{
    unsigned int entityA;
    unsigned int entityB;
    CollisionEventType type;
    bool isTrigger;
};
}  // namespace DreamEngine::Core::Physics
#endif