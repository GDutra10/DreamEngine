#ifndef CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_SCRIPT_COMPONENT_DEFINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_SCRIPT_COMPONENT_DEFINITION_H_

#include <string>

namespace DreamEngine::Core::GameSystem::Definitions::Components
{
struct ScriptComponentDefinition
{
    std::string resourceId;
    std::string className;
    std::string assemblyName;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions::Components
#endif