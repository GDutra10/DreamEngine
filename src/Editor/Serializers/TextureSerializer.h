#ifndef EDITOR_SERIALIZERS_TEXTURE_SERIALIZER_H_
#define EDITOR_SERIALIZERS_TEXTURE_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../../Core/Render/Texture.h"

namespace DreamEngine::Editor::Serializers
{
using namespace DreamEngine::Core::Render;
class TextureSerializer
{
   public:
    static std::string Serialize(const Texture& material);
    static Texture* Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Editor::Serializers
#endif