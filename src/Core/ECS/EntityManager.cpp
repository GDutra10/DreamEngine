#include "EntityManager.h"

#include "EntityMemoryPool.h"
#include "../Loggers/LoggerSingleton.h"
#include "UI/UiManager.h"

using namespace DreamEngine::Core::ECS;

EntityManager::EntityManager(): m_totalEntities(0)
{
    // just initialize the pool
	EntityMemoryPool::Instance();
}

void EntityManager::Update()
{
    for (auto entity : m_entitiesToRemove)
    {
        const auto entityTag = EntityMemoryPool::Instance().GetTag(entity->GetId());
        m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());

        std::vector<Entity*> entityVecMap = m_entityMap[entityTag];

        // if have more than one tag, remove the entity from the vector
        if (entityVecMap.size() > 1)
        {
            entityVecMap.erase(std::remove(entityVecMap.begin(), entityVecMap.end(), entity), entityVecMap.end());
            m_entityMap[entityTag] = entityVecMap;
        }
        else  // remove the tag in entity_map
            m_entityMap.erase(entityTag);

        EntityMemoryPool::Instance().RemoveEntity(entity->GetId());

        delete entity;
    }

    for (auto entity : m_entitiesToAdd)
	{
		const auto entityTag = EntityMemoryPool::Instance().GetTag(entity->GetId());
		m_entities.push_back(entity);

		if (!m_entityMap.contains(entityTag))
			m_entityMap[entityTag] = std::vector<Entity*>();

		auto entityVecMap = m_entityMap[entityTag];
		entityVecMap.push_back(entity);
		m_entityMap[entityTag] = entityVecMap;
	}

	m_entitiesToAdd.clear();
    m_entitiesToRemove.clear();
}

Entity* EntityManager::AddEntity(const std::string& tag)
{
	Loggers::LoggerSingleton::Instance().LogTrace("EntityManager::AddEntity -> Adding entity with tag: " + tag);

    const auto entityId = EntityMemoryPool::Instance().AddEntity(tag);
	const auto entity = new Entity(entityId);

	m_entitiesToAdd.push_back(entity);
	m_totalEntities++;

	return entity;
}

Entity* EntityManager::AddEntity(const std::string& tag, Entity* parent)
{
    Entity* entity = AddEntity(tag);

	ParentComponent& parentComponent = entity->GetComponent<ParentComponent>();
    parentComponent.has = true;
    parentComponent.parent = parent;

	ChildrenComponent& childrenComponent = parent->GetComponent<ChildrenComponent>();
    childrenComponent.has = true;
    childrenComponent.children.push_back(entity);

	return entity;
}

void EntityManager::RemoveEntity(Entity* entity)
{
    Loggers::LoggerSingleton::Instance().LogTrace("EntityManager::RemoveEntity -> Removing entity with id: " + std::to_string(entity->GetId()));

    std::vector<Entity*> entitiesToRemove;

    const bool hasEntity = std::ranges::any_of(m_entitiesToRemove, [entity](Entity* e)
    {
        return e->GetIdentifier() == entity->GetIdentifier();
    });

    if (!hasEntity)
        entitiesToRemove.push_back(entity);

    AddChildToRemove(entity, entitiesToRemove);

	// remove entities
	for (Entity* entityToRemove : entitiesToRemove)
	{
        ParentComponent& parentComponent = entityToRemove->GetComponent<ParentComponent>();
        entityToRemove->GetComponent<ChildrenComponent>().children.clear();

	    if (parentComponent.parent != nullptr)
        {
            // remove from the parent children
            ChildrenComponent& parentChildrenComponent = parentComponent.parent->GetComponent<ChildrenComponent>();
            parentComponent.parent->GetComponent<ChildrenComponent>().children.erase(
                std::remove(parentChildrenComponent.children.begin(),
                            parentChildrenComponent.children.end(),
                            entityToRemove),
                parentComponent.parent->GetComponent<ChildrenComponent>().children.end());
            parentComponent.parent = nullptr;
        }

        m_entitiesToRemove.push_back(entityToRemove);
	}
    
    m_totalEntities = m_totalEntities - entitiesToRemove.size();
}

const std::vector<Entity*>& EntityManager::GetEntities() { return m_entities; }

const std::vector<Entity*>& EntityManager::GetEntities(const std::string& tag) { return m_entityMap[tag]; }

const std::map<std::string, std::vector<Entity*>>& EntityManager::GetEntityMap() { return m_entityMap; }

void EntityManager::Reset()
{
    for (Entity* entity : m_entities)
    {
        // reset components
        UiComponent& uiComponent = entity->GetComponent<UiComponent>();

        if (uiComponent.instance != nullptr)
        {
            UiManager::Destroy(uiComponent.instance);
            uiComponent.instance = nullptr;
            uiComponent.content = nullptr;
            uiComponent.zOrder = 0;
        }

        RemoveEntity(entity);
    }
}

void EntityManager::AddChildToRemove(Entity* entity, std::vector<Entity*>& entitiesToRemove)
{
    ChildrenComponent& childrenComponent = entity->GetComponent<ChildrenComponent>();

    for (Entity* child : childrenComponent.children)
    {
        entitiesToRemove.push_back(child);

        AddChildToRemove(child, entitiesToRemove);
    }
}