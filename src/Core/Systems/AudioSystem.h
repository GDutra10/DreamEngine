#ifndef CORE_SYSTEM_AUDIO_SYSTEM_H_
#define CORE_SYSTEM_AUDIO_SYSTEM_H_

#include <vector>
#include <array>
#include "../Audio/AudioBackend.h"
#include "../Audio/AudioBus.h"
#include "../Audio/AudioHandle.h"
#include "../Audio/AudioVoice.h"
#include "../GameSystem/Scene.h"
#include "../CoreCApi.h"

namespace DreamEngine::Core::Systems
{
    using namespace DreamEngine::Core::Audio;
    using namespace DreamEngine::Core::GameSystem;
class CORE_API AudioSystem
{
   public:
    static AudioSystem& Instance();
    void SetBackend(AudioBackend* audioBackend);

    AudioHandle Play(const std::string& resourceId, AudioPlayOptions options);
    AudioHandle Play(unsigned int entityId, std::string resourceId, AudioPlayOptions options);
    void Stop(AudioHandle handle);
    void Pause(AudioHandle handle);
    void Resume(AudioHandle handle);
    
    void SetVolume(AudioHandle handle, float volume);
    void SetPitch(AudioHandle handle, float pitch);
    void SetBusVolume(AudioBus bus, float volume);
    
    void Update(Scene& scene);

    void StopByEntity(unsigned int entityId);
    void StopAll();

   private:
    AudioSystem();
    
    std::vector<AudioVoice> m_voices{};
    std::vector<AudioBackend*> m_audioBackends{};
    std::vector<uint32_t> m_freeVoices{};
    std::array<float, static_cast<std::size_t>(AudioBus::Count)> m_busVolumes{};
    AudioBackend* m_pAudioBackend = nullptr;
    
    void RefreshVoiceVolume(AudioVoice& voice);

    bool UpdateListener(Entity& entity);
    void UpdateEmitter(Entity& entity);
    void UpdateSpatialVoices(const bool hasListener);
    
    void CleanupFinishedVoices();
    AudioVoice* GetVoice(AudioHandle handle);
    uint32_t AcquireVoice();
    void ReleaseVoice(uint32_t index);
};
}  // namespace DreamEngine::Core::Systems
#endif