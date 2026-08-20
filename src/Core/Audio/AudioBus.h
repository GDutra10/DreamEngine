#ifndef CORE_AUDIO_AUDIO_BUS_H_
#define CORE_AUDIO_AUDIO_BUS_H_

#include <vector>
#include "../CoreCApi.h"

namespace DreamEngine::Core::Audio
{
enum class CORE_API AudioBus{
   Master,
   Music,
   SFX,
   Ambient,
   UI,
   Count
};
}  // namespace DreamEngine::Core::Resources
#endif