#ifndef CORE_LOGGERS_LOGGER_SINGLETON_H
#define CORE_LOGGERS_LOGGER_SINGLETON_H

#include <algorithm>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Log.h"
#include "Logger.h"
#include "LogLevel.h"

namespace DreamEngine::Core::Loggers
{
class CORE_API LoggerSingleton
{
   public:
    static LoggerSingleton& Instance();

    void LogTrace(const std::string& message);

    void LogDebug(const std::string& message);

    void LogInfo(const std::string& message);

    void LogWarning(const std::string& message);

    void LogError(const std::string& message);

    void Attach(Logger* logger);

    void Detach(Logger* logger);

    void HandleMessages();

   private:
    std::thread m_loggerThread;
    std::atomic<bool> m_stopFlag;
    std::vector<Logger*> m_loggers;
    std::queue<Log> m_logs;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    LoggerSingleton();

    ~LoggerSingleton();

    void LogMessage(const LogLevel logLevel, const std::string& message);
};
}  // namespace DreamEngine::Core::Loggers
#endif