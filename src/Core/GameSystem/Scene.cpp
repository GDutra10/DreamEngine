#include "Scene.h"

#include "ECS/Components/DirectionalLightComponent.h"
#include "ECS/Components/ScriptComponent.h"
#include "ECS/Components/NativeScriptComponent.h"
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

void Scene::Update(const float deltaTime)
{
    m_entityManager->Update();

    if (m_mustRunScriptComponents)
        ScriptEngine::UpdateGame(GameSynchronizer::Synchronize());

    for (Entity* entity : m_entityManager->GetEntities())
    {
        if (!entity->GetIsActive())
            continue;

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

Camera& Scene::GetCamera()
{
    return m_camera;
}

bool Scene::GetMustRunScriptComponents() const
{
    return m_mustRunScriptComponents;
}

void Scene::SetMustRunScriptComponents(const bool val)
{
    m_mustRunScriptComponents = val;
}