#ifndef CORE_SERIALIZERS_SCENE_DEFITION_SERIALIZER_H_
#define CORE_SERIALIZERS_SCENE_DEFITION_SERIALIZER_H_

#include <string>
#include <nlohmann/json.hpp>

#include "../GameSystem/Definitions/SceneDefinition.h"

namespace DreamEngine::Core::Serializers
{
using namespace DreamEngine::Core::Serializers;
using namespace DreamEngine::Core::GameSystem::Definitions;
using json = nlohmann::json;

class CORE_API SceneDefinitionSerializer
{
   public:
    static json Serialize(SceneDefinition& sceneDefition);
    static SceneDefinition Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Core::Serializers
#endif