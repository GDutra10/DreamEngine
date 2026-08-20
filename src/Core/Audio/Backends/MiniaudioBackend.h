#ifndef CORE_AUDIO_BACKENDS_MINIAUDIO_BACKEND_H_
#define CORE_AUDIO_BACKENDS_MINIAUDIO_BACKEND_H_

#include <vector>
#include <array>
#include <unordered_map>
#include "../AudioBackendHandle.h"
#include "../AudioClip.h"
#include "../AudioPlayOptions.h"
#include "../AudioBackend.h"
#include "../../../Vendors/miniaudio/miniaudio.h"

namespace DreamEngine::Core::Audio::Backends
{
    using namespace DreamEngine::Core::Audio;
class MiniaudioBackend : public AudioBackend
{
   public:
    ~MiniaudioBackend();
   protected:
    bool HandleInitialize() override;
    AudioBackendHandle HandlePlay(AudioClip& audio, AudioPlayOptions& options) override;
    void HandleStop(AudioBackendHandle handle) override;
    void HandlePause(AudioBackendHandle handle) override;
    void HandleResume(AudioBackendHandle handle) override;
    void HandleSetVolume(AudioBackendHandle handle, float volume) override;
    bool HandleIsPlaying(AudioBackendHandle handle) const override;
    void HandleDestroy(AudioBackendHandle handle) override;

    void HandleSetPitch(AudioBackendHandle handle, float pitch) override;
    void HandleSetPosition(AudioBackendHandle handle, glm::vec3 position) override;

    void HandleSetListener(glm::vec3 position, glm::vec3 direction, glm::vec3 up) override;
    void HandleSetMinDistance(AudioBackendHandle handle, float distance) override;
    void HandleSetMaxDistance(AudioBackendHandle handle, float distance) override;

   private:
       struct Voice
       {
           ma_sound sound{};
           bool initialized{};
       };

       struct CachedClip
       {
           std::string name;
           const AudioClip* clip = nullptr;
       };

       CachedClip* GetOrLoadClip(const AudioClip& audio);
       static Voice* GetVoice(AudioBackendHandle handle) { return static_cast<Voice*>(handle.ptr); }
       void UnloadClips();

  private:
    ma_resource_manager m_resourceManager{};
    ma_engine m_engine{};
    bool m_engineInitialized = true;
    bool m_resourceManagerInitialized = false;
    std::unordered_map<const AudioClip*, CachedClip> m_cachedClips;
};
}  // namespace DreamEngine::Core::Audio
#endif