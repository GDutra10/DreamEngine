#include "MiniaudioBackend.h"

using namespace DreamEngine::Core::Audio::Backends;

MiniaudioBackend::~MiniaudioBackend() 
{
    UnloadClips();

    if (m_engineInitialized)
    {
        ma_engine_uninit(&m_engine);
        m_engineInitialized = false;
    }

    if (m_resourceManagerInitialized)
    {
        ma_resource_manager_uninit(&m_resourceManager);
        m_resourceManagerInitialized = false;
    }
}

bool MiniaudioBackend::HandleInitialize()
{
    ma_resource_manager_config resourceConfig = ma_resource_manager_config_init();
    ma_result result = ma_resource_manager_init(&resourceConfig, &m_resourceManager);

    if (result != MA_SUCCESS)
        return false;

    m_resourceManagerInitialized = true;

    ma_engine_config engineConfig = ma_engine_config_init();
    engineConfig.pResourceManager = &m_resourceManager;
    result = ma_engine_init(&engineConfig, &m_engine);

    if (result != MA_SUCCESS)
    {
        m_resourceManagerInitialized = false;        
        ma_resource_manager_uninit(&m_resourceManager);

        return false;
    }

    m_engineInitialized = true;

    return true;
}

AudioBackendHandle MiniaudioBackend::HandlePlay(AudioClip& audio, AudioPlayOptions& options)
{
    if (!m_engineInitialized)
        return {};

    const CachedClip* cached = GetOrLoadClip(audio);

    if (cached == nullptr)
        return {};

    Voice* voice = new Voice();
    ma_uint32 flags = 0;

    if (!options.spatial)
        flags |= MA_SOUND_FLAG_NO_SPATIALIZATION;

    const ma_result result = ma_sound_init_from_file(&m_engine, cached->name.c_str(), flags, nullptr, nullptr, &voice->sound);

    if (result != MA_SUCCESS)
    {
        delete voice;
        return {};
    }

    voice->initialized = true;

    ma_sound_set_volume(&voice->sound, options.volume);
    ma_sound_set_pitch(&voice->sound, options.pitch);
    ma_sound_set_looping(&voice->sound, options.loop ? MA_TRUE : MA_FALSE);

    if (ma_sound_start(&voice->sound) != MA_SUCCESS)
    {
        ma_sound_uninit(&voice->sound);
        delete voice;

        return {};
    }

    return {.ptr = voice};
}

void MiniaudioBackend::HandleStop(AudioBackendHandle handle) 
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr || !voice->initialized)
        return;

    ma_sound_stop(&voice->sound);
    ma_sound_seek_to_pcm_frame(&voice->sound, 0);
}

void MiniaudioBackend::HandlePause(AudioBackendHandle handle) 
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr || !voice->initialized)
        return;

    ma_sound_stop(&voice->sound);
}

void MiniaudioBackend::HandleResume(AudioBackendHandle handle) 
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr || !voice->initialized)
        return;

    ma_sound_start(&voice->sound);
}

void MiniaudioBackend::HandleSetVolume(AudioBackendHandle handle, float volume) 
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr || !voice->initialized)
        return;

    ma_sound_set_volume(&voice->sound, volume);
}

bool MiniaudioBackend::HandleIsPlaying(AudioBackendHandle handle) const
{
    const Voice* voice = static_cast<const Voice*>(handle.ptr);

    if (voice == nullptr || !voice->initialized)
        return false;

    return ma_sound_is_playing(const_cast<ma_sound*>(&voice->sound)) == MA_TRUE;
}

void MiniaudioBackend::HandleDestroy(AudioBackendHandle handle) 
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr)
        return;

    if (voice->initialized)
    {
        ma_sound_uninit(&voice->sound);
        voice->initialized = false;
    }

    delete voice;
}

void MiniaudioBackend::HandleSetPitch(AudioBackendHandle handle, float pitch) 
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr || !voice->initialized)
        return;

    ma_sound_set_pitch(&voice->sound, pitch);
}

void MiniaudioBackend::HandleSetPosition(AudioBackendHandle handle, glm::vec3 position) 
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr || !voice->initialized)
        return;

    ma_sound_set_position(&voice->sound, position.x, position.y, position.z);
}

void MiniaudioBackend::HandleSetListener(glm::vec3 position, glm::vec3 direction, glm::vec3 up) 
{
    ma_engine_listener_set_position(&m_engine, 0, position.x, position.y, position.z);
    ma_engine_listener_set_direction(&m_engine, 0, direction.x, direction.y, direction.z);
    ma_engine_listener_set_world_up(&m_engine, 0, up.x, up.y, up.z);
}

void MiniaudioBackend::HandleSetMinDistance(AudioBackendHandle handle, float distance)
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr || !voice->initialized)
        return;

    ma_sound_set_min_distance(&voice->sound, distance);
}

void MiniaudioBackend::HandleSetMaxDistance(AudioBackendHandle handle, float distance)
{
    Voice* voice = GetVoice(handle);

    if (voice == nullptr || !voice->initialized)
        return;

    ma_sound_set_max_distance(&voice->sound, distance);
}


MiniaudioBackend::CachedClip* MiniaudioBackend::GetOrLoadClip(const AudioClip& clip)
{
    auto it = m_cachedClips.find(&clip);

    if (it != m_cachedClips.end())
        return &it->second;

    const auto& data = clip.data;

    if (data.empty())
        return nullptr;

    CachedClip cached;

    cached.clip = &clip;
    cached.name = clip.resourceId;

    const ma_result result = ma_resource_manager_register_encoded_data(&m_resourceManager, cached.name.c_str(), data.data(), data.size());

    if (result != MA_SUCCESS)
        return nullptr;

    auto [inserted, success] = m_cachedClips.emplace(&clip, std::move(cached));

    if (!success)
        return nullptr;

    return &inserted->second;
}

void MiniaudioBackend::UnloadClips() 
{
    if (!m_resourceManagerInitialized)
        return;

    for (auto& [clip, cached] : m_cachedClips)
    {
        ma_resource_manager_unregister_data(&m_resourceManager, cached.name.c_str());
    }

    m_cachedClips.clear();
}
