#include "EntityDefinitionSerializer.h"

#include <nlohmann/json.hpp>
#include <fstream>

#include "../ECS/Components/TransformComponent.h"
#include "../Helpers/GUIDHelper.h"

using namespace DreamEngine::Core::Serializers;

using json = nlohmann::json;

json EntityDefinitionSerializer::Serialize(EntityDefinition& entityDefinition)
{
    json jsonEntity;

    jsonEntity["name"] = entityDefinition.name;
    jsonEntity["tag"] = entityDefinition.tag;
    jsonEntity["identifier"] = entityDefinition.identifier;
    jsonEntity["isActive"] = entityDefinition.isActive;

    jsonEntity["transform"]["position"] = {
        entityDefinition.transform.position.x,
        entityDefinition.transform.position.y,
        entityDefinition.transform.position.z,
    };

    jsonEntity["transform"]["rotation"] = {
        entityDefinition.transform.rotation.x,
        entityDefinition.transform.rotation.y,
        entityDefinition.transform.rotation.z,
    };

    jsonEntity["transform"]["scale"] = {
        entityDefinition.transform.scale.x,
        entityDefinition.transform.scale.y,
        entityDefinition.transform.scale.z,
    };

    jsonEntity["components"] = {};

    // mesh
    if (!entityDefinition.components.mesh.resourceId.empty())
        jsonEntity["components"]["mesh"]["resourceId"] = entityDefinition.components.mesh.resourceId;

    // material
    if (!entityDefinition.components.material.resourceId.empty())
        jsonEntity["components"]["material"]["resourceId"] = entityDefinition.components.material.resourceId;

    // script
    if (!entityDefinition.components.script.resourceId.empty())
    {
        jsonEntity["components"]["script"]["resourceId"] = entityDefinition.components.script.resourceId;
        jsonEntity["components"]["script"]["className"] = entityDefinition.components.script.className;
        jsonEntity["components"]["script"]["assemblyName"] = entityDefinition.components.script.assemblyName;
    }

    // camera
    if (entityDefinition.components.camera.has)
    {
        jsonEntity["components"]["camera"]["fovDegree"] = entityDefinition.components.camera.fovDegree;
        jsonEntity["components"]["camera"]["near"] = entityDefinition.components.camera.near;
        jsonEntity["components"]["camera"]["far"] = entityDefinition.components.camera.far;
    }

    // ui
    if (entityDefinition.components.ui.has)
    {
        jsonEntity["components"]["ui"]["zOrder"] = entityDefinition.components.ui.zOrder;
        jsonEntity["components"]["ui"]["resourceId"] = entityDefinition.components.ui.resourceId;
    }

    // collider
    if (entityDefinition.components.collider.has)
    {
        jsonEntity["components"]["collider"]["enabled"] = entityDefinition.components.collider.enabled;
        jsonEntity["components"]["collider"]["isTrigger"] = entityDefinition.components.collider.isTrigger;
        jsonEntity["components"]["collider"]["center"] = {
            entityDefinition.components.collider.center.x,
            entityDefinition.components.collider.center.y,
            entityDefinition.components.collider.center.z,
        };
        jsonEntity["components"]["collider"]["size"] = {
            entityDefinition.components.collider.size.x,
            entityDefinition.components.collider.size.y,
            entityDefinition.components.collider.size.z,
        };
    }

    // children
    if (!entityDefinition.components.children.childIdentifiers.empty())
    {
        for (auto childIdentifier : entityDefinition.components.children.childIdentifiers)
            jsonEntity["components"]["children"].push_back(childIdentifier);
    }

    // parent
    if (!entityDefinition.components.parent.parentIdentifier.empty())
        jsonEntity["components"]["parent"]["parentIdentifier"] = entityDefinition.components.parent.parentIdentifier;

    return jsonEntity;
}

EntityDefinition EntityDefinitionSerializer::Deserialize(std::ifstream& stream)
{
    json jsonEntity;
    stream >> jsonEntity;

    return Deserialize(jsonEntity);
}

EntityDefinition EntityDefinitionSerializer::Deserialize(const json& jsonEntity)
{
    EntityDefinition entityDefinition{};

    entityDefinition.identifier = jsonEntity.value("identifier", Helpers::GUIDHelper::GenerateGUID());
    entityDefinition.name = jsonEntity.value("name", std::string{});
    entityDefinition.tag = jsonEntity.value("tag", std::string{});
    entityDefinition.isActive = jsonEntity.value("isActive", false);

    if (auto it = jsonEntity.find("transform"); it != jsonEntity.end() && it->is_object())
    {
        const json& t = *it;
        entityDefinition.transform.position = DeserializeVec3(t, "position");
        entityDefinition.transform.rotation = DeserializeVec3(t, "rotation");
        entityDefinition.transform.scale = DeserializeVec3(t, "scale");
    }

    if (auto it = jsonEntity.find("components"); it != jsonEntity.end())
    {
        json jsonComponent = *it;

        if (jsonComponent.find("mesh") != jsonComponent.end())
            jsonComponent["mesh"]["resourceId"].get_to(entityDefinition.components.mesh.resourceId);

        if (jsonComponent.find("material") != jsonComponent.end())
            jsonComponent["material"]["resourceId"].get_to(entityDefinition.components.material.resourceId);

        if (jsonComponent.find("script") != jsonComponent.end())
        {
            jsonComponent["script"]["resourceId"].get_to(entityDefinition.components.script.resourceId);
            jsonComponent["script"]["className"].get_to(entityDefinition.components.script.className);
            jsonComponent["script"]["assemblyName"].get_to(entityDefinition.components.script.assemblyName);
        }

        if (jsonComponent.find("parent") != jsonComponent.end())
            jsonComponent["parent"]["parentIdentifier"].get_to(entityDefinition.components.parent.parentIdentifier);

        if (jsonComponent.find("children") != jsonComponent.end() && jsonComponent["children"].is_array())
            jsonComponent["children"].get_to(entityDefinition.components.children.childIdentifiers);

        if (jsonComponent.find("camera") != jsonComponent.end())
        {
            entityDefinition.components.camera.has = true;
            jsonComponent["camera"]["fovDegree"].get_to(entityDefinition.components.camera.fovDegree);
            jsonComponent["camera"]["near"].get_to(entityDefinition.components.camera.near);
            jsonComponent["camera"]["far"].get_to(entityDefinition.components.camera.far);
        }

        if (jsonComponent.find("ui") != jsonComponent.end())
        {
            entityDefinition.components.ui.has = true;
            jsonComponent["ui"]["zOrder"].get_to(entityDefinition.components.ui.zOrder);
            jsonComponent["ui"]["resourceId"].get_to(entityDefinition.components.ui.resourceId);
        }

        if (auto itCollider = jsonComponent.find("collider"); itCollider != jsonComponent.end())
        {
            const json& t = *itCollider;

            entityDefinition.components.collider.has = true;
            jsonComponent["collider"]["enabled"].get_to(entityDefinition.components.collider.enabled);
            jsonComponent["collider"]["isTrigger"].get_to(entityDefinition.components.collider.isTrigger);
            entityDefinition.components.collider.center = DeserializeVec3(t, "center");
            entityDefinition.components.collider.size = DeserializeVec3(t, "size");
        }
    }

    return entityDefinition;
}

Vector3Definition EntityDefinitionSerializer::DeserializeVec3(const json& j, const char* key)
{
    auto it = j.find(key);

    if (it == j.end())
        return {};

    if (it->is_array() && it->size() == 3)
    {
        Vector3Definition v;
        v.x = (*it)[0].get<float>();
        v.y = (*it)[1].get<float>();
        v.z = (*it)[2].get<float>();
        return v;
    }

    return {};
}
