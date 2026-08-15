#include "PhysicsSystem.h"
#include "../ECS/Components/ColliderComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../Math/Math.h"
#include "../Scripting/ScriptEventHandler.h"

using namespace DreamEngine::Core::Systems;
using namespace DreamEngine::Core::Scripting;

void PhysicsSystem::Update(EntityManager& entityManager, float deltaTime) 
{
    m_previousCollisions = m_currentCollisions;
    m_currentCollisions.clear();

    m_colliders.clear();

    UpdateColliders(entityManager);
    DetectCollisions();
    
    ProcessCollisionEvents();
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

            if (!a.collider->worldBounds.Intersects(b.collider->worldBounds))
                continue;

            CollisionPair pair = CollisionPair::MakePair(a.entity->GetId(), b.entity->GetId());
            pair.isTrigger = a.collider->isTrigger || b.collider->isTrigger;

            m_currentCollisions.push_back(pair);
        }
    }
}

void PhysicsSystem::ProcessCollisionEvents()
{
    m_collisionEvents.clear();

    for (const CollisionPair& pair : m_currentCollisions)
    {
        const bool existedLastFrame = std::find(m_previousCollisions.begin(), m_previousCollisions.end(), pair) != m_previousCollisions.end();

        m_collisionEvents.push_back(CollisionEvent{
            pair.entityA,
            pair.entityB,
            existedLastFrame ? CollisionEventType::Stay : CollisionEventType::Enter,
            pair.isTrigger
        });
    }

    for (const CollisionPair& pair : m_previousCollisions)
    {
        const bool stillExists = std::find(m_currentCollisions.begin(), m_currentCollisions.end(), pair) != m_currentCollisions.end();

        if (!stillExists)
            m_collisionEvents.push_back(CollisionEvent{pair.entityA, pair.entityB, CollisionEventType::Exit, pair.isTrigger});
    }

    for (const CollisionEvent& event : m_collisionEvents)
        ScriptEventHandler::QueueCollisionEvent(event);
}
