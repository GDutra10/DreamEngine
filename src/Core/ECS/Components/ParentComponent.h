#ifndef CORE_ECS_COMPONENTS_PARENT_COMPONENT_H_
#define CORE_ECS_COMPONENTS_PARENT_COMPONENT_H_

#include "Component.h"
#include <ECS/Entity.h>

namespace DreamEngine::Core::ECS::Components
{

class ParentComponent : public Component
{
   public:
    Entity* parent;
    explicit ParentComponent() = default;
    ParentComponent(Entity* parent) : parent(parent) {}
};
}  // namespace DreamEngine::Core::ECS::Components
#endif