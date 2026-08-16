#ifndef EDITOR_GAME_SYSTEM_DEFINITIONS_DIRECTIONAL_LIGHT_DEFINITION_H_
#define EDITOR_GAME_SYSTEM_DEFINITIONS_DIRECTIONAL_LIGHT_DEFINITION_H_

#include "ColorDefinition.h"
#include "Vector3Definition.h"

namespace DreamEngine::Core::GameSystem::Definitions
{
struct DirectionalLightDefinition
{
    Vector3Definition specular;
    ColorDefinition color;
    float influence;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions
#endif