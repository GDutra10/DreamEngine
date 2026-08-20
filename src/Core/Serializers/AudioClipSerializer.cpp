#include "AudioClipSerializer.h"

#include <fstream>
#include <nlohmann/json.hpp>

#ifndef AUDIO_PROPERTY_RESOURCE_ID
#define AUDIO_PROPERTY_RESOURCE_ID "resourceId"
#endif

#ifndef AUDIO_PROPERTY_NAME
#define AUDIO_PROPERTY_NAME "name"
#endif

#ifndef AUDIO_PROPERTY_FILE_PATH
#define AUDIO_PROPERTY_FILE_PATH "filePath"
#endif

using namespace DreamEngine::Core::Serializers;
using namespace DreamEngine::Core::Audio;

using json = nlohmann::json;

json AudioClipSerializer::Serialize(const AudioClip& audio)
{
    json audioJson;

    audioJson[AUDIO_PROPERTY_RESOURCE_ID] = audio.resourceId;
    audioJson[AUDIO_PROPERTY_NAME] = audio.name;
    audioJson[AUDIO_PROPERTY_FILE_PATH] = audio.filePath;

    return audioJson;
}

AudioClip* AudioClipSerializer::Deserialize(std::ifstream& stream)
{
    json j;
    stream >> j;

    AudioClip* clip = new AudioClip();
    clip->resourceId = j[AUDIO_PROPERTY_RESOURCE_ID];
    clip->name = j[AUDIO_PROPERTY_NAME];
    clip->filePath = j[AUDIO_PROPERTY_FILE_PATH];

    return clip;
}
