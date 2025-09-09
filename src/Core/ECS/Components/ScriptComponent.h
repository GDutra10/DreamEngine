#ifndef CORE_ECS_COMPONENTS_SCRIPT_COMPONENT_H_
#define CORE_ECS_COMPONENTS_SCRIPT_COMPONENT_H_

#include "../../Core/Scripting/Script.h"
#include "Component.h"

namespace DreamEngine::Core::ECS::Components
{
using namespace DreamEngine::Core::Scripting;

class CORE_API ScriptComponent : public Component
{
   public:
    Script* script;
    void* instance = nullptr;

    explicit ScriptComponent() = default;
    ScriptComponent(Script* script) : script(script) {}
};
}  // namespace DreamEngine::Core::ECS::Components
#endif