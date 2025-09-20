#include "SceneController.h"

#include <fstream>
#include <regex>
#include <ranges>

#include "../Singletons/EditorSingleton.h"
#include "../../Core/Application.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../Serializers/SceneDataSerializer.h"
#include "ECS/Components/ChildrenComponent.h"
#include "ECS/Components/MaterialComponent.h"
#include "ECS/Components/ParentComponent.h"
#include "ECS/Components/ScriptComponent.h"
#include "Resources/GlobalResourceManager.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Singletons;

bool SceneController::ShouldLoadSceneData(path& path)
{
    // Check if path is scene
    return path != EditorSingleton::Instance().GetSelectedScenePath();
}

void SceneController::LoadSceneData(path& path, EntityManager* entityManager)
{
    entityManager->Reset();
    std::ifstream file(path.string());

    SceneData* sceneData = &Serializers::SceneDataSerializer::Deserialize(file);
    sceneData->path = path;

    EditorSingleton::Instance().SetSelectedEntity(nullptr);
    EditorSingleton::Instance().SetIsViewSceneData(false);
    EditorSingleton::Instance().sceneData = sceneData;
    Scene* scene = Application::Instance().GetGame()->GetActiveScene();
    Camera& camera = scene->GetCamera();

     // set camera props
    camera.fovDegree = sceneData->camera.fovDegree;
    camera.far = sceneData->camera.far;
    camera.front = sceneData->camera.front;
    camera.front.z = -1;
    camera.near = sceneData->camera.near;
    camera.position = sceneData->camera.position;
    camera.up.x = sceneData->camera.up.x;
    camera.up.y = sceneData->camera.up.y;
    camera.up.z = sceneData->camera.up.z;

    Color* backgroundColor = scene->GetBackgroundColor();
    backgroundColor->blue = sceneData->backgroundColor.blue;
    backgroundColor->red = sceneData->backgroundColor.red;
    backgroundColor->green = sceneData->backgroundColor.green;
    backgroundColor->alpha = sceneData->backgroundColor.alpha;

    GlobalLight* globalLight = scene->GetGlobalLight();

    globalLight->transform.SetPosition({
        sceneData->globalLight.transform.position.x,
        sceneData->globalLight.transform.position.y,
        sceneData->globalLight.transform.position.z,
    });

    globalLight->transform.SetScale({
        sceneData->globalLight.transform.scale.x,
        sceneData->globalLight.transform.scale.y,
        sceneData->globalLight.transform.scale.z,
    });

    globalLight->transform.SetRotation({
        sceneData->globalLight.transform.rotation.x,
        sceneData->globalLight.transform.rotation.y,
        sceneData->globalLight.transform.rotation.z,
    });

    globalLight->directionalLight.color.red = sceneData->globalLight.directionalLight.color.red;
    globalLight->directionalLight.color.green = sceneData->globalLight.directionalLight.color.green;
    globalLight->directionalLight.color.blue = sceneData->globalLight.directionalLight.color.blue;
    globalLight->directionalLight.color.alpha = sceneData->globalLight.directionalLight.color.alpha;
    globalLight->directionalLight.specular.x = sceneData->globalLight.directionalLight.specular.x;
    globalLight->directionalLight.specular.y = sceneData->globalLight.directionalLight.specular.y;
    globalLight->directionalLight.specular.z = sceneData->globalLight.directionalLight.specular.z;
    globalLight->directionalLight.influence = sceneData->globalLight.directionalLight.influence;

    // create entities
    vector<Entity*> entities;

    for (auto& entityConfig : sceneData->entities)
    {
        Entity* entity = entityManager->AddEntity(entityConfig.tag);
        entity->SetActive(entityConfig.isActive);
        entity->GetName() = entityConfig.name;
        entity->SetIdentifier(entityConfig.identifier);

        TransformComponent& transform = entity->GetComponent<TransformComponent>();
        transform.SetPosition({
            entityConfig.transform.position.x,
            entityConfig.transform.position.y,
            entityConfig.transform.position.z
        });

        transform.SetScale({
            entityConfig.transform.scale.x,
            entityConfig.transform.scale.y,
            entityConfig.transform.scale.z
        });

        transform.SetRotation({
            entityConfig.transform.rotation.x,
            entityConfig.transform.rotation.y,
            entityConfig.transform.rotation.z
        });

        if (!entityConfig.components.mesh.resourceId.empty())
        {
            MeshComponent& meshComponent = entity->GetComponent<MeshComponent>();
            meshComponent.mesh = GlobalResourceManager::Instance().GetMesh(entityConfig.components.mesh.resourceId);
            meshComponent.has = true;
        }

        if (!entityConfig.components.material.resourceId.empty())
        {
            MaterialComponent& materialComponent = entity->GetComponent<MaterialComponent>();
            materialComponent.material = GlobalResourceManager::Instance().GetMaterial(entityConfig.components.material.resourceId);
            materialComponent.has = true;
        }

        if (!entityConfig.components.script.resourceId.empty())
        {
            ScriptComponent& scriptComponent = entity->GetComponent<ScriptComponent>();
            scriptComponent.script = GlobalResourceManager::Instance().GetScript(entityConfig.components.script.resourceId);
            scriptComponent.has = true;
        }

        entities.push_back(entity);
    }

    // set parents and childrens
    for (auto& entityConfig : sceneData->entities)
    {
        const vector<std::string>& childIds = entityConfig.components.children.childIdentifiers;
        const std::string& parentId = entityConfig.components.parent.parentIdentifier;

        auto itEnt = std::ranges::find_if(entities, [&](Entity* e)
        {
            return e->GetIdentifier() == entityConfig.identifier;
        });

        if (itEnt == entities.end())
            continue;

        Entity* entity = *itEnt;

        // children
        if (!childIds.empty())
        {
            auto& childrenComponent = entity->GetComponent<ChildrenComponent>();
            childrenComponent.has = true;

            // filter all entities whose id is in childIds
            auto children_view = entities | std::views::filter([&](Entity* e)
            {
                return std::ranges::find(childIds, e->GetIdentifier()) != childIds.end();
            });

            for (Entity* child : children_view)
            {
                // skip self / duplicates
                    //&& std::ranges::find(childrenComponent.children, child) != childrenComponent.children.end())
                if (child->GetIdentifier() != entity->GetIdentifier())
                    childrenComponent.children.push_back(child);
            }
        }

        // parent
        if (!parentId.empty())
        {
            auto itParent = std::ranges::find_if(entities, [&](Entity* e)
            {
                return e->GetIdentifier() == parentId;
            });

            auto& parentComponent = entity->GetComponent<ParentComponent>();
            parentComponent.parent = (itParent != entities.end()) ? *itParent : nullptr;
            parentComponent.has = true;
        }
    }
}

void SceneController::SaveSceneData(EntityManager* entityManager)
{
    SceneData* sceneData = EditorSingleton::Instance().sceneData;

    if (sceneData == nullptr)
    {
        LoggerSingleton::Instance().LogWarning("There is no scene opened!");
        return;
    }

    Scene* scene = Application::Instance().GetGame()->GetActiveScene();
    Camera& camera = scene->GetCamera();
    sceneData->camera.fovDegree = camera.fovDegree;
    sceneData->camera.far = camera.far;
    sceneData->camera.front = camera.front;
    sceneData->camera.near = camera.near;
    sceneData->camera.position = camera.position;
    sceneData->camera.up.x = camera.up.x;
    sceneData->camera.up.y = camera.up.y;
    sceneData->camera.up.z = camera.up.z;


    Color* backgroundColor = scene->GetBackgroundColor();
    sceneData->backgroundColor.alpha = backgroundColor->alpha;
    sceneData->backgroundColor.blue = backgroundColor->blue;
    sceneData->backgroundColor.green = backgroundColor->green;
    sceneData->backgroundColor.red = backgroundColor->red;

    GlobalLight* globalLight = scene->GetGlobalLight();
    glm::vec3 globalLightPosition = globalLight->transform.GetPosition();
    glm::vec3 globalLightRotation = globalLight->transform.GetRotation();
    glm::vec3 globalLightScale = globalLight->transform.GetScale();

    sceneData->globalLight.transform.position.x = globalLightPosition.x;
    sceneData->globalLight.transform.position.y = globalLightPosition.y;
    sceneData->globalLight.transform.position.z = globalLightPosition.z;

    sceneData->globalLight.transform.rotation.x = globalLightRotation.x;
    sceneData->globalLight.transform.rotation.y = globalLightRotation.y;
    sceneData->globalLight.transform.rotation.z = globalLightRotation.z;

    sceneData->globalLight.transform.scale.x = globalLightScale.x;
    sceneData->globalLight.transform.scale.y = globalLightScale.y;
    sceneData->globalLight.transform.scale.z = globalLightScale.z;

    sceneData->globalLight.directionalLight.color.blue = globalLight->directionalLight.color.blue;
    sceneData->globalLight.directionalLight.color.green = globalLight->directionalLight.color.green;
    sceneData->globalLight.directionalLight.color.red = globalLight->directionalLight.color.red;
    sceneData->globalLight.directionalLight.color.alpha = globalLight->directionalLight.color.alpha;

    sceneData->globalLight.directionalLight.influence = globalLight->directionalLight.influence;
    sceneData->globalLight.directionalLight.specular.x = globalLight->directionalLight.specular.x;
    sceneData->globalLight.directionalLight.specular.y = globalLight->directionalLight.specular.y;
    sceneData->globalLight.directionalLight.specular.z = globalLight->directionalLight.specular.z;

    sceneData->entities.clear();

    for (Entity* entity : entityManager->GetEntities())
    {
        EntityConfigData entityConfig;
        TransformComponent& transform = entity->GetComponent<TransformComponent>();
        glm::vec3 position = transform.GetPosition();
        glm::vec3 scale = transform.GetScale();
        glm::vec3 rotation = transform.GetRotation();

        entityConfig.identifier = entity->GetIdentifier();
        entityConfig.name = entity->GetName();
        entityConfig.tag = entity->GetTag();
        entityConfig.isActive = entity->GetIsActive();

        entityConfig.transform.position.x = position.x;
        entityConfig.transform.position.y = position.y;
        entityConfig.transform.position.z = position.z;

        entityConfig.transform.scale.x = scale.x;
        entityConfig.transform.scale.y = scale.y;
        entityConfig.transform.scale.z = scale.z;

        entityConfig.transform.rotation.x = rotation.x;
        entityConfig.transform.rotation.y = rotation.y;
        entityConfig.transform.rotation.z = rotation.z;

        if (const MeshComponent& mesh = entity->GetComponent<MeshComponent>(); mesh.has && mesh.mesh != nullptr)
            entityConfig.components.mesh.resourceId = mesh.mesh->resourceId;

        if (const MaterialComponent& material = entity->GetComponent<MaterialComponent>(); material.has && material.material != nullptr)
            entityConfig.components.material.resourceId = material.material->resourceId;

        if (const ScriptComponent& script = entity->GetComponent<ScriptComponent>(); script.has && script.script != nullptr)
            entityConfig.components.script.resourceId = script.script->resourceId;

        if (const ChildrenComponent& children = entity->GetComponent<ChildrenComponent>(); children.has && !children.children.empty())
        {
            for (Entity* child : children.children)
                entityConfig.components.children.childIdentifiers.push_back(child->GetIdentifier());
        }

        if (const ParentComponent& parent = entity->GetComponent<ParentComponent>(); parent.has && parent.parent != nullptr)
            entityConfig.components.parent.parentIdentifier = parent.parent->GetIdentifier();

        sceneData->entities.push_back(entityConfig);
    }

    if (std::ofstream file(sceneData->path); file.is_open())
    {
        file << Serializers::SceneDataSerializer::Serialize(*sceneData);
        file.close();

        LoggerSingleton::Instance().LogInfo("Scene '" + sceneData->path.string() + "' saved successfully");
    }
    else
        LoggerSingleton::Instance().LogError("Failed to open the scene file");
}