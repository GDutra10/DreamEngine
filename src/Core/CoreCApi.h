#include "CoreExport.h"

// logs
CORE_API_C void CORE_CALL Core_LogTrace(const char* msg) noexcept;
CORE_API_C void CORE_CALL Core_LogDebug(const char* msg) noexcept;
CORE_API_C void CORE_CALL Core_LogInfo(const char* msg) noexcept;
CORE_API_C void CORE_CALL Core_LogWarning(const char* msg) noexcept;
CORE_API_C void CORE_CALL Core_LogError(const char* msg) noexcept;

// ui manager
CORE_API_C bool CORE_CALL Core_UiManagerSetString(size_t entityId, const char* prop, const char* value) noexcept;
CORE_API_C bool CORE_CALL Core_UiManagerSetInt(size_t entityId, const char* prop, const int value) noexcept;
CORE_API_C bool CORE_CALL Core_UiManagerSetFloat(size_t entityId, const char* prop, const float value) noexcept;
CORE_API_C bool CORE_CALL Core_UiManagerBindOnClickCallback(size_t entityId, const char* event, int eventId) noexcept;