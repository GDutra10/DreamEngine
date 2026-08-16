#ifndef EDITOR_CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_DEFINITION_H_
#define EDITOR_CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_DEFINITION_H_

#include "Components/CameraComponentDefinition.h"
#include "Components/ChildrenComponentDefinition.h"
#include "Components/MaterialComponentDefinition.h"
#include "Components/MeshComponentDefinition.h"
#include "Components/ParentComponentDefinition.h"
#include "Components/ScriptComponentDefinition.h"
#include "Components/UiComponentDefinition.h"
#include "Components/ColliderComponentDefinition.h"

namespace DreamEngine::Core::GameSystem::Definitions
{
using namespace DreamEngine::Core::GameSystem::Definitions::Components;

struct ComponentsDefinition
{
    ChildrenComponentDefinition children;
    MaterialComponentDefinition material;
    MeshComponentDefinition mesh;
    ParentComponentDefinition parent;
    ScriptComponentDefinition script;
    CameraComponentDefinition camera;
    UiComponentDefinition ui;
    ColliderComponentDefinition collider;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions
#endif