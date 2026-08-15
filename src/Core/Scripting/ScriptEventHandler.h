#ifndef CORE_SCRIPTING_SCRIPT_EVENT_HANDLER_H_
#define CORE_SCRIPTING_SCRIPT_EVENT_HANDLER_H_

#include <queue>
#include "../Physics/CollisionEvent.h"
#include "../ECS/EntityManager.h"

namespace DreamEngine::Core::Scripting
{
    using namespace DreamEngine::Core::ECS;
    using namespace DreamEngine::Core::Physics;
class ScriptEventHandler
{
public:
    static void QueueEvent(uint32_t eventId);
    static void QueueCollisionEvent(const CollisionEvent& event);
    static void Process();
    static void ProcessCollisionEvents(EntityManager& entityManager, const bool mustProcessManaged);
   private:
    static std::queue<uint32_t> m_sEventIds;
    static std::queue<CollisionEvent> m_sCollisionEvents;
    static void DispatchToNative(Entity* owner, Entity* other, const CollisionEvent& event);
    static void DispatchToManaged(Entity* owner, Entity* other, const CollisionEvent& event);
};
}  // namespace DreamEngine::Core::Scripting
#endif