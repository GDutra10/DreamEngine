#ifndef CORE_AUDIO_AUDIO_PLAY_OPTIONS_H_
#define CORE_AUDIO_AUDIO_PLAY_OPTIONS_H_

#include <vector>

#include "AudioBus.h"
#include "../CoreCApi.h"


namespace DreamEngine::Core::Audio
{

enum class AudioLoadMode
{
    Auto,
    Memory,
    Stream
};

struct CORE_API AudioPlayOptions
{
    AudioBus bus = AudioBus::Master;
    float volume = 1.0f;
    float pitch = 1.0f;
    bool loop;
    bool spatial;
    //AudioLoadMode loadMode = AudioLoadMode::Auto;
};
}  // namespace DreamEngine::Core::Audio
#endif