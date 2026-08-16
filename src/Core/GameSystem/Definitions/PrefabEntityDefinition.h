#ifndef CORE_GAME_SYSTEM_DEFINITIONS_PRE_FAB_ENTITY_DEFINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_PRE_FAB_ENTITY_DEFINITION_H_

#include <vector>
#include <string>
#include "EntityDefinition.h"

namespace DreamEngine::Core::GameSystem::Definitions
{
struct PrefabEntityDefinition
{
    std::string resourceId;
    EntityDefinition entity;
    std::vector<PrefabEntityDefinition> children;
};
}  // namespace DreamEngine::Core::GameSystem::Definitions
#endif