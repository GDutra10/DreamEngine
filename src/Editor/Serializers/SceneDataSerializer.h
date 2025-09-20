#ifndef EDITOR_SERIALIZERS_SCENE_DATA_SERIALIZER_H_
#define EDITOR_SERIALIZERS_SCENE_DATA_SERIALIZER_H_

#include <string>
#include <nlohmann/json.hpp>

#include "../Models/Datas/SceneData.h"

namespace DreamEngine::Editor::Serializers
{
using namespace DreamEngine::Editor::Models::Datas;
using json = nlohmann::json;

class SceneDataSerializer
{
   public:
    static std::string Serialize(SceneData& sceneData);
    static SceneData& Deserialize(std::ifstream& stream);
   private:
    static json Serialize(EntityConfigData& entityConfig);
    static Vector3Data DeserializeVec3(const json& j, const char* key);

};

}  // namespace DreamEngine::Editor::Serializers
#endif