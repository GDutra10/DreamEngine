#ifndef CORE_SYNC_ENTITY_SYNCHRONIZER_H_
#define CORE_SYNC_ENTITY_SYNCHRONIZER_H_

#include "EntityData.h"
#include "../ECS/Entity.h"


namespace DreamEngine::Core::Sync
{
using namespace DreamEngine::Core::ECS;
class EntitySynchronizer
{
   public:
    static void SynchronizeToData(Entity* entity);

    static void SynchronizeFromData(Entity* entity);
};
}  // namespace DreamEngine::Core::Sync
#endif