#ifndef CORE_ECS_COMPONENTS_CAMERA_COMPONENT_H_
#define CORE_ECS_COMPONENTS_CAMERA_COMPONENT_H_

#include <ECS/Entity.h>
#include "Component.h"
#include "../../Vendors/glm/glm/glm.hpp"

namespace DreamEngine::Core::ECS::Components
{
    
class CORE_API CameraComponent : public Component
{
   public:
    glm::vec3 position = {0, 0, 0};
    glm::vec3 front = {0, 0, -1};
    glm::vec3 up = {0, 1, 0};
    glm::vec3 right = {0, 0, 0};
    glm::vec3 worldUp = {0, 1, 0};

    float fovDegree = 45.0f;
    float near = 0.1f;
    float far = 100.0f;

    explicit CameraComponent() = default;    
};
}  // namespace DreamEngine::Core::ECS::Components
#endif