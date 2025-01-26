#include "TransformComponent.h"
#include <glm/gtx/euler_angles.hpp>
#include "glm/gtx/quaternion.hpp"


using namespace DreamEngine::Core::ECS::Components;

TransformComponent::TransformComponent(const glm::vec3& position)
{
    transform = glm::mat4(1.0f);
}

TransformComponent::TransformComponent(
    const glm::vec3& position,
    const glm::vec3& scale,
    const glm::vec3& rotation)
{
    transform = glm::mat4(1.0f);
}

glm::vec3 TransformComponent::GetPosition()
{
    return { transform[3][0], transform[3][1], transform[3][2] };
}

glm::vec3 TransformComponent::GetScale()
{
    float scaleX = glm::length(glm::vec3(transform[0])); // Length of X-axis vector
    float scaleY = glm::length(glm::vec3(transform[1])); // Length of Y-axis vector
    float scaleZ = glm::length(glm::vec3(transform[2])); // Length of Z-axis vector

    return { scaleX, scaleY, scaleZ };
}

glm::vec3 TransformComponent::GetRotation()
{
    glm::vec3 eulerAngles;
    glm::extractEulerAngleYXZ(transform, eulerAngles.y, eulerAngles.x, eulerAngles.z);
    return glm::degrees(eulerAngles); // Convert to degrees for easier editing
}

void TransformComponent::SetTransform(const glm::mat4& transform)
{
    this->transform = transform;
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
    transform[3] = glm::vec4(position, 1.0f);
}

void TransformComponent::SetScale(const glm::vec3& scale)
{
    transform[0][0] = scale.x;
    transform[1][1] = scale.y;
    transform[2][2] = scale.z;
}

void TransformComponent::SetRotation(const glm::vec3& rotation)
{
    const glm::vec3 radians = glm::radians(rotation);  // Convert degrees to radians
    const glm::mat4 rotMatrix = glm::eulerAngleYXZ(radians.y, radians.x, radians.z);

    KeepOtherDataAndApplyRotation(rotMatrix);
}

void TransformComponent::SetRotation(const glm::quat& rotation)
{
    // Create a rotation matrix from the quaternion
    const glm::mat4 rotMatrix = glm::toMat4(rotation);

    KeepOtherDataAndApplyRotation(rotMatrix);
}

void TransformComponent::KeepOtherDataAndApplyRotation(const glm::mat4& rotMatrix)
{
    // Keep the existing scale and translation
    const glm::vec3 scale(glm::length(glm::vec3(transform[0])),
                          glm::length(glm::vec3(transform[1])),
                          glm::length(glm::vec3(transform[2])));
    const glm::vec3 position(transform[3]);

    // Recompose the matrix with the new rotation
    transform = glm::mat4(1.0f);  // Reset to identity matrix
    transform = glm::translate(transform, position);  // Apply translation
    transform *= rotMatrix;                           // Apply the new rotation
    transform[0] *= scale.x;                          // Apply scale
    transform[1] *= scale.y;                          // Apply scale
    transform[2] *= scale.z;                          // Apply scale
}

