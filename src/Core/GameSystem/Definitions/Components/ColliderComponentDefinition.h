#ifndef CORE_GAME_SYSTEM_DEFINITIONS_COLLIDER_COMPONENT_DEFINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_COLLIDER_COMPONENT_DEFINITION_H_

#include "../Vector3Definition.h"

namespace DreamEngine::Core::GameSystem::Definitions::Components
{
struct ColliderComponentDefinition
{
    bool has = false;
    bool enabled;
    bool isTrigger;
    Vector3Definition center;
    Vector3Definition size;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions::Components
#endif