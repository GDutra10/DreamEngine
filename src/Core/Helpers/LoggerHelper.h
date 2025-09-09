#ifndef CORE_HELPERS_LOGGER_HELPER_H
#define CORE_HELPERS_LOGGER_HELPER_H

#include <string>
#include "../Loggers/Log.h"

#include "CoreExport.h"

namespace DreamEngine::Core::Helpers
{
using namespace DreamEngine::Core::Loggers;

class CORE_API LoggerHelper
{
   public:
    static std::string GetLogLevel(const Log& log);
};
}  // namespace DreamEngine::Core::Helpers
#endif