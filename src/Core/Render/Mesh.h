#ifndef CORE_RENDER_MESH_H_
#define CORE_RENDER_MESH_H_

#include <string>
#include <vector>
#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

namespace DreamEngine::Core::Render
{
class Mesh : public Resource
{
   public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;
    unsigned int VAO;

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const std::vector<Texture*>& textures)
        : vertices(std::move(vertices)),
          indices(std::move(indices)),
          textures(std::move(textures)),
          VAO(0),
          m_VBO(0),
          m_EBO(0) {}
    virtual ~Mesh() = default;
    virtual void Draw(Shader& shader) = 0;

   protected:
    //  render data
    unsigned int m_VBO, m_EBO;

    virtual void SetupMesh() = 0;
};
}  // namespace DreamEngine::Core::Render
#endif