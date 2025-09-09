#ifndef CORE_HELPERS_DATE_HELPER_H
#define CORE_HELPERS_DATE_HELPER_H

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "CoreExport.h"

namespace DreamEngine::Core::Helpers
{
class CORE_API DateHelper{
   public:
    static std::string ToString(const std::chrono::time_point<std::chrono::system_clock> timePoint);
};
}  // namespace DreamEngine::Core::Loggers
#endif