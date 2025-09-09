#ifndef CORE_LOGGERS_LOGGER_H
#define CORE_LOGGERS_LOGGER_H

#include <chrono>
#include <string>
#include "Log.h"

namespace DreamEngine::Core::Loggers
{
class CORE_API Logger abstract
{
   public:
    virtual ~Logger() = default;
    virtual void Process(Log log) = 0;
};
}  // namespace DreamEngine::Core::Loggers
#endif