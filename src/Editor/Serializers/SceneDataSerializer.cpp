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

    json jsonConfigCamera;
    j["camera"]["far"] = sceneData.camera.far;
    j["camera"]["fovDegree"] = sceneData.camera.fovDegree;
    j["camera"]["near"] = sceneData.camera.near;
    j["camera"]["front"] = {
        sceneData.camera.front.x,
        sceneData.camera.front.y,
        sceneData.camera.front.z,
    };
    j["camera"]["position"] = {
        sceneData.camera.position.x,
        sceneData.camera.position.y,
        sceneData.camera.position.z,
    };
    ;
    j["camera"]["right"] = {
        sceneData.camera.right.x,
        sceneData.camera.right.y,
        sceneData.camera.right.z,
    };
    
    j["camera"]["up"] = {
        sceneData.camera.up.x,
        sceneData.camera.up.y,
        sceneData.camera.up.z,
    };
    ;
    j["camera"]["worldUp"] = {
        sceneData.camera.worldUp.x,
        sceneData.camera.worldUp.y,
        sceneData.camera.worldUp.z,
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

    j["camera"].at("far").get_to(sceneData->camera.far);
    j["camera"].at("fovDegree").get_to(sceneData->camera.fovDegree);
    j["camera"].at("near").get_to(sceneData->camera.near);
    j["camera"]["front"][0].get_to(sceneData->camera.front.x);
    j["camera"]["front"][1].get_to(sceneData->camera.front.y);
    j["camera"]["front"][2].get_to(sceneData->camera.front.z);
    j["camera"]["position"][0].get_to(sceneData->camera.position.x);
    j["camera"]["position"][1].get_to(sceneData->camera.position.y);
    j["camera"]["position"][2].get_to(sceneData->camera.position.z);
    j["camera"]["right"][0].get_to(sceneData->camera.right.x);
    j["camera"]["right"][1].get_to(sceneData->camera.right.y);
    j["camera"]["right"][2].get_to(sceneData->camera.right.z);
    j["camera"]["up"][0].get_to(sceneData->camera.up.x);
    j["camera"]["up"][1].get_to(sceneData->camera.up.y);
    j["camera"]["up"][2].get_to(sceneData->camera.up.z);
    j["camera"]["worldUp"][0].get_to(sceneData->camera.worldUp.x);
    j["camera"]["worldUp"][1].get_to(sceneData->camera.worldUp.y);
    j["camera"]["worldUp"][2].get_to(sceneData->camera.worldUp.z);

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
