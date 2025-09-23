#ifndef CORE_RENDER_OPEN_GL_OPEN_GL_FRAME_BUFFER_H_
#define CORE_RENDER_OPEN_GL_OPEN_GL_FRAME_BUFFER_H_

#include "../FrameBuffer.h"

namespace DreamEngine::Core::Render::OpenGL
{

class OpenGLFrameBuffer : public FrameBuffer
{
   public:
    OpenGLFrameBuffer(const int width, const int height, unsigned int rbo);
    void Rescale(const int width, const int height) override;
    void Attach() override;
    void Detach() override;
    bool GetIsActive() override;
};

}  // namespace DreamEngine::Core::Render::OpenGL
#endif