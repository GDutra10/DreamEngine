#ifndef CORE_RESOURCES_RESOURCE_H_
#define CORE_RESOURCES_RESOURCE_H_

#include <string>

#include "CoreExport.h"

namespace DreamEngine::Core::Resources
{
class CORE_API Resource
{
public:
    std::string resourceId;
    std::string name;
};
}  // namespace DreamEngine::Core::Resources
#endif