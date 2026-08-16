#ifndef CORE_SERIALIZERS_MATERIAL_SERIALIZER_H_
#define CORE_SERIALIZERS_MATERIAL_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../../Core/Render/Material.h"

namespace DreamEngine::Core::Serializers
{
using namespace DreamEngine::Core::Render;
class CORE_API MaterialSerializer
{
   public:
    static std::string Serialize(const Material& material);
    static Material* Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Core::Serializers
#endif