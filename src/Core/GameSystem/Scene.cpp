#include "Scene.h"

#include "Application.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/DirectionalLightComponent.h"
#include "ECS/Components/ScriptComponent.h"
#include "ECS/Components/NativeScriptComponent.h"
#include "ECS/Components/UiComponent.h"
#include "Scripting/ScriptEngine.h"
#include "Sync/EntitySynchronizer.h"
#include "Sync/GameSynchronizer.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::ECS::Components;

Color* Scene::GetBackgroundColor() const
{
    return m_backgroundColor;
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

void Scene::Update(const float deltaTime)
{
    m_entityManager->Update();

    if (m_mustRunScriptComponents)
        ScriptEngine::UpdateGame(GameSynchronizer::Synchronize(this->GetIsFocused()));

    for (Entity* entity : m_entityManager->GetEntities())
    {
        if (!entity->GetIsActive())
            continue;

        // UI update
        UiComponent& uiComponent = entity->GetComponent<UiComponent>();

        if (uiComponent.has && uiComponent.content != nullptr)
        {
            if (uiComponent.instance == nullptr)
                uiComponent.instance = UiManager::Create(uiComponent.content);
        }

        // native script
        NativeScriptComponent& nativeScriptComponent = entity->GetComponent<NativeScriptComponent>();

        if (nativeScriptComponent.has && nativeScriptComponent.script != nullptr)
        {
            nativeScriptComponent.script->entity = entity;
            nativeScriptComponent.script->Initialize();
            nativeScriptComponent.script->Update();
        }

        // script
        ScriptComponent& scriptComponent = entity->GetComponent<ScriptComponent>();

        if (m_mustRunScriptComponents && scriptComponent.has && scriptComponent.script != nullptr)
        {
            EntitySynchronizer::SynchronizeToData(entity);
            scriptComponent.instance = scriptComponent.script->Update(scriptComponent.instance, &entity->entityData);
            EntitySynchronizer::SynchronizeFromData(entity);
        }
    }

    UiManager::Update();
}

void Scene::Initialize()
{
    m_entityManager = new ECS::EntityManager();
    m_resourceManager = new Resources::ResourceManager();

    // set default global light
    m_globalLight->transform.SetPosition({0.f, 500.f, 0.f});
    m_globalLight->directionalLight.color = {1.f, 1.f, 1.f};
    m_globalLight->directionalLight.specular = {1.f, 1.f, 1.f};
    m_globalLight->directionalLight.influence = 2.0f;
}

void Scene::Unload()
{
    UiManager::RemoveContents();

    delete m_entityManager;
    delete m_resourceManager;
}

EntityManager* Scene::GetEntityManager() const
{
    return m_entityManager;
}

GlobalLight* Scene::GetGlobalLight()
{
    return m_globalLight;
}

Entity* Scene::GetMainCameraEntity() const
{
    return m_pMainCameraEntity;
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
    return m_mustRunScriptComponents;
}

bool Scene::GetIsFocused() const
{
    return Application::Instance().GetIsFocused();
}

void Scene::SetMustRunScriptComponents(const bool val)
{
    m_mustRunScriptComponents = val;
}