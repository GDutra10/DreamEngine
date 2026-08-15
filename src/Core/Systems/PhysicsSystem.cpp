#include "PhysicsSystem.h"
#include "../ECS/Components/ColliderComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../Loggers/LoggerSingleton.h"
#include "../Math/Math.h"

using namespace DreamEngine::Core::Systems;
using namespace DreamEngine::Core::Loggers;

void PhysicsSystem::Update(EntityManager& entityManager, float deltaTime) 
{
    m_colliders.clear();

    UpdateColliders(entityManager);
    DetectCollisions();
}

void PhysicsSystem::UpdateColliders(EntityManager& entityManager)
{
    for (Entity* entity : entityManager.GetEntities())
    {
        if (!entity->GetIsActive())
            continue;

        ColliderComponent& collider = entity->GetComponent<ColliderComponent>();

        if (!collider.has || !collider.enabled)
            continue;

        const glm::mat4 worldTransform = entity->GetWorldTransform();

        collider.localBounds = AABB::FromCenterSize(collider.center, collider.size);
        collider.worldBounds = TransformAABB(collider.localBounds, worldTransform);

        m_colliders.push_back({entity, &collider});
    }
}

void PhysicsSystem::DetectCollisions() 
{
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_colliders.size(); ++j)
        {
            const auto& a = m_colliders[i];
            const auto& b = m_colliders[j];

            if (a.collider->worldBounds.Intersects(b.collider->worldBounds))
            {
                // A <-> B
                LoggerSingleton::Instance().LogDebug("Collision Detected");
            }
        }
    }
}
