#ifndef CORE_RENDER_RENDER_VIEW_H_
#define CORE_RENDER_RENDER_VIEW_H_

#include <cstdint>

#include "CoreExport.h"
#include "FrameBuffer.h"

namespace DreamEngine::Core::Render
{

struct RenderMask
{
    constexpr static uint32_t World = 1 << 0;
    constexpr static uint32_t UI = 1 << 1;
    constexpr static uint32_t Debug = 1 << 2;
};

struct CORE_API RenderView
{
    uint32_t mask = 0;
    FrameBuffer* frameBuffer = nullptr;
};
}  // namespace DreamEngine::Core::Render
#endif