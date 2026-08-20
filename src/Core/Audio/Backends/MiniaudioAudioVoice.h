#ifndef CORE_AUDIO_BACKENDS_MINIAUDIO_VOICE_H_
#define CORE_AUDIO_BACKENDS_MINIAUDIO_VOICE_H_

#include <vector>
#include "../../../Vendors/miniaudio/miniaudio.h"

namespace DreamEngine::Core::Audio::Backends
 {
struct MiniaudioVoice
{
    ma_sound m_sound;
    bool initialized;
};
}  // namespace DreamEngine::Core::Audio::Backends
#endif