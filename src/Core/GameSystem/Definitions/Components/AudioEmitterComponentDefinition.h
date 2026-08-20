#ifndef CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_AUDIO_EMITTER_COMPONENT_DEFINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_AUDIO_EMITTER_COMPONENT_DEFINITION_H_

namespace DreamEngine::Core::GameSystem::Definitions::Components
{
struct AudioEmitterComponentDefinition
{
    bool has = false;
    bool enabled = true;
    float volume = 1.0f;
    float pitch = 1.0f;
    bool spatial = false;
    float minDistance = 1.0f;
    float maxDistance = 100.0f;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions::Components
#endif