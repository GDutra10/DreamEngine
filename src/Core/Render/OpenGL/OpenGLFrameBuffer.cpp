#include "OpenGLFrameBuffer.h"


#include "../../Loggers/LoggerSingleton.h"
#include "glad/glad.h"

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::Render::OpenGL;

OpenGLFrameBuffer::OpenGLFrameBuffer(const int width, const int height, unsigned int rbo) : FrameBuffer(width, height, rbo) 
{
    // Create a framebuffer object
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    // Create a texture to render to viewport
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    // https://uysalaltas.github.io/2022/01/09/OpenGL_Imgui.html
    // Create a render buffer object, keep rendering data with performance
    glGenRenderbuffers(1, &m_renderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Loggers::LoggerSingleton::Instance().LogError("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

    // Unbind buffers
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void OpenGLFrameBuffer::Rescale(const int width, const int height) 
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    glViewport(0, 0, width, height);

    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind any existing texture
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferId);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLenum err = glGetError();

    if (err != GL_NO_ERROR)
        Loggers::LoggerSingleton::Instance().LogError("OpenGLFrameBuffer::Rescale -> OpenGL error: " + std::to_string(err));
}

void OpenGLFrameBuffer::Attach()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void OpenGLFrameBuffer::Detach()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool OpenGLFrameBuffer::GetIsActive()
{
    GLuint drawFbo = 0, readFbo = 0;

    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, reinterpret_cast<GLint*>(&drawFbo));
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, reinterpret_cast<GLint*>(&readFbo));

    return drawFbo == id && readFbo == id;
}
