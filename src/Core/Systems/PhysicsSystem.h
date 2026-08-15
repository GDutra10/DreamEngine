#ifndef CORE_SYSTEM_PHYSICS_SYSTEM_H_
#define CORE_SYSTEM_PHYSICS_SYSTEM_H_

#include <vector>
#include "../ECS/EntityManager.h"
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
    void UpdateColliders(EntityManager& entityManager);
    void DetectCollisions();
};
}  // namespace DreamEngine::Core::Systems
#endif