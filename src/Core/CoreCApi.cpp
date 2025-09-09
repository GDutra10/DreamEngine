#include "CoreCApi.h"
#include "Loggers/LoggerSingleton.h"

void CORE_CALL Core_LogTrace(const char* msg) noexcept
{
    DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogTrace(msg);
}

void CORE_CALL Core_LogDebug(const char* msg) noexcept
{
    DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogDebug(msg);
}

void CORE_CALL Core_LogInfo(const char* msg) noexcept
{
    DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogInfo(msg);
}

void CORE_CALL Core_LogWarning(const char* msg) noexcept
{
    DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogWarning(msg);
}

void CORE_CALL Core_LogError(const char* msg) noexcept
{
    DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogError(msg);
}