#ifndef CORE_HELPERS_GUID_HELPER_H
#define CORE_HELPERS_GUID_HELPER_H

#include <string>

namespace DreamEngine::Core::Helpers
{
class GUIDHelper
{
   public:
    static std::string GenerateGUID();
};
}  // namespace DreamEngine::Core::Helpers
#endif