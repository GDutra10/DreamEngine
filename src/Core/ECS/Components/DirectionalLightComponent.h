#ifndef CORE_ECS_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H_
#define CORE_ECS_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H_

#include "../../Core/Render/Mesh.h"
#include "Component.h"
#include "../../Color.h"

namespace DreamEngine::Core::ECS::Components
{
using namespace DreamEngine::Core::Render;

class CORE_API DirectionalLightComponent : public Component
{
   public:
    glm::vec3 specular = {0, 0, 0};
    Color color;
    float influence;

    explicit DirectionalLightComponent() = default;
};
}  // namespace DreamEngine::Core::ECS::Components
#endif