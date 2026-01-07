#ifndef CORE_RENDER_OPEN_GL_OPEN_GL_RENDER_API_H_
#define CORE_RENDER_OPEN_GL_OPEN_GL_RENDER_API_H_

#include "../Texture.h"
#include "../RenderAPI.h"
#include "glad/glad.h"

namespace DreamEngine::Core::Render::OpenGL
{
using namespace DreamEngine::Core::ECS;

class OpenGLRenderAPI : public RenderAPI
{
   public:
    void Initialize(int width, int height) override;
    Shader* CreateShader(std::string name, const std::string& vertexCode, const std::string& fragmentCode) override;
    Texture* CreateTexture(unsigned char* data, int width, int height, int nrChannels) override;
    Mesh* CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture*>& textures) override;
    FrameBuffer* CreateFrameBuffer(int width, int height) override;
    void AfterRender(int width, int height) override;
    void OutlineBeginPass(const OutlineOptions& options) override;
    void OutlineEndPass() override;

   protected:
    unsigned int m_rbo = 0;
    void BeforeRender() override;
    void SetSceneBackgroundColor(Color* color) override;
    void SetTransform(const Shader* shader, const std::string name, glm::mat4& transform) override;
    void StencilDefaultNoWrite() override;
    void StencilWriteObject() override;
    void StencilDrawOutlineRegion() override;

   private:
    struct GLState
    {
        GLboolean depthEnabled{};
        GLboolean cullEnabled{};
        GLint cullMode{};
    };
    std::vector<GLState> m_stateStack;
};
}  // namespace DreamEngine::Core::Render
#endif