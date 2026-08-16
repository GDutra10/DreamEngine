#include "SceneDefinitionSerializer.h"

#include <nlohmann/json.hpp>
#include <fstream>

#include "../GameSystem/Definitions/SceneDefinition.h"
#include "../GameSystem/Definitions/EntityDefinition.h"
#include "../../Core/GameSystem/Camera.h"
#include "EntityDefinitionSerializer.h"


using namespace DreamEngine::Core::Serializers;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::GameSystem::Definitions;
using namespace DreamEngine::Core::ECS::Components;
using json = nlohmann::json;

json SceneDefinitionSerializer::Serialize(SceneDefinition& sceneDefinition)
{
    json j;

    j["background"] = {
        sceneDefinition.backgroundColor.red,
        sceneDefinition.backgroundColor.green,
        sceneDefinition.backgroundColor.blue,
        sceneDefinition.backgroundColor.alpha
    };

    j["globalLight"]["transform"]["position"] = {
        sceneDefinition.globalLight.transform.position.x,
        sceneDefinition.globalLight.transform.position.y,
        sceneDefinition.globalLight.transform.position.z,
    };

    j["globalLight"]["transform"]["rotation"] = {
        sceneDefinition.globalLight.transform.rotation.x,
        sceneDefinition.globalLight.transform.rotation.y,
        sceneDefinition.globalLight.transform.rotation.z,
    };

    j["globalLight"]["transform"]["scale"] = {
        sceneDefinition.globalLight.transform.scale.x,
        sceneDefinition.globalLight.transform.scale.y,
        sceneDefinition.globalLight.transform.scale.z,
    };

    j["globalLight"]["directionalLight"]["influence"] = sceneDefinition.globalLight.directionalLight.influence;
    j["globalLight"]["directionalLight"]["color"] = {
        sceneDefinition.globalLight.directionalLight.color.red,
        sceneDefinition.globalLight.directionalLight.color.green,
        sceneDefinition.globalLight.directionalLight.color.blue,
        sceneDefinition.globalLight.directionalLight.color.alpha,
    };

    j["globalLight"]["directionalLight"]["specular"] = {
        sceneDefinition.globalLight.directionalLight.specular.x,
        sceneDefinition.globalLight.directionalLight.specular.y,
        sceneDefinition.globalLight.directionalLight.specular.z,
    };

    j["mainCameraEntityIdentifier"] = sceneDefinition.mainCameraEntityIdentifier;

    for (auto entity : sceneDefinition.entities)
        j["entities"].push_back(EntityDefinitionSerializer::Serialize(entity));

    return j;
}

SceneDefinition SceneDefinitionSerializer::Deserialize(std::ifstream& stream)
{
    json j;
    stream >> j;

    SceneDefinition* sceneDefinition = new SceneDefinition();
    sceneDefinition->entities = {};
    
    j["background"][0].get_to(sceneDefinition->backgroundColor.red);
    j["background"][1].get_to(sceneDefinition->backgroundColor.green);
    j["background"][2].get_to(sceneDefinition->backgroundColor.blue);
    j["background"][3].get_to(sceneDefinition->backgroundColor.alpha);

    if (j.find("mainCameraEntityIdentifier") != j.end())
        j["mainCameraEntityIdentifier"].get_to(sceneDefinition->mainCameraEntityIdentifier);  

    j["globalLight"]["transform"]["position"][0].get_to(sceneDefinition->globalLight.transform.position.x);
    j["globalLight"]["transform"]["position"][1].get_to(sceneDefinition->globalLight.transform.position.y);
    j["globalLight"]["transform"]["position"][2].get_to(sceneDefinition->globalLight.transform.position.z);
    j["globalLight"]["transform"]["rotation"][0].get_to(sceneDefinition->globalLight.transform.rotation.x);
    j["globalLight"]["transform"]["rotation"][1].get_to(sceneDefinition->globalLight.transform.rotation.y);
    j["globalLight"]["transform"]["rotation"][2].get_to(sceneDefinition->globalLight.transform.rotation.z);
    j["globalLight"]["transform"]["scale"][0].get_to(sceneDefinition->globalLight.transform.scale.x);
    j["globalLight"]["transform"]["scale"][1].get_to(sceneDefinition->globalLight.transform.scale.y);
    j["globalLight"]["transform"]["scale"][2].get_to(sceneDefinition->globalLight.transform.scale.z);

    j["globalLight"]["directionalLight"]["influence"].get_to(sceneDefinition->globalLight.directionalLight.influence);
    j["globalLight"]["directionalLight"]["specular"][0].get_to(sceneDefinition->globalLight.directionalLight.specular.x);
    j["globalLight"]["directionalLight"]["specular"][1].get_to(sceneDefinition->globalLight.directionalLight.specular.y);
    j["globalLight"]["directionalLight"]["specular"][2].get_to(sceneDefinition->globalLight.directionalLight.specular.z);
    j["globalLight"]["directionalLight"]["color"][0].get_to(sceneDefinition->globalLight.directionalLight.color.red);
    j["globalLight"]["directionalLight"]["color"][1].get_to(sceneDefinition->globalLight.directionalLight.color.green);
    j["globalLight"]["directionalLight"]["color"][2].get_to(sceneDefinition->globalLight.directionalLight.color.blue);
    j["globalLight"]["directionalLight"]["color"][3].get_to(sceneDefinition->globalLight.directionalLight.color.alpha);

    if (auto it = j.find("entities"); it == j.end())
        return *sceneDefinition;

    json& entities = j["entities"];

    if (!entities.is_array())
        return *sceneDefinition;

    for (const auto& jsonEntity : entities)
    {
        auto entity = EntityDefinitionSerializer::Deserialize(jsonEntity);
        sceneDefinition->entities.push_back(entity);
    }

    return *sceneDefinition;
}
