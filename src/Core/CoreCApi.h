#ifndef CORE_C_API_H_
#define CORE_C_API_H_

#include "CoreExport.h"

#include <stdint.h>

typedef struct CoreAudioPlayOptions
{
    float volume;
    float pitch;

    uint8_t loop;
    uint8_t spatial;

    uint32_t bus;
} CoreAudioPlayOptions;

typedef struct CoreAudioHandle
{
    uint32_t id;
    uint32_t generation;
} CoreAudioHandle;

// logs
CORE_API_C void CORE_CALL Core_LogTrace(const char* msg) noexcept;
CORE_API_C void CORE_CALL Core_LogDebug(const char* msg) noexcept;
CORE_API_C void CORE_CALL Core_LogInfo(const char* msg) noexcept;
CORE_API_C void CORE_CALL Core_LogWarning(const char* msg) noexcept;
CORE_API_C void CORE_CALL Core_LogError(const char* msg) noexcept;

// ui manager
CORE_API_C bool CORE_CALL Core_UiManagerSetString(uint32_t entityId, const char* prop, const char* value) noexcept;
CORE_API_C bool CORE_CALL Core_UiManagerSetInt(uint32_t entityId, const char* prop, const int value) noexcept;
CORE_API_C bool CORE_CALL Core_UiManagerSetFloat(uint32_t entityId, const char* prop, const float value) noexcept;
CORE_API_C bool CORE_CALL Core_UiManagerBindOnClickCallback(uint32_t entityId, const char* event, int eventId) noexcept;

// scene manager
CORE_API_C unsigned int CORE_CALL Core_SceneManagerCreateEntity(const char* tag, const char* name) noexcept;
CORE_API_C bool CORE_CALL Core_SceneManagerDestroyEntity(uint32_t entityId) noexcept;
CORE_API_C bool CORE_CALL Core_SceneManagerSetMainCamera(unsigned int entityId) noexcept;
CORE_API_C bool CORE_CALL Core_SceneManagerSetShowCursor(bool showCursor) noexcept;
CORE_API_C bool CORE_CALL Core_SceneManagerSetGlobalLight(float r, float g, float b, float intensity) noexcept;
CORE_API_C bool CORE_CALL Core_SceneManagerChangeScene(const char* sceneName) noexcept;

// resource manager
CORE_API_C int CORE_CALL Core_PrefabInstantiate(const char* resourceId) noexcept;

// audio system
CORE_API_C CoreAudioHandle CORE_CALL Core_AudioSystemPlay(const char* resourceId, CoreAudioPlayOptions options) noexcept;
CORE_API_C CoreAudioHandle CORE_CALL Core_AudioSystemPlayByEntity(uint32_t entityId, const char* resourceId, CoreAudioPlayOptions options) noexcept;
CORE_API_C void CORE_CALL Core_AudioSystemStop(CoreAudioHandle handle) noexcept;
CORE_API_C void CORE_CALL Core_AudioSystemResume(CoreAudioHandle handle) noexcept;
CORE_API_C void CORE_CALL Core_AudioSystemPause(CoreAudioHandle handle) noexcept;

#endif  // CORE_C_API_H_