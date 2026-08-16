#ifndef CORE_GAME_SYSTEM_DEFINITIONS_GLOBAL_LIGHT_DEFINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_GLOBAL_LIGHT_DEFINITION_H_

#include "TransformDefinition.h"
#include "DirectionalLightDefinition.h"

namespace DreamEngine::Core::GameSystem::Definitions
{
struct GlobalLightDefinition
{
    TransformDefinition transform;
    DirectionalLightDefinition directionalLight;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions
#endif