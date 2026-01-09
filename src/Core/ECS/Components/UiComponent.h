#ifndef CORE_ECS_COMPONENTS_UI_COMPONENT_H_
#define CORE_ECS_COMPONENTS_UI_COMPONENT_H_

#include "Component.h"
#include "UI/UiContent.h"
#include "UI/UiInstance.h"

namespace DreamEngine::Core::ECS::Components
{
class CORE_API UiComponent : public Component
{
   public:
    int zOrder = 0;
    UI::UiContent* content;
    UI::UiInstance* instance = nullptr;
};
}  // namespace DreamEngine::Core::ECS::Components
#endif