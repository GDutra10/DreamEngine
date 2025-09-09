#ifndef CORE_ECS_COMPONENTS_MATERIAL_COMPONENT_H_
#define CORE_ECS_COMPONENTS_MATERIAL_COMPONENT_H_

#include "../../Core/Render/Material.h"
#include "Component.h"

namespace DreamEngine::Core::ECS::Components
{
using namespace DreamEngine::Core::Render;

class CORE_API MaterialComponent : public Component
{
   public:
    Material* material;

    explicit MaterialComponent() = default;
    MaterialComponent(Material* material) : material(material) {}
};
}  // namespace DreamEngine::Core::ECS::Components
#endif