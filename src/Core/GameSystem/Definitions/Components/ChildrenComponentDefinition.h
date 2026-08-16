#ifndef CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_CHILDREN_COMPONENT_DEINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_CHILDREN_COMPONENT_DEINITION_H_

#include <string>
#include <vector>

namespace DreamEngine::Core::GameSystem::Definitions::Components
{
struct ChildrenComponentDefinition
{
    std::vector<std::string> childIdentifiers;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions::Components
#endif