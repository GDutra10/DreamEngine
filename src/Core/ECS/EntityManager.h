#ifndef CORE_ECS_ENTITY_MANAGER_H_
#define CORE_ECS_ENTITY_MANAGER_H_
#include <vector>
#include <map>
#include <string>
#include "Entity.h"

namespace DreamEngine::Core::ECS
{
class CORE_API EntityManager
{
   public:
    EntityManager();

    void Update();

    Entity* AddEntity(const std::string& tag);
    Entity* AddEntity(const std::string& tag, Entity* parent);
    Entity* GetEntityById(size_t id);
    void RemoveEntity(Entity* entity);
    const std::vector<Entity*>& GetEntities();
    const std::vector<Entity*>& GetEntities(const std::string& tag);
    const std::map<std::string, std::vector<Entity*>>& GetEntityMap();
    void Reset();

   private:
    size_t m_totalEntities;
    std::vector<Entity*> m_entities;
    std::vector<Entity*> m_entitiesToAdd;
    std::vector<Entity*> m_entitiesToRemove;
    std::map<std::string, std::vector<Entity*>> m_entityMap;
    static void AddChildToRemove(Entity* entity, std::vector<Entity*>& entitiesToRemove);
};
}  // namespace DreamEngine::Core::ECS
#endif