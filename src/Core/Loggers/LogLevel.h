#ifndef CORE_LOGGERS_LOG_LEVEL_H
#define CORE_LOGGERS_LOG_LEVEL_H

#include <chrono>
#include <string>

namespace DreamEngine::Core::Loggers
{
enum LogLevel
{
    Trace,
    Debug,
    Info,
    Warning,
    Error,
};
}  // namespace DreamEngine::Core::Loggers
#endif