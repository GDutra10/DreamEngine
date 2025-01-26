#include "OpenGLMesh.h"

#include "../../Loggers/LoggerSingleton.h"
#include "glad/glad.h"

using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::Render::OpenGL;


OpenGLMesh::OpenGLMesh(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    const std::vector<Texture*>& textures)
    : Mesh(vertices, indices, textures)
{
}

OpenGLMesh::~OpenGLMesh()
{
    // Delete the buffers and array objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);

    // If textures are owned by this class, delete them as well
    for (Texture* texture : textures)
    {
        if (texture != nullptr)
        {
            glDeleteTextures(1, &texture->id);
            delete texture;  // Assuming textures are dynamically allocated
        }
    }
}

void OpenGLMesh::Draw(Shader& shader)
{
    if (!m_isSetup)
    {
        SetupMesh();
        m_isSetup = true;
    }

    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        Texture* texture = textures[i];

        if (texture == nullptr)
            continue;

        // active proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);  

        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name;

        switch (texture->type)
        {
            case TextureType::Diffuse:
                number = std::to_string(diffuseNr++);
                name = "texture_diffuse";
                break;
            case TextureType::Specular:
                number = std::to_string(specularNr++);  // transfer unsigned int to string
                name = "texture_specular";
                break;
            case TextureType::Normal:
                number = std::to_string(normalNr++);  // transfer unsigned int to string
                name = "texture_normal";
                break;
            case TextureType::Height:
                number = std::to_string(heightNr++);  // transfer unsigned int to string
                name = "texture_height";
                break;
            default:
                Loggers::LoggerSingleton::Instance().LogWarning("Texture type not recognized!");
                break;
        }

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.id, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void OpenGLMesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    if (!vertices.empty())
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    if (!indices.empty())
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
