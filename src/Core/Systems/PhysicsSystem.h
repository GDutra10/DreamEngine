#ifndef CORE_SYSTEM_PHYSICS_SYSTEM_H_
#define CORE_SYSTEM_PHYSICS_SYSTEM_H_

#include <vector>
#include "../ECS/EntityManager.h"
#include "../Physics/CollisionEvent.h"
#include "../Physics/CollisionPair.h"
#include "../Physics/PhysicsCollider.h"

namespace DreamEngine::Core::Systems
{
    using namespace DreamEngine::Core::ECS;
    using namespace DreamEngine::Core::Physics;
class PhysicsSystem
{
   public:
    void Update(EntityManager& entityManager, float deltaTime);

   private:
    std::vector<PhysicsCollider> m_colliders;
    std::vector<CollisionPair> m_previousCollisions;
    std::vector<CollisionPair> m_currentCollisions;
    std::vector<CollisionEvent> m_collisionEvents;
    void UpdateColliders(EntityManager& entityManager);
    void DetectCollisions();
    void ProcessCollisionEvents();
};
}  // namespace DreamEngine::Core::Systems
#endif