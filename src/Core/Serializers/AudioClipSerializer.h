#ifndef CORE_SERIALIZERS_AUDIO_SERIALIZER_H_
#define CORE_SERIALIZERS_AUDIO_SERIALIZER_H_

#include <string>

#include <fstream>
#include <nlohmann/json.hpp>

#include "../../Core/Audio/AudioClip.h"

namespace DreamEngine::Core::Serializers
{
using namespace DreamEngine::Core::Audio;
using json = nlohmann::json;

class CORE_API AudioClipSerializer
{
   public:
    static json Serialize(const AudioClip& audio);
    static AudioClip* Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Core::Serializers
#endif