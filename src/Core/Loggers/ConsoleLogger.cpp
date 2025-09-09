#include "ConsoleLogger.h"

void DreamEngine::Core::Loggers::ConsoleLogger::Process(const Log log)
{
    std::cout << GetColor(log.logLevel) << DateHelper::ToString(log.dateTime) << " " << LoggerHelper::GetLogLevel(log) << " " << log.message << "\r\n";
}

std::string DreamEngine::Core::Loggers::ConsoleLogger::GetColor(const LogLevel logLevel)
{
    switch (logLevel)
    {
        case LogLevel::Trace:
            return LOG_BLUE;
        case LogLevel::Debug:
            return LOG_GREEN;
        case LogLevel::Info:
            return LOG_RESET;
        case LogLevel::Warning:
            return LOG_YELLOW;
        case LogLevel::Error:
            return LOG_RED;
        default:
            return LOG_RESET;
    }
}