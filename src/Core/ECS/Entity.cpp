#include "Entity.h"

#include "EntityMemoryPool.h"

using namespace DreamEngine::Core::ECS;

void Entity::Destroy() const
{
    EntityMemoryPool::Instance().SetActive(m_id, false);
}

bool Entity::IsActive() const
{
    return EntityMemoryPool::Instance().GetActive(m_id);
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

void Entity::SetActive(const bool value) const
{
    EntityMemoryPool::Instance().SetActive(m_id, value);
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
template TransformComponent& Entity::GetComponent<TransformComponent>();
template MeshComponent& Entity::GetComponent<MeshComponent>();
template DirectionalLightComponent& Entity::GetComponent<DirectionalLightComponent>();
template MaterialComponent& Entity::GetComponent<MaterialComponent>();
template ScriptComponent& Entity::GetComponent<ScriptComponent>();
template ChildrenComponent& Entity::GetComponent<ChildrenComponent>();
template ParentComponent& Entity::GetComponent<ParentComponent>();
template bool Entity::HasComponent<TransformComponent>() const;
template bool Entity::HasComponent<MeshComponent>() const;
template bool Entity::HasComponent<DirectionalLightComponent>() const;
template bool Entity::HasComponent<MaterialComponent>() const;
template bool Entity::HasComponent<ScriptComponent>() const;
template bool Entity::HasComponent<ChildrenComponent>() const;
template bool Entity::HasComponent<ParentComponent>() const;