#ifndef CORE_ECS_COMPONENTS_COLLIDER_COMPONENT_H_
#define CORE_ECS_COMPONENTS_COLLIDER_COMPONENT_H_

#include "Component.h"
#include "../../Math/AABB.h"

namespace DreamEngine::Core::ECS::Components
{
    using namespace DreamEngine::Core::Math;
class CORE_API ColliderComponent : public Component
{
   public:
    bool enabled = true;
    bool isTrigger = false;

    glm::vec3 center{0.0f};
    glm::vec3 size{1.0f};

    AABB localBounds;
    AABB worldBounds;
    explicit ColliderComponent() = default;
    void Reset() override
    {
        enabled = true;
        isTrigger = false;
        center = {0.0f, 0.0f, 0.0f};
        size = {1.0f, 1.0f, 1.0f};
    }
};
}  // namespace DreamEngine::Core::ECS::Components
#endif