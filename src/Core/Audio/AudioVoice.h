#ifndef CORE_AUDIO_AUDIO_VOICE_H_
#define CORE_AUDIO_AUDIO_VOICE_H_

#include <vector>
#include "AudioBackendHandle.h"
#include "AudioBus.h"

namespace DreamEngine::Core::Audio
{

enum class AudioVoiceState
{
    Playing,
    Paused
};

struct AudioVoice
{
    AudioBackendHandle audioHandle;
    unsigned int generation;
    int entityId;
    bool spatial;
    bool active;
    float volume = 1.0f;
    float emitterVolume = 1.0f;
    AudioBus bus;
    AudioVoiceState state = AudioVoiceState::Playing;
};
}  // namespace DreamEngine::Core::Audio
#endif