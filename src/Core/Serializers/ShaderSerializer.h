#ifndef CORE_SERIALIZERS_SHADER_SERIALIZER_H_
#define CORE_SERIALIZERS_SHADER_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../../Core/Render/Shader.h"

namespace DreamEngine::Core::Serializers
{
using namespace DreamEngine::Core::Render;
class CORE_API ShaderSerializer
{
   public:
    static std::string Serialize(const Shader& shader);
    static Shader* Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Core::Serializers
#endif