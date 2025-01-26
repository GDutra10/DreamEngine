#ifndef CORE_LOGGERS_CONSOLE_LOGGER_H
#define CORE_LOGGERS_CONSOLE_LOGGER_H

#define LOG_RESET "\x1B[0m"
#define LOG_RED "\x1B[31m"
#define LOG_GREEN "\x1B[32m"
#define LOG_YELLOW "\x1B[33m"
#define LOG_BLUE "\x1B[34m"

#include <iostream>

#include "Log.h"
#include "Logger.h"
#include "../helpers/DateHelper.h"
#include "../helpers/LoggerHelper.h"

namespace DreamEngine::Core::Loggers
{

using namespace DreamEngine::Core::Helpers;

class ConsoleLogger final : public Logger
{
   public:
    void Process(const Log log) override
    {
        std::cout 
            << GetColor(log.logLevel)
            << DateHelper::ToString(log.dateTime) 
            << " "
            << LoggerHelper::GetLogLevel(log) 
            << " " 
            << log.message 
            << "\r\n";
    }

   private:
    static std::string GetColor(const LogLevel logLevel)
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
};
}  // namespace DreamEngine::Core::Loggers
#endif