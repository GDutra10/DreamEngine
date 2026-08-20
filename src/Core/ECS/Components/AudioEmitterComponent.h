#ifndef CORE_ECS_COMPONENTS_AUDIO_EMITTER_COMPONENT_H_
#define CORE_ECS_COMPONENTS_AUDIO_EMITTER_COMPONENT_H_

#include "Component.h"

namespace DreamEngine::Core::ECS::Components
{

class CORE_API AudioEmitterComponent : public Component
{
   public:
    bool enabled = true;
    float volume = 1.0f;
    float pitch = 1.0f;
    bool spatial = false;
    float minDistance = 1.0f;
    float maxDistance = 100.0f;

    explicit AudioEmitterComponent() = default;

    void Reset() override
    {
        enabled = true;
        volume = 1.0f;
        pitch = 1.0f;
        spatial = false;
        minDistance = 1.0f;
        maxDistance = 100.0f;
    }
};
}  // namespace DreamEngine::Core::ECS::Components
#endif