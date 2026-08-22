#include "CoreCApi.h"

#include "Application.h"
#include "ECS/Components/UiComponent.h"
#include "Loggers/LoggerSingleton.h"
#include "Scripting/ScriptEventHandler.h"
#include "UI/UiManager.h"
#include "Audio/AudioHandle.h"
#include "Audio/AudioPlayOptions.h"
#include "Systems/AudioSystem.h"

// logs

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Audio;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::UI;
using namespace DreamEngine::Core::Systems;

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

bool CORE_CALL Core_UiManagerSetString(uint32_t entityId, const char* prop, const char* value) noexcept
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

bool CORE_CALL Core_UiManagerSetInt(uint32_t entityId, const char* prop, const int value) noexcept
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

bool CORE_CALL Core_UiManagerSetFloat(uint32_t entityId, const char* prop, const float value) noexcept
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

bool CORE_CALL Core_UiManagerBindOnClickCallback(uint32_t entityId, const char* event, int eventId) noexcept
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

unsigned int CORE_CALL Core_SceneManagerCreateEntity(const char* tag, const char* name) noexcept
{
    Entity* entity = Application::Instance().GetGame()->GetActiveScene()->GetEntityManager()->AddEntity(tag);

    if (name[0] != '\0')
    {
        std::string& entityName = entity->GetName();
        entityName = name;
    }

    return entity->GetId();
}

bool CORE_CALL Core_SceneManagerDestroyEntity(uint32_t entityId) noexcept
{
    Entity* entity = Application::Instance().GetGame()->GetActiveScene()->GetEntityManager()->GetEntityById(entityId);

    if (entity == nullptr)
    {
        LoggerSingleton::Instance().LogWarning("Core_SceneManagerDestroyEntity -> Entity '" + std::to_string(entityId) + "' not found!");
        
        return false;
    }

    entity->Destroy();

    return true;
}

bool CORE_CALL Core_SceneManagerSetMainCamera(unsigned int entityId) noexcept
{
    Entity* entity = Application::Instance().GetGame()->GetActiveScene()->GetEntityManager()->GetEntityById(entityId);

    if (entity == nullptr)
        return false;

    Application::Instance().GetGame()->GetActiveScene()->SetMainCameraEntity(entity);
    
    return true;
}

bool CORE_CALL Core_SceneManagerSetShowCursor(bool showCursor) noexcept
{
    Application::Instance().GetGame()->GetActiveScene()->SetShowCursor(showCursor);
    
    return true;
}

bool CORE_CALL Core_SceneManagerSetGlobalLight(float r, float g, float b, float intensity) noexcept
{
    GlobalLight* globalLight = Application::Instance().GetGame()->GetActiveScene()->GetGlobalLight();
    globalLight->directionalLight.color = { r, g, b };
    globalLight->directionalLight.influence = intensity;

    return true;
}

bool CORE_CALL Core_SceneManagerChangeScene(const char* sceneName) noexcept
{
    Scene* scene = Application::Instance().GetGame()->GetActiveScene();

    return scene->ChangeScene(sceneName);
}

CORE_API_C int CORE_CALL Core_PrefabInstantiate(const char* resourceId) noexcept
{
    if (!resourceId)
        return -1;

    auto& application = Application::Instance();
    Scene* scene = application.GetGame()->GetActiveScene();

    if (!scene)
        return -1;

    return scene->InstantiatePrefab(resourceId);
}

CoreAudioHandle CORE_CALL Core_AudioSystemPlay(const char* resourceId, CoreAudioPlayOptions options) noexcept
{
    return Core_AudioSystemPlayByEntity(-1, resourceId, options);
}

CoreAudioHandle CORE_CALL Core_AudioSystemPlayByEntity(uint32_t entityId, const char* resourceId, CoreAudioPlayOptions options) noexcept
{
    AudioPlayOptions nativeOptions;

    nativeOptions.volume = options.volume;
    nativeOptions.pitch = options.pitch;
    nativeOptions.loop = options.loop != 0;
    nativeOptions.spatial = options.spatial != 0;
    nativeOptions.bus = static_cast<AudioBus>(options.bus);

    const AudioHandle handle = AudioSystem::Instance().Play(entityId, resourceId, nativeOptions);

    return { handle.id, handle.generation };
}

void CORE_CALL Core_AudioSystemStop(CoreAudioHandle handle) noexcept
{
    AudioSystem::Instance().Stop({handle.id, handle.generation});
}

void CORE_CALL Core_AudioSystemResume(CoreAudioHandle handle) noexcept
{
    AudioSystem::Instance().Resume({handle.id, handle.generation});
}

void CORE_CALL Core_AudioSystemPause(CoreAudioHandle handle) noexcept
{
    AudioSystem::Instance().Pause({handle.id, handle.generation});
}
