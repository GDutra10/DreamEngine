#include "LoggerHelper.h"

using namespace DreamEngine::Core::Helpers;

std::string LoggerHelper::GetLogLevel(const Log& log)
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