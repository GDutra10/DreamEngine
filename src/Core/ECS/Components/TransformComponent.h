#ifndef CORE_ECS_COMPONENTS_TRANSFORM_COMPONENT_H_
#define CORE_ECS_COMPONENTS_TRANSFORM_COMPONENT_H_

#include "Component.h"
#include "../../Vendors/glm/glm/glm.hpp"
#include "glm/detail/type_quat.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"

namespace DreamEngine::Core::ECS::Components
{
class TransformComponent : public Component
{
   public:
    glm::mat4 transform = glm::mat4(1.0f);

    explicit TransformComponent() = default;
    TransformComponent(const glm::vec3& position);
    TransformComponent(const glm::vec3& position,
                        const glm::vec3& scale,
                        const glm::vec3& rotation);
    glm::vec3 GetPosition();
    glm::vec3 GetScale();
    glm::vec3 GetRotation();
    void SetTransform(const glm::mat4& transform);
    void SetPosition(const glm::vec3& position);
    void SetScale(const glm::vec3& scale);
    void SetRotation(const glm::vec3& rotation);
    void SetRotation(const glm::quat& rotation);

   private:
    void KeepOtherDataAndApplyRotation(const glm::mat4& rotMatrix);

    
};
}  // namespace DreamEngine::Core::ECS::Components
#endif