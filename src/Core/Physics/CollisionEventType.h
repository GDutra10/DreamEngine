#ifndef CORE_PHYSICS_COLLISION_EVENT_TYPE_H
#define CORE_PHYSICS_COLLISION_EVENT_TYPE_H

#include "CoreExport.h"

namespace DreamEngine::Core::Physics
{
enum class CORE_API CollisionEventType : unsigned int
{
    Enter = 0,
    Stay = 1,
    Exit = 2
};
}  // namespace DreamEngine::Core::Physics
#endif