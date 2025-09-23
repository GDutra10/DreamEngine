#include "Entity.h"

#include "EntityMemoryPool.h"

using namespace DreamEngine::Core::ECS;

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

glm::mat4 Entity::GetTransform()
{
    const TransformComponent& transformComponent = GetComponent<TransformComponent>();
    const ParentComponent& parentComponent = GetComponent<ParentComponent>();

    if (parentComponent.has && parentComponent.parent != nullptr)
        return parentComponent.parent->GetTransform() * transformComponent.transform;

    return transformComponent.transform;
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
template CORE_API bool Entity::HasComponent<TransformComponent>() const;
template CORE_API bool Entity::HasComponent<MeshComponent>() const;
template CORE_API bool Entity::HasComponent<DirectionalLightComponent>() const;
template CORE_API bool Entity::HasComponent<MaterialComponent>() const;
template CORE_API bool Entity::HasComponent<ScriptComponent>() const;
template CORE_API bool Entity::HasComponent<ChildrenComponent>() const;
template CORE_API bool Entity::HasComponent<ParentComponent>() const;
template CORE_API bool Entity::HasComponent<NativeScriptComponent>() const;
template CORE_API bool Entity::HasComponent<CameraComponent>() const;

