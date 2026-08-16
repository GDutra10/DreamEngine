#ifndef CORE_SERIALIZERS_ENTITY_DEFITION_SERIALIZER_H_
#define CORE_SERIALIZERS_ENTITY_DEFITION_SERIALIZER_H_

#include <nlohmann/json.hpp>
#include <string>

#include "../GameSystem/Definitions/EntityDefinition.h"
#include "../GameSystem/Definitions/Vector3Definition.h"

namespace DreamEngine::Core::Serializers
{
using namespace DreamEngine::Core::GameSystem::Definitions;
using json = nlohmann::json;

class CORE_API EntityDefinitionSerializer
{
   public:
    static json Serialize(EntityDefinition& entityDefinition);
    static EntityDefinition Deserialize(std::ifstream& stream);
    static EntityDefinition Deserialize(const json& jsonEntity);

   private:
    static Vector3Definition DeserializeVec3(const json& j, const char* key);
};

}  // namespace DreamEngine::Core::Serializers
#endif