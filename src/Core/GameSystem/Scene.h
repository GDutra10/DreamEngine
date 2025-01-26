#ifndef CORE_GAME_SYSTEM_SCENE_H_
#define CORE_GAME_SYSTEM_SCENE_H_

#include <string>
#include "Camera.h"
#include "GlobalLight.h"
#include "../Color.h"
#include "../ECS/EntityManager.h"
#include "../Resources/ResourceManager.h"

namespace DreamEngine::Core::GameSystem
{

using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::Resources;

class Scene
{
   public:
    virtual ~Scene() = default;
    explicit Scene(std::string name) : m_backgroundColor(new Color{0, 0, 0}),
                                       m_entityManager(nullptr),
                                       m_resourceManager(nullptr),
                                       m_globalLight(new GlobalLight()),
                                       m_showCursor(false),
                                       m_name(std::move(name)) {}

    void* viewport = nullptr;
    [[nodiscard]] bool GetShowCursor() const;
    [[nodiscard]] Color* GetBackgroundColor() const;
    [[nodiscard]] EntityManager* GetEntityManager() const;
    [[nodiscard]] GlobalLight* GetGlobalLight();
    Camera& GetCamera();
    bool GetMustRunScriptComponents() const;
    void SetMustRunScriptComponents(bool val);
    void SetShowCursor(bool showCursor);

    virtual void Update(const float deltaTime);

    // TODO: Initialize
    //         read entities from binary dll and add to the memory pool
    virtual void Initialize();

    // TODO: Unload
    //         remove all entities from the memory pool
    virtual void Unload();

   protected:
    Color* m_backgroundColor;
    Camera m_camera;
    EntityManager* m_entityManager;
    ResourceManager* m_resourceManager;
    GlobalLight* m_globalLight;
    bool m_showCursor;
    bool m_mustRunScriptComponents = true;
   private:
    std::string m_name;
};
}  // namespace DreamEngine::Core::GameSystem
#endif