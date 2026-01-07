#include "EntityMemoryPool.h"

#include "Loggers/LoggerSingleton.h"

#ifndef MAX_ENTITIES
#define MAX_ENTITIES 1000
#endif

#ifndef NEW_ENTITY_NAME
#define NEW_ENTITY_NAME "New Entity"
#endif

using namespace DreamEngine::Core::ECS;

EntityMemoryPool& EntityMemoryPool::Instance()
{
    static EntityMemoryPool pool(MAX_ENTITIES);

    return pool;
}

std::string& EntityMemoryPool::GetTag(const size_t entityId) { return m_tags[entityId]; }

std::string& EntityMemoryPool::GetName(const size_t entityId) { return m_names[entityId]; }

bool EntityMemoryPool::GetActive(const size_t entityId) const { return m_active[entityId]; }

void EntityMemoryPool::SetActive(const size_t entityId, const bool value) { m_active[entityId] = value; }

size_t EntityMemoryPool::AddEntity(const std::string& tag)
{
    const size_t index = GetNextEntityIndex();

    // reset all entity or reallocate the vector if need
    if (m_tags.size() == index)
    {
        m_tags.emplace_back(tag);
        m_active.emplace_back(true);
        m_names.emplace_back(NEW_ENTITY_NAME);

        std::get<std::vector<TransformComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<MeshComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<DirectionalLightComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<MaterialComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<ScriptComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<ChildrenComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<ParentComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<NativeScriptComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<CameraComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<OutlineComponent>>(m_entityComponentPool).emplace_back();
        std::get<std::vector<UiComponent>>(m_entityComponentPool).emplace_back();

        m_numEntities++;
    }
    else 
    {
        m_tags[index] = tag;
        m_active[index] = true;
        m_names[index] = NEW_ENTITY_NAME;

        std::get<std::vector<TransformComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<MeshComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<DirectionalLightComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<MaterialComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<ScriptComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<ChildrenComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<ParentComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<NativeScriptComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<CameraComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<OutlineComponent>>(m_entityComponentPool)[index].has = false;
        std::get<std::vector<UiComponent>>(m_entityComponentPool)[index].has = false;
    }

    return index;
}

void EntityMemoryPool::RemoveEntity(const size_t entityId)
{
    m_active[entityId] = false;
    m_tags[entityId] = "";
    m_names[entityId] = "";

    std::get<std::vector<TransformComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<MeshComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<DirectionalLightComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<MaterialComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<ScriptComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<ChildrenComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<ParentComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<CameraComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<OutlineComponent>>(m_entityComponentPool)[entityId].has = false;
    std::get<std::vector<UiComponent>>(m_entityComponentPool)[entityId].has = false;
    NativeScriptComponent& nativeComponent = std::get<std::vector<NativeScriptComponent>>(m_entityComponentPool)[entityId];
    nativeComponent.has = false;
    
    if (nativeComponent.script != nullptr)
        delete nativeComponent.script;
}

EntityMemoryPool::EntityMemoryPool(size_t maxEntities) : m_numEntities(maxEntities)
{
    Loggers::LoggerSingleton::Instance().LogTrace("EntityMemoryPool::EntityMemoryPool -> Creating!");
    m_tags = std::vector<std::string>(m_numEntities);
    m_names = std::vector<std::string>(m_numEntities);
    m_active = std::vector<bool>(m_numEntities);
    m_entityComponentPool =
        std::make_tuple(std::vector<TransformComponent>(m_numEntities),
                        std::vector<MeshComponent>(m_numEntities),
                        std::vector<DirectionalLightComponent>(m_numEntities),
                        std::vector<MaterialComponent>(m_numEntities),
                        std::vector<ScriptComponent>(m_numEntities),
                        std::vector<ChildrenComponent>(m_numEntities),
                        std::vector<ParentComponent>(m_numEntities),
                        std::vector<NativeScriptComponent>(m_numEntities), 
                        std::vector<CameraComponent>(m_numEntities),
                        std::vector<OutlineComponent>(m_numEntities),
                        std::vector<UiComponent>(m_numEntities)
            );

        for (size_t index = 0; index < m_numEntities; index++) 
        {
            // allocate entities
            m_active[index] = false;
            m_tags[index] = "default";
            m_names[index] = "";

            // allocate components
            // TODO: do for another components
            std::get<std::vector<TransformComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<MeshComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<DirectionalLightComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<MaterialComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<ScriptComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<ChildrenComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<ParentComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<NativeScriptComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<CameraComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<OutlineComponent>>(m_entityComponentPool)[index].has = false;
            std::get<std::vector<UiComponent>>(m_entityComponentPool)[index].has = false;
        }
}

size_t EntityMemoryPool::GetNextEntityIndex()
{
    const auto size = m_active.size();

    for (size_t i = 0; i < size; i++)
    {
        if (m_active[i] == false)
            return i;
    }

    return size + 1;
}

template <IsComponent T>
T& EntityMemoryPool::GetComponent(size_t entityId)
{
    return std::get<std::vector<T>>(m_entityComponentPool)[entityId];
}

template <IsComponent T>
bool EntityMemoryPool::HasComponent(size_t entityId)
{
    auto& component = std::get<std::vector<T>>(m_entityComponentPool)[entityId];

    return component.has;
}

// Explicit template instantiation for specific types
template TransformComponent& EntityMemoryPool::GetComponent<TransformComponent>(size_t entityId);
template MeshComponent& EntityMemoryPool::GetComponent<MeshComponent>(size_t entityId);
template DirectionalLightComponent& EntityMemoryPool::GetComponent<DirectionalLightComponent>(size_t entityId);
template MaterialComponent& EntityMemoryPool::GetComponent<MaterialComponent>(size_t entityId);
template ScriptComponent& EntityMemoryPool::GetComponent<ScriptComponent>(size_t entityId);
template ChildrenComponent& EntityMemoryPool::GetComponent<ChildrenComponent>(size_t entityId);
template ParentComponent& EntityMemoryPool::GetComponent<ParentComponent>(size_t entityId);
template NativeScriptComponent& EntityMemoryPool::GetComponent<NativeScriptComponent>(size_t entityId);
template CameraComponent& EntityMemoryPool::GetComponent<CameraComponent>(size_t entityId);
template OutlineComponent& EntityMemoryPool::GetComponent<OutlineComponent>(size_t entityId);
template UiComponent& EntityMemoryPool::GetComponent<UiComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<TransformComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<MeshComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<DirectionalLightComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<MaterialComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<ScriptComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<ChildrenComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<ParentComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<NativeScriptComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<CameraComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<OutlineComponent>(size_t entityId);
template bool EntityMemoryPool::HasComponent<UiComponent>(size_t entityId);
