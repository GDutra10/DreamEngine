#ifndef CORE_GAME_SYSTEM_GLOBAL_LIGHT_H_
#define CORE_GAME_SYSTEM_GLOBAL_LIGHT_H_

#include "../ECS/Components/DirectionalLightComponent.h"
#include "../ECS/Components/TransformComponent.h"

namespace DreamEngine::Core::GameSystem
{
    using namespace DreamEngine::Core::ECS::Components;
class GlobalLight
{
   public:
    DirectionalLightComponent directionalLight;
    TransformComponent transform;

    explicit GlobalLight() = default;
};
}  // namespace DreamEngine::Core::GameSystem
#endif