#ifndef CORE_ECS_COMPONENTS_OUTLINE_COMPONENT_H_
#define CORE_ECS_COMPONENTS_OUTLINE_COMPONENT_H_
#include "Component.h"
#include "../Core/Color.h"

namespace DreamEngine::Core::ECS::Components
{

class CORE_API OutlineComponent : public Component
{
   public:
    Color color = Color(1.0f, 1.0f, 0.0f, 1.0f);
    
    // Fractional inflation: final scale = 1.0 + thickness (e.g. 0.03 = +3%)
    float thickness = 0.06f;

    Shader* shader = nullptr;

    explicit OutlineComponent() = default;   
};
}  // namespace DreamEngine::Core::ECS::Components
#endif