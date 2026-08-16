#include "PrefabEntityDefinitionSerializer.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "EntityDefinitionSerializer.h"

#ifndef PREFAB_PROPERTY_RESOURCE_ID
#define PREFAB_PROPERTY_RESOURCE_ID "resourceId"
#endif

#ifndef PREFAB_PROPERTY_ENTITY
#define PREFAB_PROPERTY_ENTITY "entity"
#endif

#ifndef PREFAB_PROPERTY_CHILDREN
#define PREFAB_PROPERTY_CHILDREN "children"
#endif

using namespace DreamEngine::Core::Serializers;
using namespace DreamEngine::Core::GameSystem::Definitions;

using json = nlohmann::json;

json PrefabEntityDefinitionSerializer::Serialize(PrefabEntityDefinition& prefabDefinition)
{
    json prefabJson;

    prefabJson[PREFAB_PROPERTY_RESOURCE_ID] = prefabDefinition.resourceId;
    prefabJson[PREFAB_PROPERTY_ENTITY] = EntityDefinitionSerializer::Serialize(prefabDefinition.entity);

    for (auto& child : prefabDefinition.children)
        prefabJson[PREFAB_PROPERTY_CHILDREN].push_back(Serialize(child));

    return prefabJson;
}

PrefabEntityDefinition PrefabEntityDefinitionSerializer::Deserialize(std::ifstream& stream)
{
    json j;
    stream >> j;

    return Deserialize(j);
}

PrefabEntityDefinition PrefabEntityDefinitionSerializer::Deserialize(const json& jsonPrefab)
{
    PrefabEntityDefinition prefabDefinition{};
    prefabDefinition.entity = EntityDefinitionSerializer::Deserialize(jsonPrefab[PREFAB_PROPERTY_ENTITY]);
    prefabDefinition.children = {};

    if (jsonPrefab.contains(PREFAB_PROPERTY_RESOURCE_ID))
        prefabDefinition.resourceId = jsonPrefab[PREFAB_PROPERTY_RESOURCE_ID];

    if (!jsonPrefab.contains(PREFAB_PROPERTY_CHILDREN))
        return prefabDefinition;

    const json& children = jsonPrefab[PREFAB_PROPERTY_CHILDREN];

    if (!children.is_array())
        return prefabDefinition;

    for (const auto& jsonChildPrefab : children)
        prefabDefinition.children.push_back(PrefabEntityDefinitionSerializer::Deserialize(jsonChildPrefab));

    return prefabDefinition;
}