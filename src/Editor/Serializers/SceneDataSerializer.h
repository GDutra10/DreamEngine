#ifndef EDITOR_SERIALIZERS_SCENE_DATA_SERIALIZER_H_
#define EDITOR_SERIALIZERS_SCENE_DATA_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../Models/SceneData.h"

namespace DreamEngine::Editor::Serializers
{
using namespace DreamEngine::Editor::Models;
class SceneDataSerializer
{
   public:
    static std::string Serialize(SceneData& model);
    static SceneData& Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Editor::Serializers
#endif