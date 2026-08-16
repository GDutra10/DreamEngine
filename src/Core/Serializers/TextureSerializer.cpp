#include "TextureSerializer.h"

using namespace DreamEngine::Core::Serializers;

std::string TextureSerializer::Serialize(const Texture& material)
{
    return std::string();
}

Texture* TextureSerializer::Deserialize(std::ifstream& stream)
{
    return nullptr;
}
