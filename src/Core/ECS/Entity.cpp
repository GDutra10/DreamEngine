#include "Entity.h"

#include "EntityMemoryPool.h"
#include "../GameSystem/Definitions/EntityDefinition.h"

#include "../Resources/ResourceManager.h"

using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::GameSystem::Definitions;
using namespace DreamEngine::Core::Resources;

void Entity::Destroy() const
{
    EntityMemoryPool::Instance().RemoveEntity(m_id);
}

bool Entity::GetIsActive() const
{
    return !m_shouldIgnore;
}

size_t Entity::GetId() const
{
    return m_id;
}

std::string& Entity::GetTag()
{
    return EntityMemoryPool::Instance().GetTag(m_id);
}

std::string& Entity::GetName()
{
    return EntityMemoryPool::Instance().GetName(m_id);
}

std::string& Entity::GetIdentifier()
{
    return m_identifier;
}

void Entity::SetIdentifier(const std::string& identifier)
{
    m_identifier = identifier;
}

void Entity::SetActive(const bool value)
{
    m_shouldIgnore = !value;
}

glm::mat4 Entity::GetWorldTransform()
{
    const TransformComponent& transformComponent = GetComponent<TransformComponent>();
    const ParentComponent& parentComponent = GetComponent<ParentComponent>();

    if (parentComponent.has && parentComponent.parent != nullptr)
        return parentComponent.parent->GetWorldTransform() * transformComponent.transform;

    return transformComponent.transform;
}

const EntityDefinition Entity::GetDefinition()
{
    EntityDefinition definition{};

    definition.name = GetName();
    definition.tag = GetTag();
    definition.identifier = GetIdentifier();
    definition.isActive = GetIsActive();

    TransformComponent& transform = GetComponent<TransformComponent>();
    const glm::vec3& position = transform.GetPosition();
    const glm::vec3& rotation = transform.GetRotation();
    const glm::vec3& scale = transform.GetScale();

    definition.transform.position = {position.x, position.y, position.z};
    definition.transform.rotation = {rotation.x, rotation.y, rotation.z};
    definition.transform.scale = {scale.x, scale.y, scale.z};

    // components
    const ChildrenComponent& children = GetComponent<ChildrenComponent>();
    for (auto& child : children.children)
        definition.components.children.childIdentifiers.push_back(child->m_identifier);

    const MaterialComponent& material = GetComponent<MaterialComponent>();
    definition.components.material.resourceId = (material.has) ? material.material->resourceId : "";

    const MeshComponent& mesh = GetComponent<MeshComponent>();
    definition.components.mesh.resourceId = (mesh.has) ? mesh.mesh->resourceId : "";

    const ParentComponent& parent = GetComponent<ParentComponent>();
    definition.components.parent.parentIdentifier = (parent.has && parent.parent != nullptr) ? parent.parent->GetIdentifier() : "";

    const ScriptComponent& script = GetComponent<ScriptComponent>();
    if (script.has)
    {
        definition.components.script.resourceId = script.script->resourceId;
        definition.components.script.assemblyName = script.script->GetAssemblyName();
        definition.components.script.className = script.script->GetClassName();
    }

    const CameraComponent& camera = GetComponent<CameraComponent>();
    if (camera.has)
    {
        definition.components.camera.far = camera.far;
        definition.components.camera.fovDegree = camera.fovDegree;
        definition.components.camera.has = camera.has;
        definition.components.camera.near = camera.near;
    }

    const UiComponent& ui = GetComponent<UiComponent>();
    if (ui.has)
    {
        definition.components.ui.has = ui.has;
        definition.components.ui.resourceId = ui.content->resourceId;
        definition.components.ui.zOrder = ui.zOrder;
    }

    const ColliderComponent& collider = GetComponent<ColliderComponent>();
    if (collider.has)
    {
        definition.components.collider.has = collider.has;
        definition.components.collider.center.x = collider.center.x;
        definition.components.collider.center.y = collider.center.y;
        definition.components.collider.center.z = collider.center.z;
        definition.components.collider.size.x = collider.size.x;
        definition.components.collider.size.y = collider.size.y;
        definition.components.collider.size.z = collider.size.z;
        definition.components.collider.enabled = collider.enabled;
        definition.components.collider.isTrigger = collider.isTrigger;
    }

    return definition;
}

void Entity::ApplyDefinition(const EntityDefinition& definition)
{
    GetName() = definition.name;
    GetTag() = definition.tag;
    SetActive(definition.isActive);

    TransformComponent& transform = GetComponent<TransformComponent>();

    transform.SetPosition({definition.transform.position.x, definition.transform.position.y, definition.transform.position.z});
    transform.SetRotation({definition.transform.rotation.x, definition.transform.rotation.y, definition.transform.rotation.z});
    transform.SetScale({definition.transform.scale.x, definition.transform.scale.y, definition.transform.scale.z});

    // components
    // the children/parent is handle when add the Entity in entityManager.Add(tag, parent) do not do manually here!

    MaterialComponent& material = GetComponent<MaterialComponent>();
    material.has = !definition.components.material.resourceId.empty();

    if (material.has)
        material.material = ResourceManager::Instance().GetMaterial(definition.components.material.resourceId);

    MeshComponent& mesh = GetComponent<MeshComponent>();

    if (!definition.components.mesh.resourceId.empty())
    {
        mesh.has = true;
        mesh.mesh = ResourceManager::Instance().GetMesh(definition.components.mesh.resourceId);
    }

    ScriptComponent& script = GetComponent<ScriptComponent>();
    script.has = !definition.components.script.resourceId.empty();

    if (script.has)
        script.script = ResourceManager::Instance().GetScript(definition.components.script.resourceId);

    CameraComponent& camera = GetComponent<CameraComponent>();
    camera.has = definition.components.camera.has;

    if (camera.has)
    {
        camera.far = definition.components.camera.far;
        camera.near = definition.components.camera.near;
        camera.fovDegree = definition.components.camera.fovDegree;
    }

    UiComponent& ui = GetComponent<UiComponent>();
    ui.has = definition.components.ui.has;

    if (ui.has)
    {
        ui.content = ResourceManager::Instance().GetUiContent(definition.components.ui.resourceId);
        ui.zOrder = definition.components.ui.zOrder;
    }

    ColliderComponent& collider = GetComponent<ColliderComponent>();
    collider.has = definition.components.collider.has;

    if (collider.has)
    {
        collider.center.x = definition.components.collider.center.x;
        collider.center.y = definition.components.collider.center.y;
        collider.center.z = definition.components.collider.center.z;
        collider.size.x = definition.components.collider.size.x;
        collider.size.y = definition.components.collider.size.y;
        collider.size.z = definition.components.collider.size.z;
        collider.enabled = definition.components.collider.enabled;
        collider.isTrigger = definition.components.collider.isTrigger;
    }
}

template <IsComponent T>
T& Entity::GetComponent()
{
    return EntityMemoryPool::Instance().GetComponent<T>(m_id);
}

template <IsComponent T>
bool Entity::HasComponent() const
{
    return EntityMemoryPool::Instance().HasComponent<T>(m_id);
}

// Explicit template instantiation for specific types
template CORE_API TransformComponent& Entity::GetComponent<TransformComponent>();
template CORE_API MeshComponent& Entity::GetComponent<MeshComponent>();
template CORE_API DirectionalLightComponent& Entity::GetComponent<DirectionalLightComponent>();
template CORE_API MaterialComponent& Entity::GetComponent<MaterialComponent>();
template CORE_API ScriptComponent& Entity::GetComponent<ScriptComponent>();
template CORE_API ChildrenComponent& Entity::GetComponent<ChildrenComponent>();
template CORE_API ParentComponent& Entity::GetComponent<ParentComponent>();
template CORE_API NativeScriptComponent& Entity::GetComponent<NativeScriptComponent>();
template CORE_API CameraComponent& Entity::GetComponent<CameraComponent>();
template CORE_API UiComponent& Entity::GetComponent<UiComponent>();
template CORE_API ColliderComponent& Entity::GetComponent<ColliderComponent>();
template CORE_API bool Entity::HasComponent<TransformComponent>() const;
template CORE_API bool Entity::HasComponent<MeshComponent>() const;
template CORE_API bool Entity::HasComponent<DirectionalLightComponent>() const;
template CORE_API bool Entity::HasComponent<MaterialComponent>() const;
template CORE_API bool Entity::HasComponent<ScriptComponent>() const;
template CORE_API bool Entity::HasComponent<ChildrenComponent>() const;
template CORE_API bool Entity::HasComponent<ParentComponent>() const;
template CORE_API bool Entity::HasComponent<NativeScriptComponent>() const;
template CORE_API bool Entity::HasComponent<CameraComponent>() const;
template CORE_API bool Entity::HasComponent<UiComponent>() const;
template CORE_API bool Entity::HasComponent<ColliderComponent>() const;

