#ifndef CORE_AUDIO_AUDIO_BACKEND_HANDLE_H_
#define CORE_AUDIO_AUDIO_BACKEND_HANDLE_H_

#include <vector>

namespace DreamEngine::Core::Audio
{
struct AudioBackendHandle
{
    void* ptr;

    explicit operator bool() const noexcept { return ptr != nullptr; }
};
}  // namespace DreamEngine::Core::Audio
#endif