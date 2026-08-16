#ifndef CORE_SERIALIZERS_TEXTURE_SERIALIZER_H_
#define CORE_SERIALIZERS_TEXTURE_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../../Core/Render/Texture.h"

namespace DreamEngine::Core::Serializers
{
using namespace DreamEngine::Core::Render;
class CORE_API TextureSerializer
{
   public:
    static std::string Serialize(const Texture& material);
    static Texture* Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Core::Serializers
#endif