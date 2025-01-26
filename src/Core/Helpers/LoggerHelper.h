#ifndef CORE_HELPERS_LOGGER_HELPER_H
#define CORE_HELPERS_LOGGER_HELPER_H

#include <string>
#include "../Loggers/Log.h"

namespace DreamEngine::Core::Helpers
{
using namespace DreamEngine::Core::Loggers;

class LoggerHelper
{
   public:
    static std::string GetLogLevel(const Log& log)
    {
        switch (log.logLevel)
        {
            case LogLevel::Trace:
                return "[TRACE]";
            case LogLevel::Debug:
                return "[DEBUG]";
            case LogLevel::Info:
                return "[INFO]";
            case LogLevel::Warning:
                return "[WARN]";
            case LogLevel::Error:
                return "[ERROR]";
            default:
                return "[NONE]";
        }
    }
};
}  // namespace DreamEngine::Core::Helpers
#endif