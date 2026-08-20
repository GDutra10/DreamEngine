#include "AudioSystem.h"
#include <algorithm>
#include <ECS/Components/AudioListenerComponent.h>
#include <ECS/Components/AudioEmitterComponent.h>
#include <Loggers/LoggerSingleton.h>

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::Systems;
using namespace DreamEngine::Core::ECS::Components;

AudioSystem::AudioSystem() 
{
    m_busVolumes.fill(1.0f);
}

AudioSystem& AudioSystem::Instance()
{
    static AudioSystem audioSystem;

    return audioSystem;
}

void AudioSystem::SetBackend(AudioBackend* audioBackend) 
{
    m_pAudioBackend = audioBackend;
}

AudioHandle AudioSystem::Play(const std::string& resourceId, AudioPlayOptions options)
{
    if (options.spatial)
        LoggerSingleton::Instance().LogWarning("AudioSystem::Play -> Cannot play spatial audio '" + resourceId + "' without an entity. Falling back to non-spatial playback.");

    options.spatial = false;

    return Play(-1, resourceId, options);
}

AudioHandle AudioSystem::Play(unsigned int entityId, std::string resourceId, AudioPlayOptions options)
{
    AudioClip* clip = ResourceManager::Instance().GetAudio(resourceId);

    if (clip == nullptr)
        return {};

    AudioBackendHandle backendHandle = m_pAudioBackend->Play(*clip, options);

    if (!backendHandle)
        return {};

    const uint32_t index = AcquireVoice();

    AudioVoice& voice = m_voices[index];

    voice.audioHandle = backendHandle;
    voice.entityId = entityId;
    voice.active = true;
    voice.spatial = options.spatial;
    voice.bus = options.bus;
    voice.state = AudioVoiceState::Playing;

    // Global sound has no emitter contribution.
    if (entityId < 0)
        RefreshVoiceVolume(voice);

    return {index, voice.generation};
}

void AudioSystem::Stop(AudioHandle handle) 
{
    AudioVoice* voice = GetVoice(handle);

    if (voice == nullptr)
        return;

    m_pAudioBackend->Stop(voice->audioHandle);
    m_pAudioBackend->Destroy(voice->audioHandle);

    ReleaseVoice(handle.id);
}

void AudioSystem::Pause(AudioHandle handle) 
{
    AudioVoice* voice = GetVoice(handle);

    if (voice == nullptr)
        return;

    m_pAudioBackend->Pause(voice->audioHandle);
    voice->state = AudioVoiceState::Paused;
}

void AudioSystem::Resume(AudioHandle handle) 
{
    AudioVoice* voice = GetVoice(handle);

    if (voice == nullptr)
        return;

    m_pAudioBackend->Resume(voice->audioHandle);
    voice->state = AudioVoiceState::Playing;
}

void AudioSystem::SetVolume(AudioHandle handle, float volume) 
{
    AudioVoice* voice = GetVoice(handle);

    if (voice == nullptr)
        return;

    m_pAudioBackend->SetVolume(voice->audioHandle, volume);
}

void AudioSystem::SetPitch(AudioHandle handle, float pitch) 
{
    AudioVoice* voice = GetVoice(handle);

    if (voice == nullptr)
        return;

    m_pAudioBackend->SetPitch(voice->audioHandle, pitch);
}

void AudioSystem::SetBusVolume(AudioBus bus, float volume) 
{
    const size_t index = static_cast<size_t>(bus);

    if (index >= m_busVolumes.size())
        return;

    volume = std::clamp(volume, 0.0f, 1.0f);

    m_busVolumes[index] = volume;

    for (AudioVoice& voice : m_voices)
    {
        if (!voice.active || voice.bus != bus)
            continue;

        m_pAudioBackend->SetVolume(voice.audioHandle, volume);
        // For entity voices, we need their emitter volume.
        // More on this below.
        RefreshVoiceVolume(voice);
    }
}

void AudioSystem::Update(Scene& scene) 
{
    bool hasListener = false;

    for (Entity* entity : scene.GetEntityManager()->GetEntities())
    {
        if (UpdateListener(*entity))
            hasListener = true;

        UpdateEmitter(*entity);
    }

    UpdateSpatialVoices(hasListener);

    CleanupFinishedVoices();
}

void AudioSystem::StopByEntity(unsigned int entityId)
{
    for (uint32_t i = 0; i < m_voices.size(); ++i)
    {
        AudioVoice& voice = m_voices[i];

        if (!voice.active)
            continue;

        if (voice.entityId != entityId)
            continue;

        m_pAudioBackend->Stop(voice.audioHandle);
        m_pAudioBackend->Destroy(voice.audioHandle);

        ReleaseVoice(i);
    }
}

void AudioSystem::StopAll()
{
    for (uint32_t i = 0; i < m_voices.size(); ++i)
    {
        AudioVoice& voice = m_voices[i];

        if (!voice.active)
            continue;

        m_pAudioBackend->Stop(voice.audioHandle);
        m_pAudioBackend->Destroy(voice.audioHandle);

        ReleaseVoice(i);
    }
}

void AudioSystem::RefreshVoiceVolume(AudioVoice& voice)
{
    const float busVolume = m_busVolumes[static_cast<size_t>(voice.bus)];
    const float finalVolume = voice.volume * voice.emitterVolume * busVolume;

    m_pAudioBackend->SetVolume(voice.audioHandle, finalVolume);
}

bool AudioSystem::UpdateListener(Entity& entity)
{
    auto& listener = entity.GetComponent<AudioListenerComponent>();

    if (!listener.has || !listener.enabled)
        return false;

    auto& transform = entity.GetComponent<TransformComponent>();

    m_pAudioBackend->SetListener(transform.GetPosition(), transform.GetForward(), transform.GetUp());

    return true;
}

void AudioSystem::UpdateEmitter(Entity& entity)
{
    auto& emitter = entity.GetComponent<AudioEmitterComponent>();

    if (!emitter.has || !emitter.enabled)
        return;

    auto& transform = entity.GetComponent<TransformComponent>();

    for (AudioVoice& voice : m_voices)
    {
        if (!voice.active)
            continue;

        if (voice.entityId < 0)
            continue;

        if (!voice.spatial)
            continue;

        if (voice.entityId != entity.GetId())
            continue;

        m_pAudioBackend->SetPosition(voice.audioHandle, transform.GetPosition());

        m_pAudioBackend->SetMinDistance(voice.audioHandle, emitter.minDistance);

        m_pAudioBackend->SetMaxDistance(voice.audioHandle, emitter.maxDistance);

        voice.emitterVolume = emitter.volume;

        RefreshVoiceVolume(voice);
    }
}

void AudioSystem::UpdateSpatialVoices(const bool hasListener)
{
    for (AudioVoice& voice : m_voices)
    {
        if (!voice.active || !voice.spatial)
            continue;

        voice.volume = hasListener ? 1.0f : 0.0f;

        RefreshVoiceVolume(voice);
    }
}

void AudioSystem::CleanupFinishedVoices() 
{
    for (uint32_t i = 0; i < m_voices.size(); ++i)
    {
        AudioVoice& voice = m_voices[i];

        if (!voice.active)
            continue;

        if (voice.state == AudioVoiceState::Paused)
            continue;

        if (m_pAudioBackend->IsPlaying(voice.audioHandle))
            continue;

        m_pAudioBackend->Destroy(voice.audioHandle);

        ReleaseVoice(i);
    }
}

AudioVoice* AudioSystem::GetVoice(AudioHandle handle)
{
    if (handle.id >= m_voices.size())
        return nullptr;

    AudioVoice& voice = m_voices[handle.id];

    if (!voice.active)
        return nullptr;

    if (voice.generation != handle.generation)
        return nullptr;

    return &voice;
}

uint32_t AudioSystem::AcquireVoice()
{
    if (!m_freeVoices.empty())
    {
        const uint32_t index = m_freeVoices.back();
        m_freeVoices.pop_back();

        return index;
    }

    const uint32_t index = static_cast<uint32_t>(m_voices.size());

    m_voices.emplace_back();

    return index;
}

void AudioSystem::ReleaseVoice(uint32_t index) 
{
    AudioVoice& voice = m_voices[index];

    voice.audioHandle = {};
    voice.active = false;
    voice.entityId = -1;

    ++voice.generation;

    m_freeVoices.push_back(index);
}
