#ifndef CORE_ECS_COMPONENTS_COMPONENT_H_
#define CORE_ECS_COMPONENTS_COMPONENT_H_

#include <type_traits>

namespace DreamEngine::Core::ECS::Components
{
class Component
{
   public:
    bool has = false;
};

template <typename T>
concept IsComponent = requires
{
    std::is_base_of_v<Component, T>;
};
}  // namespace DreamEngine::Core::ECS::Component
#endif