#ifndef CORE_RENDER_TEXTURE_H_
#define CORE_RENDER_TEXTURE_H_

#include "TextureType.h"
#include "../Resources/Resource.h"

namespace DreamEngine::Core::Render
{
using namespace DreamEngine::Core::Resources;
class Texture : public Resource
{
   public:
    unsigned int id = 0;
    unsigned char* data;
    TextureType type;
    std::string path;

    Texture(unsigned char* data) : data(data) {}; 

    virtual ~Texture() = default;
};
}  // namespace DreamEngine::Core::Render
#endif