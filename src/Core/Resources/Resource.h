#ifndef CORE_RESOURCES_RESOURCE_H_
#define CORE_RESOURCES_RESOURCE_H_

#include <string>

namespace DreamEngine::Core::Resources
{
class Resource
{
public:
    std::string resourceId;
    std::string name;
};
}  // namespace DreamEngine::Core::Resources
#endif