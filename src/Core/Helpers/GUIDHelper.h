#ifndef CORE_HELPERS_GUID_HELPER_H
#define CORE_HELPERS_GUID_HELPER_H

#include <string>
#include "CoreExport.h"

namespace DreamEngine::Core::Helpers
{
class CORE_API GUIDHelper
{
   public:
    static std::string GenerateGUID();
};
}  // namespace DreamEngine::Core::Helpers
#endif