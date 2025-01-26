#ifndef EDITOR_SERIALIZERS_MATERIAL_SERIALIZER_H_
#define EDITOR_SERIALIZERS_MATERIAL_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../../Core/Render/Material.h"

namespace DreamEngine::Editor::Serializers
{
using namespace DreamEngine::Core::Render;
class MaterialSerializer
{
   public:
    static std::string Serialize(const Material& material);
    static Material* Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Editor::Serializers
#endif