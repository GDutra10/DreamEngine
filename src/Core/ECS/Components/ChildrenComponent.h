#ifndef CORE_ECS_COMPONENTS_CHILDREN_COMPONENT_H_
#define CORE_ECS_COMPONENTS_CHILDREN_COMPONENT_H_

#include "Component.h"

namespace DreamEngine::Core::ECS::Components
{
class CORE_API ChildrenComponent : public Component
{
   public:
    std::vector<Entity*> children;
    explicit ChildrenComponent() = default;
    ChildrenComponent(const std::vector<Entity*>& children) : children(children) {}
};
}  // namespace DreamEngine::Core::ECS::Components
#endif