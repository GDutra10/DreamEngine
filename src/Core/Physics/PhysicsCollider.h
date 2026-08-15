#ifndef CORE_PHYSICS_COLLIDER_H
#define CORE_PHYSICS_COLLIDER_H

#include "CoreExport.h"

#include "../ECS/Entity.h"
#include "../ECS/Components/ColliderComponent.h"

namespace DreamEngine::Core::Physics
{
    using namespace DreamEngine::Core::ECS;
    using namespace DreamEngine::Core::ECS::Components;

    struct CORE_API PhysicsCollider
    {
        Entity* entity;
        ColliderComponent* collider;
    };
}  // namespace DreamEngine::Core::Physics
#endif