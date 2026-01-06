#include "SceneDataSerializer.h"

#include <nlohmann/json.hpp>
#include <fstream>

#include "../Models/Datas/SceneData.h"
#include "../Models/Datas/EntityConfigData.h"
#include "../../Core/GameSystem/Camera.h"
#include "../../Core/ECS/Components/TransformComponent.h"

using namespace DreamEngine::Editor::Serializers;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::Models::Datas;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::ECS::Components;
using json = nlohmann::json;

std::string SceneDataSerializer::Serialize(SceneData& sceneData)
{
    json j;

    j["background"] = {
        sceneData.backgroundColor.red,
        sceneData.backgroundColor.green,
        sceneData.backgroundColor.blue,
        sceneData.backgroundColor.alpha
    };

    j["globalLight"]["transform"]["position"] = {
        sceneData.globalLight.transform.position.x,
        sceneData.globalLight.transform.position.y,
        sceneData.globalLight.transform.position.z,
    };

    j["globalLight"]["transform"]["rotation"] = {
        sceneData.globalLight.transform.rotation.x,
        sceneData.globalLight.transform.rotation.y,
        sceneData.globalLight.transform.rotation.z,
    };

    j["globalLight"]["transform"]["scale"] = {
        sceneData.globalLight.transform.scale.x,
        sceneData.globalLight.transform.scale.y,
        sceneData.globalLight.transform.scale.z,
    };

    j["globalLight"]["directionalLight"]["influence"] = sceneData.globalLight.directionalLight.influence;
    j["globalLight"]["directionalLight"]["color"] = {
        sceneData.globalLight.directionalLight.color.red,
        sceneData.globalLight.directionalLight.color.green,
        sceneData.globalLight.directionalLight.color.blue,
        sceneData.globalLight.directionalLight.color.alpha,
    };

    j["globalLight"]["directionalLight"]["specular"] = {
        sceneData.globalLight.directionalLight.specular.x,
        sceneData.globalLight.directionalLight.specular.y,
        sceneData.globalLight.directionalLight.specular.z,
    };

    j["mainCameraEntityIdentifier"] = sceneData.mainCameraEntityIdentifier;

    for (auto entity : sceneData.entities)
        j["entities"].push_back(Serialize(entity));

    return j.dump(4);
}

SceneData& SceneDataSerializer::Deserialize(std::ifstream& stream)
{
    json j;
    stream >> j;

    SceneData* sceneData = new SceneData();
    
    j["background"][0].get_to(sceneData->backgroundColor.red);
    j["background"][1].get_to(sceneData->backgroundColor.green);
    j["background"][2].get_to(sceneData->backgroundColor.blue);
    j["background"][3].get_to(sceneData->backgroundColor.alpha);

    if (j.find("mainCameraEntityIdentifier") != j.end())
        j["mainCameraEntityIdentifier"].get_to(sceneData->mainCameraEntityIdentifier);  

    j["globalLight"]["transform"]["position"][0].get_to(sceneData->globalLight.transform.position.x);
    j["globalLight"]["transform"]["position"][1].get_to(sceneData->globalLight.transform.position.y);
    j["globalLight"]["transform"]["position"][2].get_to(sceneData->globalLight.transform.position.z);
    j["globalLight"]["transform"]["rotation"][0].get_to(sceneData->globalLight.transform.rotation.x);
    j["globalLight"]["transform"]["rotation"][1].get_to(sceneData->globalLight.transform.rotation.y);
    j["globalLight"]["transform"]["rotation"][2].get_to(sceneData->globalLight.transform.rotation.z);
    j["globalLight"]["transform"]["scale"][0].get_to(sceneData->globalLight.transform.scale.x);
    j["globalLight"]["transform"]["scale"][1].get_to(sceneData->globalLight.transform.scale.y);
    j["globalLight"]["transform"]["scale"][2].get_to(sceneData->globalLight.transform.scale.z);

    j["globalLight"]["directionalLight"]["influence"].get_to(sceneData->globalLight.directionalLight.influence);
    j["globalLight"]["directionalLight"]["specular"][0].get_to(sceneData->globalLight.directionalLight.specular.x);
    j["globalLight"]["directionalLight"]["specular"][1].get_to(sceneData->globalLight.directionalLight.specular.y);
    j["globalLight"]["directionalLight"]["specular"][2].get_to(sceneData->globalLight.directionalLight.specular.z);
    j["globalLight"]["directionalLight"]["color"][0].get_to(sceneData->globalLight.directionalLight.color.red);
    j["globalLight"]["directionalLight"]["color"][1].get_to(sceneData->globalLight.directionalLight.color.green);
    j["globalLight"]["directionalLight"]["color"][2].get_to(sceneData->globalLight.directionalLight.color.blue);
    j["globalLight"]["directionalLight"]["color"][3].get_to(sceneData->globalLight.directionalLight.color.alpha);

    if (auto it = j.find("entities"); it == j.end())
        return *sceneData;

    json& entities = j["entities"];

    if (!entities.is_array())
        return *sceneData;

    for (const auto& jsonEntity : entities)
    {
        EntityConfigData e{};

        e.identifier = jsonEntity.value("identifier", Core::Helpers::GUIDHelper::GenerateGUID());
        e.name = jsonEntity.value("name", std::string{});
        e.tag = jsonEntity.value("tag", std::string{});
        e.isActive = jsonEntity.value("isActive", false);

        if (auto it = jsonEntity.find("transform"); it != jsonEntity.end() && it->is_object())
        {
            const json& t = *it;
            e.transform.position = DeserializeVec3(t, "position");
            e.transform.rotation = DeserializeVec3(t, "rotation");
            e.transform.scale = DeserializeVec3(t, "scale");
        }

        if (auto it = jsonEntity.find("components"); it != jsonEntity.end())
        {
            json jsonComponent = *it;

            if (jsonComponent.find("mesh") != jsonComponent.end())
                jsonComponent["mesh"]["resourceId"].get_to(e.components.mesh.resourceId);

            if (jsonComponent.find("material") != jsonComponent.end())
                jsonComponent["material"]["resourceId"].get_to(e.components.material.resourceId);

            if (jsonComponent.find("script") != jsonComponent.end())
                jsonComponent["script"]["resourceId"].get_to(e.components.script.resourceId);

            if (jsonComponent.find("parent") != jsonComponent.end())
                jsonComponent["parent"]["parentIdentifier"].get_to(e.components.parent.parentIdentifier);

            if (jsonComponent.find("children") != jsonComponent.end() && jsonComponent["children"].is_array())
                jsonComponent["children"].get_to(e.components.children.childIdentifiers);

            if (jsonComponent.find("camera") != jsonComponent.end())
            {
                e.components.camera.has = true;
                jsonComponent["camera"]["fovDegree"].get_to(e.components.camera.fovDegree);
                jsonComponent["camera"]["near"].get_to(e.components.camera.near);
                jsonComponent["camera"]["far"].get_to(e.components.camera.far);
            }
        }

        sceneData->entities.push_back(std::move(e));
    }

    return *sceneData;
}

json SceneDataSerializer::Serialize(EntityConfigData& entityConfig)
{
    json jsonEntity;

    jsonEntity["name"] = entityConfig.name;
    jsonEntity["tag"] = entityConfig.tag;
    jsonEntity["identifier"] = entityConfig.identifier;
    jsonEntity["isActive"] = entityConfig.isActive;

    jsonEntity["transform"]["position"] = {
        entityConfig.transform.position.x,
        entityConfig.transform.position.y,
        entityConfig.transform.position.z,
    };

    jsonEntity["transform"]["rotation"] = {
        entityConfig.transform.rotation.x,
        entityConfig.transform.rotation.y,
        entityConfig.transform.rotation.z,
    };

    jsonEntity["transform"]["scale"] = {
        entityConfig.transform.scale.x,
        entityConfig.transform.scale.y,
        entityConfig.transform.scale.z,
    };


    jsonEntity["components"] = {};

    // mesh
    if (!entityConfig.components.mesh.resourceId.empty())
        jsonEntity["components"]["mesh"]["resourceId"] = entityConfig.components.mesh.resourceId;

    // material
    if (!entityConfig.components.material.resourceId.empty())
        jsonEntity["components"]["material"]["resourceId"] = entityConfig.components.material.resourceId;

    // script
    if (!entityConfig.components.script.resourceId.empty())
        jsonEntity["components"]["script"]["resourceId"] = entityConfig.components.script.resourceId;

    // camera
    if (entityConfig.components.camera.has)
    {
        jsonEntity["components"]["camera"]["fovDegree"] = entityConfig.components.camera.fovDegree;
        jsonEntity["components"]["camera"]["near"] = entityConfig.components.camera.near;
        jsonEntity["components"]["camera"]["far"] = entityConfig.components.camera.far;
    }

    // children
    if (!entityConfig.components.children.childIdentifiers.empty())
    {
        for (auto childIdentifier : entityConfig.components.children.childIdentifiers)
            jsonEntity["components"]["children"].push_back(childIdentifier);    
    }

    // parent
    if (!entityConfig.components.parent.parentIdentifier.empty())
        jsonEntity["components"]["parent"]["parentIdentifier"] = entityConfig.components.parent.parentIdentifier;

    return jsonEntity;
}

Vector3Data SceneDataSerializer::DeserializeVec3(const json& j, const char* key)
{
    auto it = j.find(key);

    if (it == j.end())
        return {};

    if (it->is_array() && it->size() == 3)
    {
        Vector3Data v;
        v.x = (*it)[0].get<float>();
        v.y = (*it)[1].get<float>();
        v.z = (*it)[2].get<float>();
        return v;
    }

    return {};
}
