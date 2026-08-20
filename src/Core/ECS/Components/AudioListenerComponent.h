#ifndef CORE_ECS_COMPONENTS_AUDIO_LISTENER_COMPONENT_H_
#define CORE_ECS_COMPONENTS_AUDIO_LISTENER_COMPONENT_H_

#include "Component.h"

namespace DreamEngine::Core::ECS::Components
{

class CORE_API AudioListenerComponent : public Component
{
   public:
    bool enabled = true;

    explicit AudioListenerComponent() = default;
    
    void Reset() override
    {
        enabled = true;
    }
};
}  // namespace DreamEngine::Core::ECS::Components
#endif