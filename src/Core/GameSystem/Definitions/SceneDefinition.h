#ifndef CORE_GAME_SYSTEM_DEFINITIONS_SCENE_DEFINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_SCENE_DEFINITION_H_

#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include "EntityDefinition.h"
#include "ColorDefinition.h"
#include "GlobalLightDefinition.h"
#include <Application.h>

namespace DreamEngine::Core::GameSystem::Definitions
{
struct SceneDefinition
{
    filesystem::path path;
    ColorDefinition backgroundColor;
    GlobalLightDefinition globalLight;
    std::string mainCameraEntityIdentifier;
    std::vector<EntityDefinition> entities;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions
#endif