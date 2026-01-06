#ifndef CORE_ECS_COMPONENTS_CAMERA_COMPONENT_H_
#define CORE_ECS_COMPONENTS_CAMERA_COMPONENT_H_

#include "Component.h"

namespace DreamEngine::Core::ECS::Components
{
    
class CORE_API CameraComponent : public Component
{
   public:
    float fovDegree = 45.0f;
    float near = 0.1f;
    float far = 100.0f;

    explicit CameraComponent() = default;    
};
}  // namespace DreamEngine::Core::ECS::Components
#endif