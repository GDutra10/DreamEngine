#ifndef CORE_GAME_SYSTEM_SCENE_H_
#define CORE_GAME_SYSTEM_SCENE_H_

#include <string>
#include "Camera.h"
#include "GlobalLight.h"
#include "../Color.h"
#include "../ECS/EntityManager.h"
#include "../Resources/ResourceManager.h"
#include "../Sync/SceneData.h"
#include "../Systems/PhysicsSystem.h"

namespace DreamEngine::Core::GameSystem
{

using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Systems;

class CORE_API Scene
{
   public:
    virtual ~Scene() = default;
    explicit Scene(std::string name) : m_pBackgroundColor(new Color{0, 0, 0}),
                                       m_pEntityManager(nullptr),
                                       m_pResourceManager(nullptr),
                                       m_pGlobalLight(new GlobalLight()),
                                       m_showCursor(false),
                                       m_name(std::move(name)) {}

    void* viewport = nullptr;
    [[nodiscard]] bool GetShowCursor() const;
    [[nodiscard]] Color* GetBackgroundColor() const;
    [[nodiscard]] EntityManager* GetEntityManager() const;
    [[nodiscard]] GlobalLight* GetGlobalLight();
    [[nodiscard]] Entity* GetMainCameraEntity() const;
    [[nodiscard]] SceneData* GetSceneData() const;
    virtual Camera& GetCamera();
    virtual bool GetIsFocused() const;
    virtual bool ChangeScene(std::string sceneName);
    bool GetMustRunScriptComponents() const;
    void SetMustRunScriptComponents(bool val);
    void SetShowCursor(bool showCursor);
    void SetMainCameraEntity(Entity* entity);
    
    std::string GetName() const { return m_name; }

    virtual void Update(const float deltaTime);

    // TODO: Initialize
    //         read entities from binary dll and add to the memory pool
    virtual void Initialize();

    // TODO: Unload
    //         remove all entities from the memory pool
    virtual void Unload();

   protected:
    Color* m_pBackgroundColor;
    Camera m_camera;
    EntityManager* m_pEntityManager;
    ResourceManager* m_pResourceManager;
    GlobalLight* m_pGlobalLight;
    Entity* m_pMainCameraEntity = nullptr;
    SceneData* m_pSceneData = new SceneData();
    PhysicsSystem* m_pPhysicsSystem = nullptr;
    bool m_showCursor;
    bool m_mustRunScriptComponents = true;
    bool m_mustRecreateEntitiesInScriptEngine = false;
   private:
    std::string m_name;
};
}  // namespace DreamEngine::Core::GameSystem
#endif