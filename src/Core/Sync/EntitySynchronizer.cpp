#include "EntitySynchronizer.h"
#include "../ECS/Components/TransformComponent.h"
#include "ECS/Components/UiComponent.h"
#include "glm/gtx/quaternion.hpp"

using namespace DreamEngine::Core::Sync;
using namespace DreamEngine::Core::ECS::Components;

void EntitySynchronizer::SynchronizeToData(Entity* entity)
{
    TransformComponent& transformComponent = entity->GetComponent<TransformComponent>();
    const glm::vec3 position = transformComponent.GetPosition();
    const glm::vec3 rotation = transformComponent.GetRotation();
    const glm::vec3 scale = transformComponent.GetScale();

    entity->entityData.id = entity->GetId();
    entity->entityData.transformPositionX = position.x;
    entity->entityData.transformPositionY = position.y;
    entity->entityData.transformPositionZ = position.z;
    entity->entityData.transformRotationX = rotation.x;
    entity->entityData.transformRotationY = rotation.y;
    entity->entityData.transformRotationZ = rotation.z;
    entity->entityData.transformScaleX = scale.x;
    entity->entityData.transformScaleY = scale.y;
    entity->entityData.transformScaleZ = scale.z;

    UiComponent& uiComponent = entity->GetComponent<UiComponent>();
    entity->entityData.uiComponentHas = uiComponent.has;
}

void EntitySynchronizer::SynchronizeFromData(Entity* entity)
{
    EntityData& entityData = entity->entityData;
    TransformComponent& transformComponent = entity->GetComponent<TransformComponent>();

    // Extract individual components from EntityData
    glm::vec3 scale(entityData.transformScaleX, entityData.transformScaleY, entityData.transformScaleZ);
    glm::vec3 rotationEuler(glm::radians(entityData.transformRotationX), glm::radians(entityData.transformRotationY), glm::radians(entityData.transformRotationZ));
    glm::vec3 position(entityData.transformPositionX, entityData.transformPositionY, entityData.transformPositionZ);

    // Update the TransformComponent's individual properties
    transformComponent.SetScale(scale);
    transformComponent.SetRotation(rotationEuler);
    transformComponent.SetPosition(position);

    // Construct the transformation matrix: Scale -> Rotate -> Translate
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rotationEuler));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

    glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    // Update the TransformComponent's transform matrix
    transformComponent.SetTransform(transformMatrix);
}