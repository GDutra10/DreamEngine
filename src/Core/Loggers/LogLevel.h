#ifndef CORE_LOGGERS_LOG_LEVEL_H
#define CORE_LOGGERS_LOG_LEVEL_H

#include <chrono>
#include <string>
#include "CoreExport.h"

namespace DreamEngine::Core::Loggers
{
enum CORE_API LogLevel
{
    Trace,
    Debug,
    Info,
    Warning,
    Error,
};
}  // namespace DreamEngine::Core::Loggers
#endif