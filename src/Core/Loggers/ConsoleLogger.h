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

#include "CoreExport.h"

namespace DreamEngine::Core::Loggers
{

using namespace DreamEngine::Core::Helpers;

class CORE_API ConsoleLogger final : public Logger
{
   public:
    void Process(const Log log) override;

   private:
    static std::string GetColor(const LogLevel logLevel);
};
}  // namespace DreamEngine::Core::Loggers
#endif