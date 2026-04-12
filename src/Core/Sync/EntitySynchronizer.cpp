#include "EntitySynchronizer.h"
#include "../ECS/Components/TransformComponent.h"
#include "ECS/Components/ScriptComponent.h"
#include "ECS/Components/UiComponent.h"
#include "ECS/Components/DirectionalLightComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/ChildrenComponent.h"
#include "Application.h"
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

    // parent component
    ParentComponent& parentComponent = entity->GetComponent<ParentComponent>();

    if (parentComponent.has && parentComponent.parent != nullptr)
        entity->entityData.parentId = parentComponent.parent->GetId();

    // ui component
    UiComponent& uiComponent = entity->GetComponent<UiComponent>();
    entity->entityData.uiComponentHas = uiComponent.has;

    // script info
    ScriptComponent& scriptComponent = entity->GetComponent<ScriptComponent>();
    if (scriptComponent.has && scriptComponent.script != nullptr)
    {
        std::string assembly = scriptComponent.script->GetAssemblyName();
        std::strncpy(entity->entityData.scriptAssemblyName, assembly.c_str(), sizeof(entity->entityData.scriptAssemblyName) - 1);
        entity->entityData.scriptAssemblyName[sizeof(entity->entityData.scriptAssemblyName) - 1] = '\0';

        std::string className = scriptComponent.script->GetClassName();
        std::strncpy(entity->entityData.scriptClassName, className.c_str(), sizeof(entity->entityData.scriptClassName) - 1);
        entity->entityData.scriptClassName[sizeof(entity->entityData.scriptClassName) - 1] = '\0';
    }

    // directional light component
    DirectionalLightComponent& directionalLightComponent = entity->GetComponent<DirectionalLightComponent>();
    entity->entityData.directionalLightComponentHas = directionalLightComponent.has;

    if (directionalLightComponent.has)
    {
        entity->entityData.directionalLightSpecularX = directionalLightComponent.specular.x;
        entity->entityData.directionalLightSpecularY = directionalLightComponent.specular.y;
        entity->entityData.directionalLightSpecularZ = directionalLightComponent.specular.z;

        entity->entityData.directionalLightInfluence = directionalLightComponent.influence;

        entity->entityData.directionalLightColorRed = directionalLightComponent.color.red;
        entity->entityData.directionalLightColorGreen = directionalLightComponent.color.green;
        entity->entityData.directionalLightColorBlue = directionalLightComponent.color.blue;
        entity->entityData.directionalLightColorAlpha = directionalLightComponent.color.alpha;
    }

    // camera component
    CameraComponent& cameraComponent = entity->GetComponent<CameraComponent>();
    entity->entityData.cameraComponentHas = cameraComponent.has;

    if (entity->entityData.cameraComponentHas)
    {
        entity->entityData.cameraFar = cameraComponent.far;
        entity->entityData.cameraFov = cameraComponent.fovDegree;
        entity->entityData.cameraNear = cameraComponent.near;
    }
}

void EntitySynchronizer::SynchronizeFromData(Entity* entity)
{
    TransformComponent& transformComponent = entity->GetComponent<TransformComponent>();

    // parent id component
    HandleParentComponent(entity);

    // Extract individual components from EntityData
    glm::vec3 scale(entity->entityData.transformScaleX, entity->entityData.transformScaleY, entity->entityData.transformScaleZ);
    glm::vec3 rotationEuler(glm::radians(entity->entityData.transformRotationX), 
                            glm::radians(entity->entityData.transformRotationY),
                            glm::radians(entity->entityData.transformRotationZ));
    glm::vec3 position(entity->entityData.transformPositionX, entity->entityData.transformPositionY, entity->entityData.transformPositionZ);

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

    // update ui component
    UiComponent& uiComponent = entity->GetComponent<UiComponent>();
    uiComponent.has = entity->entityData.uiComponentHas;

    // update directional light component
    DirectionalLightComponent& directionalLightComponent = entity->GetComponent<DirectionalLightComponent>();
    directionalLightComponent.has = entity->entityData.directionalLightComponentHas;
    directionalLightComponent.color.red = entity->entityData.directionalLightColorRed;
    directionalLightComponent.color.green = entity->entityData.directionalLightColorGreen;
    directionalLightComponent.color.blue = entity->entityData.directionalLightColorBlue;
    directionalLightComponent.color.alpha = entity->entityData.directionalLightColorAlpha;

    directionalLightComponent.specular.x = entity->entityData.directionalLightSpecularX;
    directionalLightComponent.specular.y = entity->entityData.directionalLightSpecularY;
    directionalLightComponent.specular.z = entity->entityData.directionalLightSpecularZ;

    directionalLightComponent.influence = entity->entityData.directionalLightInfluence;

    // update camera component
    CameraComponent& cameraComponent = entity->GetComponent<CameraComponent>();
    cameraComponent.has = entity->entityData.cameraComponentHas;
    cameraComponent.far = entity->entityData.cameraFar;
    cameraComponent.fovDegree = entity->entityData.cameraFov;
    cameraComponent.near = entity->entityData.cameraNear;
}

void DreamEngine::Core::Sync::EntitySynchronizer::HandleParentComponent(Entity* entity)
{
    ParentComponent& parentComponent = entity->GetComponent<ParentComponent>();
    parentComponent.has = entity->entityData.parentId > 0;

    if (parentComponent.parent != nullptr && (parentComponent.parent->GetId() != entity->entityData.parentId || entity->entityData.parentId <= 0) ||
        parentComponent.parent == nullptr && entity->entityData.parentId > 0)
    {
        RemoveEntityFromParent(entity, parentComponent);
        AddEntityToParent(entity, parentComponent);
    }
}

void DreamEngine::Core::Sync::EntitySynchronizer::RemoveEntityFromParent(Entity* entity, ParentComponent& parentComponent)
{
    if (parentComponent.parent == nullptr)
        return;

    unsigned int entityId = entity->GetId();
    ChildrenComponent& oldParentChildrenComp = parentComponent.parent->GetComponent<ChildrenComponent>();

    std::vector<Entity*>::iterator childrenFound =
        std::find_if(oldParentChildrenComp.children.begin(), oldParentChildrenComp.children.end(), [entityId](Entity* ent) { return ent->GetId() == entityId; });

    if (childrenFound != oldParentChildrenComp.children.end())
    {
        oldParentChildrenComp.children.erase(childrenFound);
    }
}

void DreamEngine::Core::Sync::EntitySynchronizer::AddEntityToParent(Entity* entity, ParentComponent& parentComponent)
{
    if (entity->entityData.parentId <= 0)
    {
        parentComponent.parent = nullptr;

        return;
    }

    unsigned int parentId = entity->entityData.parentId;
    std::vector<Entity*> entities = Application::Instance().GetGame()->GetActiveScene()->GetEntityManager()->GetEntities();
    std::vector<Entity*>::iterator it = std::find_if(entities.begin(), entities.end(), [parentId](Entity* pEntity) { return pEntity->GetId() == parentId; });

    if (it != entities.end())
    {
        parentComponent.parent = (*it);
        ChildrenComponent& parentChildrenComponent = parentComponent.parent->GetComponent<ChildrenComponent>();
        parentChildrenComponent.children.push_back(entity);
    }
}
