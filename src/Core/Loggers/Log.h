#ifndef CORE_LOGGERS_LOG_H
#define CORE_LOGGERS_LOG_H

#include <string>
#include <chrono>
#include "LogLevel.h"

namespace DreamEngine::Core::Loggers
{
struct Log
{
    std::chrono::time_point<std::chrono::system_clock> dateTime;
    std::string message;
    LogLevel logLevel;
};
}  // namespace DreamEngine::Core::Math
#endif