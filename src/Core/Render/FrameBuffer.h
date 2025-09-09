#ifndef CORE_RENDER_FRAME_BUFFER_H_
#define CORE_RENDER_FRAME_BUFFER_H_

#include "CoreExport.h"

namespace DreamEngine::Core::Render
{
class CORE_API FrameBuffer
{
   public:
    unsigned int id = 0;
    unsigned int textureColorBuffer = 0;
    FrameBuffer(const int width, const int height, unsigned int rbo) : m_renderBufferId(rbo) {};
    ~FrameBuffer() = default;
    virtual void Rescale(const int width, const int height) = 0;
   protected:
    unsigned int m_renderBufferId = 0;
};
}  // namespace DreamEngine::Core::Render
#endif