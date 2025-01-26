#ifndef CORE_RENDER_RENDER_API_H_
#define CORE_RENDER_RENDER_API_H_

#include <functional>

#include "Mesh.h"
#include "Texture.h"
#include "../ECS/Entity.h"
#include "../GameSystem/Game.h"

namespace DreamEngine::Core::Render
{
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::GameSystem;

class RenderAPI
{
   public:
    unsigned int fbo;
    unsigned int texColorBuffer;
    virtual ~RenderAPI() = default;
    virtual void Initialize(int width, int height) = 0;
    virtual Shader* CreateShader(std::string name, const std::string& vertexCode, const std::string& fragmentCode) = 0;
    virtual Texture* CreateTexture(unsigned char* data, int width, int height, int nrChannels) = 0;
    virtual Mesh* CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture*>& textures) = 0;
    void Render(Game* game);
    virtual void AfterRender(int width, int height);
    void AddBeforeRenderEntitiesCallbacks(const std::function<void()>& callback);
    void AddAfterRenderEntitiesCallbacks(const std::function<void(int width, int height)>& callback);
    void AddAfterRenderEntityCallbacks(const std::function<void(Entity* entity)>& callback);
    virtual void RescaleFrameBuffer(int width, int height) const = 0;

   protected:
    virtual void BeforeRender();
    virtual void SetSceneBackgroundColor(Color* color) = 0;
    virtual void SetTransform(const Shader* shader, const std::string name, glm::mat4& transform) = 0;
   private:
    std::vector<std::function<void()>> m_beforeRenderEntitiesCallbacks;
    std::vector<std::function<void(int width, int height)>> m_afterRenderEntitiesCallbacks;
    std::vector<std::function<void(Entity* entity)>> m_afterRenderEntityCallbacks;
};

}  // namespace DreamEngine::Core::Render
#endif