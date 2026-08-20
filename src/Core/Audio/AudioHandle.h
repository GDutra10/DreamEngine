#ifndef CORE_AUDIO_AUDIO_HANDLE_H_
#define CORE_AUDIO_AUDIO_HANDLE_H_

#include <vector>

#include "../CoreCApi.h"

namespace DreamEngine::Core::Audio
{
struct CORE_API AudioHandle
{
    unsigned int id = UINT32_MAX;
    unsigned int generation = 0;

    explicit operator bool() const noexcept { return id != UINT32_MAX; }
};
}  // namespace DreamEngine::Core::Audio
#endif