#include "ScriptEventHandler.h"
#include "ScriptEngine.h"
#include "../ECS/Components/NativeScriptComponent.h"
#include "../ECS/Components/ScriptComponent.h"

using namespace DreamEngine::Core::Scripting;
using namespace DreamEngine::Core::ECS::Components;

std::queue<uint32_t> ScriptEventHandler::m_sEventIds;
std::queue<CollisionEvent> ScriptEventHandler::m_sCollisionEvents;

void ScriptEventHandler::QueueEvent(uint32_t eventId)
{
    m_sEventIds.emplace(eventId);
}

void ScriptEventHandler::QueueCollisionEvent(const CollisionEvent& event)
{
    m_sCollisionEvents.emplace(event);
}

void ScriptEventHandler::Process()
{
    while (!m_sEventIds.empty())
    {
        uint32_t eventId = m_sEventIds.front();
        m_sEventIds.pop();

        // Process the event with eventId
        ScriptEngine::ProcessEvent(eventId);
    }
}

void ScriptEventHandler::ProcessCollisionEvents(EntityManager& entityManager, const bool mustProcessManaged)
{
    while (!m_sCollisionEvents.empty())
    {
        CollisionEvent& event = m_sCollisionEvents.front();
        m_sCollisionEvents.pop();

        Entity* entityA = entityManager.GetEntityById(event.entityA);

        Entity* entityB = entityManager.GetEntityById(event.entityB);

        if (entityA == nullptr || entityB == nullptr)
            return;

        // native script
        DispatchToNative(entityA, entityB, event);
        DispatchToNative(entityB, entityA, event);

        // managed script
        if (mustProcessManaged)
        {
            DispatchToManaged(entityA, entityB, event);
            DispatchToManaged(entityB, entityA, event);
        }
    }
}

void ScriptEventHandler::DispatchToNative(Entity* owner, Entity* other, const CollisionEvent& event) 
{
    NativeScriptComponent& component = owner->GetComponent<NativeScriptComponent>();

    if (!component.has || component.script == nullptr)
        return;

    NativeScript* script = component.script;

    if (event.isTrigger)
    {
        switch (event.type)
        {
            case CollisionEventType::Enter:
                script->OnTriggerEnter(other);
                break;

            case CollisionEventType::Stay:
                script->OnTriggerStay(other);
                break;

            case CollisionEventType::Exit:
                script->OnTriggerExit(other);
                break;
        }

        return;
    }

    switch (event.type)
    {
        case CollisionEventType::Enter:
            script->OnCollisionEnter(other);
            break;

        case CollisionEventType::Stay:
            script->OnCollisionStay(other);
            break;

        case CollisionEventType::Exit:
            script->OnCollisionExit(other);
            break;
    }
}

void ScriptEventHandler::DispatchToManaged(Entity* owner, Entity* other, const CollisionEvent& event) 
{
    ScriptComponent& component = owner->GetComponent<ScriptComponent>();

    if (!component.has)
        return;

    unsigned int typeUint = static_cast<unsigned int>(event.type);
    ScriptEngine::ProcessCollisionEvent(owner->GetId(), other->GetId(), typeUint, event.isTrigger);
}
