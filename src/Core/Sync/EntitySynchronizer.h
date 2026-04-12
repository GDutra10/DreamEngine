#ifndef CORE_SYNC_ENTITY_SYNCHRONIZER_H_
#define CORE_SYNC_ENTITY_SYNCHRONIZER_H_

#include "EntityData.h"
#include "ECS/Entity.h"
#include "ECS/Components/ParentComponent.h"


namespace DreamEngine::Core::Sync
{
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::ECS::Components;
class EntitySynchronizer
{
   public:
    static void SynchronizeToData(Entity* entity);

    static void SynchronizeFromData(Entity* entity);

   private:
    static void HandleParentComponent(Entity* entity);
    static void RemoveEntityFromParent(Entity* entity, ParentComponent& parentComponent);
    static void AddEntityToParent(Entity* entity, ParentComponent& parentComponent);
};
}  // namespace DreamEngine::Core::Sync
#endif