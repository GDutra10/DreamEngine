#ifndef CORE_RENDER_OPEN_GL_OPEN_GL_MESH_H_
#define CORE_RENDER_OPEN_GL_OPEN_GL_MESH_H_

#include "../../ECS/Entity.h"
#include "../RenderAPI.h"
#include "../Texture.h"

namespace DreamEngine::Core::Render::OpenGL
{
using namespace DreamEngine::Core::ECS;

class OpenGLMesh : public Mesh
{
   public:
    OpenGLMesh(const std::vector<Vertex>& vertices,
               const std::vector<unsigned int>& indices,
               const std::vector<Texture*>& textures);
    ~OpenGLMesh() override;
    void Draw(Shader& shader) override;

   private:
       bool m_isSetup = false;
	   void SetupMesh() override;
};

}  // namespace DreamEngine::Core::Render::OpenGL
#endif