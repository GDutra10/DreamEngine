#ifndef CORE_AUDIO_AUDIO_BACKEND_H_
#define CORE_AUDIO_AUDIO_BACKEND_H_

#include <vector>
#include <glm/glm.hpp>

#include "AudioBackendHandle.h"
#include "AudioClip.h"
#include "AudioPlayOptions.h"
#include "../CoreCApi.h"


namespace DreamEngine::Core::Audio
{
class AudioBackend
{
   public:
    bool Initialize();
    AudioBackendHandle Play(AudioClip& audio, AudioPlayOptions& options);
    void Stop(AudioBackendHandle handle);
    void Pause(AudioBackendHandle handle);
    void Resume(AudioBackendHandle handle);
    void SetVolume(AudioBackendHandle handle, float volume);
    bool IsPlaying(AudioBackendHandle handle);
    void Destroy(AudioBackendHandle handle);
    
    // sets
    void SetPitch(AudioBackendHandle handle, float pitch);
    void SetPosition(AudioBackendHandle handle, glm::vec3 position);
    void SetListener(glm::vec3 position, glm::vec3 direction, glm::vec3 up); 
    void SetMinDistance(AudioBackendHandle handle, float distance);
    void SetMaxDistance(AudioBackendHandle handle, float distance);

   protected:
    virtual bool HandleInitialize() = 0;
    virtual AudioBackendHandle HandlePlay(AudioClip& audio, AudioPlayOptions& options) = 0;
    virtual void HandleStop(AudioBackendHandle handle) = 0;
    virtual void HandlePause(AudioBackendHandle handle) = 0;
    virtual void HandleResume(AudioBackendHandle handle) = 0;
    virtual void HandleSetVolume(AudioBackendHandle handle, float volume) = 0;
    virtual bool HandleIsPlaying(AudioBackendHandle handle) const = 0;
    virtual void HandleDestroy(AudioBackendHandle handle) = 0;
    
    // sets
    virtual void HandleSetPitch(AudioBackendHandle handle, float pitch) = 0;
    virtual void HandleSetPosition(AudioBackendHandle handle, glm::vec3 position) = 0;
    virtual void HandleSetListener(glm::vec3 position, glm::vec3 direction, glm::vec3 up) = 0;
    virtual void HandleSetMinDistance(AudioBackendHandle handle, float distance) = 0;
    virtual void HandleSetMaxDistance(AudioBackendHandle handle, float distance) = 0;

   private:
    bool m_initialized;
};
}  // namespace DreamEngine::Core::Audio
#endif