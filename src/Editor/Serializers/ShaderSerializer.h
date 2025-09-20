#ifndef EDITOR_SERIALIZERS_SHADER_SERIALIZER_H_
#define EDITOR_SERIALIZERS_SHADER_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../../Core/Render/Shader.h"

namespace DreamEngine::Editor::Serializers
{
using namespace DreamEngine::Core::Render;
class ShaderSerializer
{
   public:
    static std::string Serialize(const Shader& shader);
    static Shader* Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Editor::Serializers
#endif