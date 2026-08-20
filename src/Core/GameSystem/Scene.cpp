#include "Scene.h"

#include "Application.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/DirectionalLightComponent.h"
#include "ECS/Components/ScriptComponent.h"
#include "ECS/Components/NativeScriptComponent.h"
#include "ECS/Components/UiComponent.h"
#include "Scripting/ScriptEngine.h"
#include "Scripting/ScriptEventHandler.h"
#include "Sync/EntitySynchronizer.h"
#include "Sync/GameSynchronizer.h"
#include "PreFab.h"
#include "Definitions/SceneDefinition.h"
#include "Loggers/LoggerSingleton.h"
#include "Systems/AudioSystem.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::GameSystem::Definitions;
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::Systems;

Color* Scene::GetBackgroundColor() const
{
    return m_pBackgroundColor;
}

bool Scene::GetShowCursor() const
{
    return m_showCursor;
}

void Scene::SetShowCursor(const bool showCursor)
{
    m_showCursor = showCursor;
}

void Scene::SetMainCameraEntity(Entity* entity)
{
    m_pMainCameraEntity = entity;
}

int Scene::InstantiatePrefab(const char* resourceId)
{
    LoggerSingleton::Instance().LogTrace("Scene::IntantiatePrefab -> Start -> resource id: " + std::string(resourceId));
    
    Prefab* prefab = ResourceManager::Instance().GetPrefab(resourceId);

    if (prefab == nullptr)
    {
        LoggerSingleton::Instance().LogTrace("Scene::IntantiatePrefab -> Prefab not found");
        return -1;
    }

    return InstantiatePrefab(prefab->root, -1);
}

int Scene::InstantiatePrefab(PrefabEntityDefinition& definition, int parentId)
{
    Entity* parent = parentId >= 0 
        ? m_pEntityManager->GetEntityById(parentId, true)
        : nullptr;
    
    const bool hasParent = parent != nullptr;

    Entity* entity = hasParent
        ? m_pEntityManager->AddEntity(definition.entity.tag, parent) 
        : m_pEntityManager->AddEntity(definition.entity.tag);

    entity->ApplyDefinition(definition.entity);

    for (auto& child : definition.children)
        InstantiatePrefab(child, entity->GetId());

    return entity->GetId();
}

bool Scene::ChangeScene(const std::string sceneName)
{
    return Application::Instance().GetGame()->ChangeActiveScene(sceneName);
}

void Scene::Update(const float deltaTime)
{
    m_pEntityManager->Update();

    for (Entity* entity : m_pEntityManager->GetEntities())
    {
        if (!entity->GetIsActive())
        {
            UiComponent& uiComponent = entity->GetComponent<UiComponent>();

            if (uiComponent.has && uiComponent.content != nullptr)
            {
                if (uiComponent.instance != nullptr)
                    UiManager::Hide(uiComponent.instance);
            }

            continue;
        }

        // UI update
        UiComponent& uiComponent = entity->GetComponent<UiComponent>();

        if (uiComponent.has && uiComponent.content != nullptr)
        {
            if (uiComponent.instance == nullptr)
                uiComponent.instance = UiManager::Create(uiComponent.content);
            else
                UiManager::Show(uiComponent.instance);
        }

        // native script
        NativeScriptComponent& nativeScriptComponent = entity->GetComponent<NativeScriptComponent>();

        if (nativeScriptComponent.has && nativeScriptComponent.script != nullptr)
        {
            nativeScriptComponent.script->entity = entity;
            nativeScriptComponent.script->Initialize();
            nativeScriptComponent.script->Update();
        }
    }

    if (m_mustRunManagedScripts)
    {
        ScriptEventHandler::Process();
        GameData* pGameData = GameSynchronizer::Synchronize(this->GetIsFocused());

        std::vector<EntityData> entityDataArray;

        for (Entity* entity : m_pEntityManager->GetEntities())
        {
            EntitySynchronizer::SynchronizeToData(entity);
            entityDataArray.push_back(entity->entityData);
        }

        int size = static_cast<int>(entityDataArray.size());
        EntityData* pEntityDataList = entityDataArray.data();
        ScriptEngine::UpdateGame(pGameData, pEntityDataList, size);

        for (size_t i = 0; i < size; i++)
        {
            Entity* entity = m_pEntityManager->GetEntityById(entityDataArray[i].id);
            entity->entityData = pEntityDataList[i];
            EntitySynchronizer::SynchronizeFromData(entity);
        }
    }

    m_pPhysicsSystem->Update(*m_pEntityManager, deltaTime);
    ScriptEventHandler::ProcessCollisionEvents(*m_pEntityManager, m_mustRunManagedScripts);

    AudioSystem::Instance().Update(*this);

    UiManager::Update();
    m_mustRecreateEntitiesInScriptEngine = false;
}

void Scene::Initialize()
{
    m_pEntityManager = new ECS::EntityManager();
    m_pResourceManager = new Resources::ResourceManager();
    m_pPhysicsSystem = new Systems::PhysicsSystem();

    // set default global light
    m_pGlobalLight->transform.SetPosition({0.f, 500.f, 0.f});
    m_pGlobalLight->directionalLight.color = {1.f, 1.f, 1.f};
    m_pGlobalLight->directionalLight.specular = {1.f, 1.f, 1.f};
    m_pGlobalLight->directionalLight.influence = 2.0f;
}

void Scene::Unload()
{
    UiManager::RemoveContents();
    AudioSystem::Instance().StopAll();

    delete m_pEntityManager;
    delete m_pResourceManager;
    delete m_pPhysicsSystem;
}

EntityManager* Scene::GetEntityManager() const
{
    return m_pEntityManager;
}

GlobalLight* Scene::GetGlobalLight()
{
    return m_pGlobalLight;
}

Entity* Scene::GetMainCameraEntity() const
{
    return m_pMainCameraEntity;
}

SceneData* Scene::GetSceneData() const
{
    m_pSceneData->mainCameraEntityId = m_pMainCameraEntity != nullptr ? m_pMainCameraEntity->GetId() : 0;
    m_pSceneData->showCursor = m_showCursor ? 1 : 0;
    m_pSceneData->mustRecreateEntities = m_mustRecreateEntitiesInScriptEngine ? 1 : 0;
    m_pSceneData->globalLightColorR = m_pGlobalLight->directionalLight.color.red;
    m_pSceneData->globalLightColorG = m_pGlobalLight->directionalLight.color.green;
    m_pSceneData->globalLightColorB = m_pGlobalLight->directionalLight.color.blue;
    m_pSceneData->globalLightIntensity = m_pGlobalLight->directionalLight.influence;

    return m_pSceneData;
}

Camera& Scene::GetCamera()
{
    if (m_pMainCameraEntity == nullptr)
        return m_camera;

    const CameraComponent& cameraComponent = m_pMainCameraEntity->GetComponent<CameraComponent>();

    if (!cameraComponent.has)
        return m_camera;

    const TransformComponent& transformComponent = m_pMainCameraEntity->GetComponent<TransformComponent>();
    const glm::vec3 right = glm::normalize(glm::vec3(transformComponent.transform[0]));
    const glm::vec3 up = glm::normalize(glm::vec3(transformComponent.transform[1]));
    const glm::vec3 fwd = -glm::normalize(glm::vec3(transformComponent.transform[2]));  // -Z forward convention

    m_camera.position = glm::vec3(transformComponent.transform[3]);
    m_camera.right = right;
    m_camera.up = up;
    m_camera.front = fwd;
    m_camera.fovDegree = cameraComponent.fovDegree;
    m_camera.near = cameraComponent.near;
    m_camera.far = cameraComponent.far;

    m_camera.worldUp = glm::vec3(0, 1, 0);

    return m_camera;
}

bool Scene::GetMustRunScriptComponents() const
{
    return m_mustRunManagedScripts;
}

bool Scene::GetIsFocused() const
{
    return Application::Instance().GetIsFocused();
}

void Scene::SetMustRunScriptComponents(const bool val)
{
    if (m_mustRunManagedScripts == false && val == true)
        m_mustRecreateEntitiesInScriptEngine = true;

    m_mustRunManagedScripts = val;
}