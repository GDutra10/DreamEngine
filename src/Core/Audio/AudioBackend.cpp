#include "AudioBackend.h"

using namespace DreamEngine::Core::Audio;

bool AudioBackend::Initialize()
{
    if (m_initialized)
        return true;

    m_initialized = HandleInitialize();
    return m_initialized;

    return false;
}

AudioBackendHandle AudioBackend::Play(AudioClip& audio, AudioPlayOptions& options)
{
    if (!m_initialized)
        return {};

    return HandlePlay(audio, options);
}

void AudioBackend::Stop(AudioBackendHandle handle) 
{
    if (!m_initialized || !handle)
        return;

    HandleStop(handle);
}

void AudioBackend::Pause(AudioBackendHandle handle) 
{
    if (!m_initialized || !handle)
        return;

    HandlePause(handle);
}

void AudioBackend::Resume(AudioBackendHandle handle) 
{
    if (!m_initialized || !handle)
        return;

    HandleResume(handle);
}

void AudioBackend::SetVolume(AudioBackendHandle handle, float volume) 
{
    if (!m_initialized || !handle)
        return;

    HandleSetVolume(handle, volume);
}

bool AudioBackend::IsPlaying(AudioBackendHandle handle)
{
    if (!m_initialized || !handle)
        return false;

    return HandleIsPlaying(handle);
}

void AudioBackend::Destroy(AudioBackendHandle handle) 
{
    if (!m_initialized || !handle)
        return;

    HandleDestroy(handle);
}

void AudioBackend::SetPitch(AudioBackendHandle handle, float pitch) 
{
    if (!m_initialized || !handle)
        return;

    return HandleSetPitch(handle, pitch);
}

void AudioBackend::SetPosition(AudioBackendHandle handle, glm::vec3 position)
{
    if (!m_initialized || !handle)
        return;

    return HandleSetPosition(handle, position);
}

void AudioBackend::SetListener(glm::vec3 position, glm::vec3 forward, glm::vec3 up) 
{
    if (!m_initialized)
        return;

    return HandleSetListener(position, forward, up);
}

void AudioBackend::SetMinDistance(AudioBackendHandle handle, float distance)
{
    if (!m_initialized || !handle)
        return;

    HandleSetMinDistance(handle, distance);
}

void AudioBackend::SetMaxDistance(AudioBackendHandle handle, float distance)
{
    if (!m_initialized || !handle)
        return;

    HandleSetMaxDistance(handle, distance);
}
