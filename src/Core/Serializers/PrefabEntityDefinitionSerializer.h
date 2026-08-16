#ifndef CORE_SERIALIZERS_PREFAB_ENTITY_DATA_SERIALIZER_H_
#define CORE_SERIALIZERS_PREFAB_ENTITY_DATA_SERIALIZER_H_

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../GameSystem/Definitions/PrefabEntityDefinition.h"
#include "../GameSystem/Definitions/EntityDefinition.h"

namespace DreamEngine::Core::Serializers
{
using namespace DreamEngine::Core::GameSystem::Definitions;
using json = nlohmann::json;

class CORE_API PrefabEntityDefinitionSerializer
{
   public:
    static json Serialize(PrefabEntityDefinition& prefabDefinition);
    static PrefabEntityDefinition Deserialize(std::ifstream& stream);
   private:
    static PrefabEntityDefinition Deserialize(const json& jsonPrefab);
};
}  // namespace DreamEngine::Core::Serializers
#endif