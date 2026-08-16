#include "SceneController.h"

#include <fstream>
#include <regex>
#include <ranges>

#include "../../Core/Application.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../../Core/Serializers/SceneDefinitionSerializer.h"
#include "../../Core/GameSystem/Definitions/SceneDefinition.h"
#include "ECS/Components/ChildrenComponent.h"
#include "ECS/Components/MaterialComponent.h"
#include "ECS/Components/ParentComponent.h"
#include "ECS/Components/ScriptComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/UiComponent.h"
#include "Resources/ResourceManager.h"
#include "../EditorContext.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::GameSystem::Definitions;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::Serializers;
using namespace DreamEngine::Editor::Controllers;

SceneController::SceneController(EditorContext& editorContext) 
    : m_editorContext(editorContext)
    , m_pOriginalSceneData(nullptr){}

bool SceneController::ShouldLoadSceneData(path& path)
{
    // Check if path is scene
    return path != m_editorContext.GetSelectedScenePath();
}

void SceneController::LoadSceneData(path& path, bool isByChangeScene)
{
    LoggerSingleton::Instance().LogTrace("SceneController::LoadSceneData -> Start");

    std::ifstream file(path.string());

    if (isByChangeScene && m_pOriginalSceneData == nullptr)
        m_pOriginalSceneData = m_editorContext.GetSceneData();

    SceneDefinition sceneDifinition = Serializers::SceneDefinitionSerializer::Deserialize(file);
    sceneDifinition.path = path;

    m_editorContext.SetSceneData(std::move(sceneDifinition));

    LoadScene();
    LoggerSingleton::Instance().LogTrace("SceneController::LoadSceneData -> Finish");
}

bool SceneController::SaveSceneData()
{
    LoggerSingleton::Instance().LogTrace("SceneController::SaveSceneData -> Start");

    SceneDefinition* sceneDefinition = m_editorContext.GetSceneData();

    if (sceneDefinition == nullptr)
    {
        LoggerSingleton::Instance().LogWarning("There is no scene opened!");
        return false;
    }

    Scene* scene = Application::Instance().GetGame()->GetActiveScene();
    Entity* cameraEntity = scene->GetMainCameraEntity();
    sceneDefinition->mainCameraEntityIdentifier = cameraEntity != nullptr ? cameraEntity->GetIdentifier() : "";

    Color* backgroundColor = scene->GetBackgroundColor();
    sceneDefinition->backgroundColor.alpha = backgroundColor->alpha;
    sceneDefinition->backgroundColor.blue = backgroundColor->blue;
    sceneDefinition->backgroundColor.green = backgroundColor->green;
    sceneDefinition->backgroundColor.red = backgroundColor->red;

    GlobalLight* globalLight = scene->GetGlobalLight();
    glm::vec3 globalLightPosition = globalLight->transform.GetPosition();
    glm::vec3 globalLightRotation = globalLight->transform.GetRotation();
    glm::vec3 globalLightScale = globalLight->transform.GetScale();

    sceneDefinition->globalLight.transform.position.x = globalLightPosition.x;
    sceneDefinition->globalLight.transform.position.y = globalLightPosition.y;
    sceneDefinition->globalLight.transform.position.z = globalLightPosition.z;

    sceneDefinition->globalLight.transform.rotation.x = globalLightRotation.x;
    sceneDefinition->globalLight.transform.rotation.y = globalLightRotation.y;
    sceneDefinition->globalLight.transform.rotation.z = globalLightRotation.z;

    sceneDefinition->globalLight.transform.scale.x = globalLightScale.x;
    sceneDefinition->globalLight.transform.scale.y = globalLightScale.y;
    sceneDefinition->globalLight.transform.scale.z = globalLightScale.z;

    sceneDefinition->globalLight.directionalLight.color.blue = globalLight->directionalLight.color.blue;
    sceneDefinition->globalLight.directionalLight.color.green = globalLight->directionalLight.color.green;
    sceneDefinition->globalLight.directionalLight.color.red = globalLight->directionalLight.color.red;
    sceneDefinition->globalLight.directionalLight.color.alpha = globalLight->directionalLight.color.alpha;

    sceneDefinition->globalLight.directionalLight.influence = globalLight->directionalLight.influence;
    sceneDefinition->globalLight.directionalLight.specular.x = globalLight->directionalLight.specular.x;
    sceneDefinition->globalLight.directionalLight.specular.y = globalLight->directionalLight.specular.y;
    sceneDefinition->globalLight.directionalLight.specular.z = globalLight->directionalLight.specular.z;

    sceneDefinition->entities.clear();

    for (Entity* entity : m_editorContext.GetEntityManager()->GetEntities())
    {
        EntityDefinition entityConfig;
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
        {
            entityConfig.components.script.resourceId = script.script->resourceId;
            entityConfig.components.script.assemblyName = script.script->GetAssemblyName();
            entityConfig.components.script.className = script.script->GetClassName();
        }

        if (const CameraComponent& camera = entity->GetComponent<CameraComponent>(); camera.has)
        {
            entityConfig.components.camera.has = true;
            entityConfig.components.camera.fovDegree = camera.fovDegree;
            entityConfig.components.camera.near = camera.near;
            entityConfig.components.camera.far = camera.far;
        }

        if (const ChildrenComponent& children = entity->GetComponent<ChildrenComponent>(); children.has && !children.children.empty())
        {
            for (Entity* child : children.children)
                entityConfig.components.children.childIdentifiers.push_back(child->GetIdentifier());
        }

        if (const ParentComponent& parent = entity->GetComponent<ParentComponent>(); parent.has && parent.parent != nullptr)
            entityConfig.components.parent.parentIdentifier = parent.parent->GetIdentifier();

        if (const UiComponent& uiComponent = entity->GetComponent<UiComponent>(); uiComponent.has)
        {
            entityConfig.components.ui.has = true;
            entityConfig.components.ui.zOrder = uiComponent.zOrder;
            entityConfig.components.ui.resourceId = uiComponent.content->resourceId;
        }

        if (const ColliderComponent& colliderComponent = entity->GetComponent<ColliderComponent>(); colliderComponent.has)
        {
            entityConfig.components.collider.has = true;
            entityConfig.components.collider.enabled = colliderComponent.enabled;
            entityConfig.components.collider.isTrigger = colliderComponent.isTrigger;
            entityConfig.components.collider.center.x = colliderComponent.center.x;
            entityConfig.components.collider.center.y = colliderComponent.center.y;
            entityConfig.components.collider.center.z = colliderComponent.center.z;
            entityConfig.components.collider.size.x = colliderComponent.size.x;
            entityConfig.components.collider.size.y = colliderComponent.size.y;
            entityConfig.components.collider.size.z = colliderComponent.size.z;
        }

        sceneDefinition->entities.push_back(entityConfig);
    }

    if (std::ofstream file(sceneDefinition->path); file.is_open())
    {
        file << Serializers::SceneDefinitionSerializer::Serialize(*sceneDefinition).dump(4);
        file.close();

        LoggerSingleton::Instance().LogInfo("Scene '" + sceneDefinition->path.string() + "' saved successfully");
    }
    else
    {
        LoggerSingleton::Instance().LogError("Failed to open the scene file");
        return false;
    }

    return true;
}

void SceneController::Play()
{
    LoggerSingleton::Instance().LogTrace("SceneController::Play -> Start");

    if (SaveSceneData())
        m_editorContext.GetEditorScene()->SetMustRunScriptComponents(true);
}

void SceneController::Stop()
{
    LoggerSingleton::Instance().LogTrace("SceneController::Stop -> Start");

    m_editorContext.GetEditorScene()->SetMustRunScriptComponents(false);

    if (m_pOriginalSceneData != nullptr)
    {
        m_editorContext.SetSceneData(*m_pOriginalSceneData);
        m_pOriginalSceneData = nullptr;
    }

    LoadScene();
}

void SceneController::LoadScene()
{
    LoggerSingleton::Instance().LogTrace("SceneController::LoadScene -> Start");

    Scene* scene = Application::Instance().GetGame()->GetActiveScene();
    SceneDefinition* sceneDefinition = m_editorContext.GetSceneData();

    if (scene == nullptr)
    {
        LoggerSingleton::Instance().LogWarning("There is no SceneData loaded!");
        return;
    }

    m_editorContext.GetEditorScene()->SetMainCameraEntity(nullptr);
    m_editorContext.SetSelectedEntity(nullptr);
    m_editorContext.SetIsViewSceneData(false);

    m_editorContext.GetEntityManager()->Reset();

    Color* backgroundColor = scene->GetBackgroundColor();
    backgroundColor->blue = sceneDefinition->backgroundColor.blue;
    backgroundColor->red = sceneDefinition->backgroundColor.red;
    backgroundColor->green = sceneDefinition->backgroundColor.green;
    backgroundColor->alpha = sceneDefinition->backgroundColor.alpha;

    GlobalLight* globalLight = scene->GetGlobalLight();

    globalLight->transform.SetPosition({
        sceneDefinition->globalLight.transform.position.x,
        sceneDefinition->globalLight.transform.position.y,
        sceneDefinition->globalLight.transform.position.z,
    });

    globalLight->transform.SetScale({
        sceneDefinition->globalLight.transform.scale.x,
        sceneDefinition->globalLight.transform.scale.y,
        sceneDefinition->globalLight.transform.scale.z,
    });

    globalLight->transform.SetRotation({
        sceneDefinition->globalLight.transform.rotation.x,
        sceneDefinition->globalLight.transform.rotation.y,
        sceneDefinition->globalLight.transform.rotation.z,
    });

    globalLight->directionalLight.color.red = sceneDefinition->globalLight.directionalLight.color.red;
    globalLight->directionalLight.color.green = sceneDefinition->globalLight.directionalLight.color.green;
    globalLight->directionalLight.color.blue = sceneDefinition->globalLight.directionalLight.color.blue;
    globalLight->directionalLight.color.alpha = sceneDefinition->globalLight.directionalLight.color.alpha;
    globalLight->directionalLight.specular.x = sceneDefinition->globalLight.directionalLight.specular.x;
    globalLight->directionalLight.specular.y = sceneDefinition->globalLight.directionalLight.specular.y;
    globalLight->directionalLight.specular.z = sceneDefinition->globalLight.directionalLight.specular.z;
    globalLight->directionalLight.influence = sceneDefinition->globalLight.directionalLight.influence;

    Entity* mainCameraEntity = nullptr;
    vector<Entity*> entities = CreateEntities();

    SetParentAndChildren(mainCameraEntity, entities);

    if (mainCameraEntity != nullptr)
        scene->SetMainCameraEntity(mainCameraEntity);

    LoggerSingleton::Instance().LogTrace("SceneController::LoadScene -> Finish");
}

vector<Entity*> SceneController::CreateEntities()
{
    vector<Entity*> entities;

    for (auto& entityConfig : m_editorContext.GetSceneData()->entities)
    {
        Entity* entity = m_editorContext.GetEntityManager()->AddEntity(entityConfig.tag);
        entity->SetActive(entityConfig.isActive);
        entity->GetName() = entityConfig.name;
        entity->SetIdentifier(entityConfig.identifier);

        TransformComponent& transform = entity->GetComponent<TransformComponent>();
        transform.SetPosition({entityConfig.transform.position.x, entityConfig.transform.position.y, entityConfig.transform.position.z});
        transform.SetScale({entityConfig.transform.scale.x, entityConfig.transform.scale.y, entityConfig.transform.scale.z});
        transform.SetRotation({entityConfig.transform.rotation.x, entityConfig.transform.rotation.y, entityConfig.transform.rotation.z});

        if (!entityConfig.components.mesh.resourceId.empty())
        {
            MeshComponent& meshComponent = entity->GetComponent<MeshComponent>();
            meshComponent.mesh = ResourceManager::Instance().GetMesh(entityConfig.components.mesh.resourceId);
            meshComponent.has = true;
        }

        if (!entityConfig.components.material.resourceId.empty())
        {
            MaterialComponent& materialComponent = entity->GetComponent<MaterialComponent>();
            materialComponent.material = ResourceManager::Instance().GetMaterial(entityConfig.components.material.resourceId);
            materialComponent.has = true;
        }

        if (!entityConfig.components.script.className.empty() && !entityConfig.components.script.assemblyName.empty())
        {
            ScriptComponent& scriptComponent = entity->GetComponent<ScriptComponent>();
            scriptComponent.script = ResourceManager::Instance().GetScript(
                entityConfig.components.script.className,
                entityConfig.components.script.assemblyName
            );
            
            if (scriptComponent.script != nullptr)
                scriptComponent.has = true;
        }

        if (entityConfig.components.camera.has)
        {
            CameraComponent& cameraComponent = entity->GetComponent<CameraComponent>();
            cameraComponent.has = true;
            cameraComponent.fovDegree = entityConfig.components.camera.fovDegree;
            cameraComponent.near = entityConfig.components.camera.near;
            cameraComponent.far = entityConfig.components.camera.far;
        }

        if (!entityConfig.components.ui.resourceId.empty())
        {
            UiComponent& uiComponent = entity->GetComponent<UiComponent>();
            uiComponent.has = true;
            uiComponent.zOrder = entityConfig.components.ui.zOrder;
            uiComponent.content = ResourceManager::Instance().GetUiContent(entityConfig.components.ui.resourceId);
        }

        if (entityConfig.components.collider.has)
        {
            ColliderComponent& colliderComponent = entity->GetComponent<ColliderComponent>();
            colliderComponent.has = true;
            colliderComponent.enabled = entityConfig.components.collider.enabled;
            colliderComponent.isTrigger = entityConfig.components.collider.isTrigger;
            colliderComponent.center.x = entityConfig.components.collider.center.x;
            colliderComponent.center.y = entityConfig.components.collider.center.y;
            colliderComponent.center.z = entityConfig.components.collider.center.z;
            colliderComponent.size.x = entityConfig.components.collider.size.x;
            colliderComponent.size.y = entityConfig.components.collider.size.y;
            colliderComponent.size.z = entityConfig.components.collider.size.z;
        }

        entities.push_back(entity);
    }

    return entities;
}

void SceneController::SetParentAndChildren(Entity*& mainCameraEntity, vector<Entity*> entities)
{
    SceneDefinition* sceneDefinition = m_editorContext.GetSceneData();

    for (auto& entityConfig : sceneDefinition->entities)
    {
        const vector<std::string>& childIds = entityConfig.components.children.childIdentifiers;
        const std::string& parentId = entityConfig.components.parent.parentIdentifier;

        auto itEnt = std::ranges::find_if(entities, [&](Entity* e) { return e->GetIdentifier() == entityConfig.identifier; });

        if (itEnt == entities.end())
            continue;

        Entity* entity = *itEnt;

        if (entityConfig.identifier == sceneDefinition->mainCameraEntityIdentifier)
            mainCameraEntity = entity;

        // children
        if (!childIds.empty())
        {
            auto& childrenComponent = entity->GetComponent<ChildrenComponent>();
            childrenComponent.has = true;

            // filter all entities whose id is in childIds
            auto children_view = entities | std::views::filter([&](Entity* e) { return std::ranges::find(childIds, e->GetIdentifier()) != childIds.end(); });

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
            auto itParent = std::ranges::find_if(entities, [&](Entity* e) { return e->GetIdentifier() == parentId; });

            auto& parentComponent = entity->GetComponent<ParentComponent>();
            parentComponent.parent = (itParent != entities.end()) ? *itParent : nullptr;
            parentComponent.has = true;
        }
    }
}