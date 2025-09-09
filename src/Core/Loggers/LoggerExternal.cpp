#include "LoggerExternal.h"
#include "LoggerSingleton.h"

extern "C" {

    CORE_API void coreLogTrace(const std::string& message)
    {
        return DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogTrace(message);
    }

    CORE_API void coreLogDebug(const std::string& message)
    {
        return DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogDebug(message);
    }

    CORE_API void coreLogInfo(const std::string& message)
    {
        return DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogInfo(message);
    }

    CORE_API void coreLogWarning(const std::string& message)
    {
        return DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogWarning(message);
    }

    CORE_API void coreLogError(const std::string& message)
    {
        return DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogError(message);
    }
}
