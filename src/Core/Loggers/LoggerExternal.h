#ifndef CORE_LOGGERS_LOGGER_EXTERNAL_H
#define CORE_LOGGERS_LOGGER_EXTERNAL_H

#include "CoreExport.h"
#include <string>

extern "C" CORE_API void coreLogTrace(const std::string& message);

extern "C" CORE_API void coreLogDebug(const std::string& message);

extern "C" CORE_API void coreLogInfo(const std::string& message);

extern "C" CORE_API void coreLogWarning(const std::string& message);

extern "C" CORE_API void coreLogError(const std::string& message);

#endif