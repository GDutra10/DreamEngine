#include "OpenGLRenderAPI.h"

#include <iostream>

#include "OpenGLMesh.h"
#include "OpenGLShader.h"
#include "OpenGLFrameBuffer.h"
#include "../../Vendors/glad.h"
#include "../../ECS/Components/TransformComponent.h"
#include "../../ECS/Components/MeshComponent.h"
#include "../../Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::Render::OpenGL;

enum compileType
{
    program,
    vertexShader,
    fragmentShader
};

std::string toString(compileType type);

std::string& checkCompileErrorsByShader(const std::string& name, unsigned int shaderId, const compileType type);

void OpenGLRenderAPI::Initialize(int width, int height)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // Disable face culling for debugging
    //glDisable(GL_CULL_FACE);

    // add stencil for object outlining
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glFrontFace(GL_CCW); 
    glCullFace(GL_BACK);
}

Shader* OpenGLRenderAPI::CreateShader(const std::string name, const std::string& vertexCode, const std::string& fragmentCode)
{
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // vertex shader
    const unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    std::string& vertexError = checkCompileErrorsByShader(name, vertex, vertexShader);

    // fragment shader
    const unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    std::string& fragmentError = checkCompileErrorsByShader(name, fragment, fragmentShader);

    // shader program
    OpenGLShader* shader = new OpenGLShader(glCreateProgram());
    glAttachShader(shader->id, vertex);
    glAttachShader(shader->id, fragment);
    glLinkProgram(shader->id);
    std::string& programError = checkCompileErrorsByShader(name, shader->id, program);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    shader->hasErrors = !vertexError.empty() ||  !fragmentError.empty() || !programError.empty();
    shader->name = name;

    return shader;
}

Texture* OpenGLRenderAPI::CreateTexture(unsigned char* data, int width, int height, int nrChannels)
{
    Texture* texture = new Texture(data);

    glGenTextures(1, &texture->id);

    // load texture
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else  // if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture->id);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return texture;
    }

    Loggers::LoggerSingleton::Instance().LogError("Failed to load texture");

    return nullptr;
}

Mesh* OpenGLRenderAPI::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture*>& textures)
{
    return new OpenGLMesh(vertices, indices, textures);
}

FrameBuffer* OpenGLRenderAPI::CreateFrameBuffer(int width, int height)
{
    FrameBuffer* frameBuffer = new OpenGLFrameBuffer(width, height, m_rbo);
    m_frameBuffers.push_back(frameBuffer);

    return frameBuffer;
}

void OpenGLRenderAPI::AfterRender(int width, int height)
{
    glViewport(0, 0, width, height);

    GLenum err;

    while ((err = glGetError()) != GL_NO_ERROR)
        Loggers::LoggerSingleton::Instance().LogError("OpenGLRenderAPI::AfterRender -> OpenGL error: " + std::to_string(err));

    RenderAPI::AfterRender(width, height);
}

void OpenGLRenderAPI::OutlineBeginPass(const OutlineOptions& options) 
{
    // Save current
    GLState glState{};
    glState.depthEnabled = glIsEnabled(GL_DEPTH_TEST);
    glState.cullEnabled = glIsEnabled(GL_CULL_FACE);
    glGetIntegerv(GL_CULL_FACE_MODE, &glState.cullMode);
    m_stateStack.push_back(glState);

    if (options.disableDepthTest)
        glDisable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(options.cullFace == OutlineOptions::CullFace::Front ? GL_FRONT : GL_BACK);
}

void OpenGLRenderAPI::OutlineEndPass() 
{
    if (m_stateStack.empty())
        return;
    const GLState glState = m_stateStack.back();
    m_stateStack.pop_back();

    glCullFace(glState.cullMode);
    glState.cullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    glState.depthEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void OpenGLRenderAPI::BeforeRender()
{
    // Clear default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (FrameBuffer* frameBuffer : m_frameBuffers)
    {
        if (frameBuffer == nullptr)
            continue;

        frameBuffer->Attach();
        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        frameBuffer->Detach();
    }

    RenderAPI::BeforeRender();
}

void OpenGLRenderAPI::SetSceneBackgroundColor(Color* color)
{
    glClearColor(color->red, color->green, color->blue, color->alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAPI::SetTransform(const Shader* shader, const std::string name, glm::mat4& transform)
{
    const unsigned int transformLoc = glGetUniformLocation(shader->id, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void OpenGLRenderAPI::StencilDefaultNoWrite()
{
    /*glDisable(GL_POLYGON_OFFSET_FILL);
    glDepthMask(GL_TRUE);*/

    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilMask(0x00);
}

void OpenGLRenderAPI::StencilWriteObject()
{
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}

void OpenGLRenderAPI::StencilDrawOutlineRegion()
{
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);

    /*glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.f, 1.f);*/
}

std::string& checkCompileErrorsByShader(const std::string& name, const unsigned int shaderId, const compileType type)
{
    int success;
    char infoLog[1024];
    std::string logMessage;

    if (type == compileType::program)
    {
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);

        if (!success)
            glGetProgramInfoLog(shaderId, 1024, nullptr, infoLog);
    }
    else
    {
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

        if (!success)
            glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
    }

    if (!success)
    {
        logMessage.append("ERROR::SHADER_COMPILATION_ERROR");
        logMessage.append(" '");
        logMessage.append(name);
        logMessage.append("' ");
        logMessage.append(" of type: ");
        logMessage.append(toString(type));
        logMessage.append(" ");
        logMessage.append(infoLog);
        DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogError(logMessage);
    }
    else
    {
        DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogInfo(
            "Shader '" + name + "' (" + toString(type) + ") compiled successfully");
    }

    return logMessage;
}

std::string toString(const compileType type)
{
    switch (type)
    {
        case program:
            return "PROGRAM";
        case fragmentShader:
            return "FRAGMENT_SHADER";
        case vertexShader:
            return "VERTEX_SHADER";
        default:
            return "UNRECOGNIZED";
    }
}