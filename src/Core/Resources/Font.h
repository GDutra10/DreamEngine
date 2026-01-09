#ifndef CORE_RESOURCES_FONT_H_
#define CORE_RESOURCES_FONT_H_

#include "../Resources/Resource.h"

namespace DreamEngine::Core::Resources
{
using namespace DreamEngine::Core::Resources;
class CORE_API Font : public Resource
{
   public:
    std::vector<unsigned char> data;
    std::string path;

    Font(std::vector<unsigned char> data) : data(data) {};

    virtual ~Font() = default;
};
}  // namespace DreamEngine::Core::Render
#endif