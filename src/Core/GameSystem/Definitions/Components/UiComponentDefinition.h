#ifndef CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_UI_COMPONENT_DEFINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_UI_COMPONENT_DEFINITION_H_

#include <string>

namespace DreamEngine::Core::GameSystem::Definitions::Components
{
struct UiComponentDefinition
{
    bool has = false;
    int zOrder = 0;
    std::string resourceId = "";
};

}  // namespace DreamEngine::Core::GameSystem::Definitions::Components
#endif