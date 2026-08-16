#ifndef CORE_GAME_SYSTEM_DEFINITIONS_TRANSFORM_DEFINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_TRANSFORM_DEFINITION_H_

#include "Vector3Definition.h"

namespace DreamEngine::Core::GameSystem::Definitions
{
struct TransformDefinition
{
    Vector3Definition position;
    Vector3Definition rotation;
    Vector3Definition scale;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions
#endif