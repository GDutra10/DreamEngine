#ifndef CORE_ECS_COMPONENTS_NATIVE_SCRIPT_COMPONENT_H_
#define CORE_ECS_COMPONENTS_NATIVE_SCRIPT_COMPONENT_H_

#include "../../Core/Scripting/NativeScript.h"
#include "Component.h"

namespace DreamEngine::Core::ECS::Components
{
using namespace DreamEngine::Core::Scripting;

class CORE_API NativeScriptComponent : public Component
{
   public:
    NativeScript* script = nullptr;

    explicit NativeScriptComponent() = default;
    NativeScriptComponent(NativeScript* script) : script(script) {}
};
}  // namespace DreamEngine::Core::ECS::Components
#endif