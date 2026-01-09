#include "CoreCApi.h"

#include "Application.h"
#include "ECS/Components/UiComponent.h"
#include "Loggers/LoggerSingleton.h"
#include "Scripting/ScriptEventHandler.h"
#include "UI/UiManager.h"

// logs

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::UI;

void CORE_CALL Core_LogTrace(const char* msg) noexcept
{
    LoggerSingleton::Instance().LogTrace(msg);
}

void CORE_CALL Core_LogDebug(const char* msg) noexcept
{
    LoggerSingleton::Instance().LogDebug(msg);
}

void CORE_CALL Core_LogInfo(const char* msg) noexcept
{
    LoggerSingleton::Instance().LogInfo(msg);
}

void CORE_CALL Core_LogWarning(const char* msg) noexcept
{
    LoggerSingleton::Instance().LogWarning(msg);
}

void CORE_CALL Core_LogError(const char* msg) noexcept
{
    LoggerSingleton::Instance().LogError(msg);
}

// ui manager

bool CORE_CALL Core_UiManagerSetString(size_t entityId, const char* prop, const char* value) noexcept
{
    Entity* entity = Application::Instance().GetGame()->GetActiveScene()->GetEntityManager()->GetEntityById(entityId);

    if (entity == nullptr)
        return false;

    UiComponent& uiComponent = entity->GetComponent<UiComponent>();

    if (!uiComponent.has || uiComponent.instance == nullptr)
        return false;

    std::string valueStr(value);
    UiManager::Set(uiComponent.instance, std::string(prop), valueStr);

    return true;
}

bool CORE_CALL Core_UiManagerSetInt(size_t entityId, const char* prop, const int value) noexcept
{
    Entity* entity = Application::Instance().GetGame()->GetActiveScene()->GetEntityManager()->GetEntityById(entityId);

    if (entity == nullptr)
        return false;

    UiComponent& uiComponent = entity->GetComponent<UiComponent>();

    if (!uiComponent.has || uiComponent.instance == nullptr)
        return false;

    UiManager::Set(uiComponent.instance, std::string(prop), value);

    return true;
}

bool CORE_CALL Core_UiManagerSetFloat(size_t entityId, const char* prop, const float value) noexcept
{
    Entity* entity = Application::Instance().GetGame()->GetActiveScene()->GetEntityManager()->GetEntityById(entityId);

    if (entity == nullptr)
        return false;

    UiComponent& uiComponent = entity->GetComponent<UiComponent>();

    if (!uiComponent.has || uiComponent.instance == nullptr)
        return false;

    UiManager::Set(uiComponent.instance, std::string(prop), value);

    return true;
}

bool CORE_CALL Core_UiManagerBindOnClickCallback(size_t entityId, const char* event, int eventId) noexcept
{
    Entity* entity = Application::Instance().GetGame()->GetActiveScene()->GetEntityManager()->GetEntityById(entityId);

    if (entity == nullptr)
        return false;

    UiComponent& uiComponent = entity->GetComponent<UiComponent>();

    if (!uiComponent.has || uiComponent.instance == nullptr)
        return false;
    
    UiManager::BindOnClickCallback(uiComponent.instance, std::string(event), [eventId]()
    {
        ScriptEventHandler::QueueEvent(eventId);
    });

    return true;
}