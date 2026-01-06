#ifndef CORE_ECS_ENTITY_MEMORY_POOL_H_
#define CORE_ECS_ENTITY_MEMORY_POOL_H_
#include <vector>
#include "Entity.h"
#include "Components/Component.h"
#include "Components/TransformComponent.h"
#include "Components/MeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/ChildrenComponent.h"
#include "Components/ParentComponent.h"
#include "Components/NativeScriptComponent.h"
#include "Components/CameraComponent.h"
#include "Components/OutlineComponent.h"

namespace DreamEngine::Core::ECS
{

using namespace DreamEngine::Core::ECS::Components;

typedef std::tuple<std::vector<TransformComponent>,
                   std::vector<MeshComponent>,
                   std::vector<DirectionalLightComponent>,
                   std::vector<MaterialComponent>,
                   std::vector<ScriptComponent>,
                   std::vector<ChildrenComponent>,
                   std::vector<ParentComponent>,
                   std::vector<NativeScriptComponent>,
                   std::vector<CameraComponent>,
                   std::vector<OutlineComponent>
   > EntityComponentVectorTuple;

class CORE_API EntityMemoryPool
{
   public:
    static EntityMemoryPool& Instance();
    std::string& GetTag(size_t entityId);
    std::string& GetName(size_t entityId);
    bool GetActive(size_t entityId) const;
    void SetActive(size_t entityId, bool value);
    size_t AddEntity(const std::string& tag);
    void RemoveEntity(size_t entityId);

    template <IsComponent T>
    T& GetComponent(size_t entityId);

    template <IsComponent T>
    bool HasComponent(size_t entityId);

   private:
    size_t m_numEntities;
    std::vector<std::string> m_tags;
    std::vector<std::string> m_names;
    std::vector<bool> m_active;
    EntityComponentVectorTuple m_entityComponentPool;

    EntityMemoryPool(size_t maxEntities);

    size_t GetNextEntityIndex();
};
}  // namespace DreamEngine::Core::ECS
#endif