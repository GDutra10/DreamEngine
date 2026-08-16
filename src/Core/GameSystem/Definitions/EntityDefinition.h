#ifndef CORE_GAME_SYSTEM_DEFINITIONS_ENTITY_DEFIINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_ENTITY_DEFIINITION_H_

#include <string>

#include "TransformDefinition.h"
#include "ComponentsDefinition.h"
#include "../../ECS/Components/MeshComponent.h"

namespace DreamEngine::Core::GameSystem::Definitions
{
    using namespace std;
    using namespace DreamEngine::Core::ECS::Components;

struct EntityDefinition
{
    string tag;
    string name;
    string identifier;
    bool isActive;
    TransformDefinition transform;
    ComponentsDefinition components;
};
}  // namespace DreamEngine::Core::GameSystem::Definitions
#endif