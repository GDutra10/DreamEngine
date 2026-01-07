#ifndef CORE_ECS_COMPONENTS_UI_COMPONENT_H_
#define CORE_ECS_COMPONENTS_UI_COMPONENT_H_

#include "Component.h"
#include "UI/UiContent.h"

namespace DreamEngine::Core::ECS::Components
{
class CORE_API UiComponent : public Component
{
   public:
    UI::UiContent* content;
    int zOrder = 0;
    void* instance = nullptr;
};
}  // namespace DreamEngine::Core::ECS::Components
#endif