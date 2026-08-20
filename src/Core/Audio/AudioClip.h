#ifndef CORE_AUDIO_AUDIO_CLIP_H_
#define CORE_AUDIO_AUDIO_CLIP_H_

#include <vector>

#include "../CoreCApi.h"
#include "../Resources/Resource.h"

namespace DreamEngine::Core::Audio
{
    using namespace DreamEngine::Core::Resources;
class CORE_API AudioClip : public Resource
{
   public:
    std::vector<uint8_t> data;
    std::string filePath;
};
}  // namespace DreamEngine::Core::Audio
#endif