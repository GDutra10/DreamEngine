#include "AssimpModelImporter.h"

#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application.h"
#include "../Vendors/stb_image.h"
#include "Loggers/LoggerSingleton.h"
#include "Render/OpenGL/OpenGLMesh.h"

using namespace DreamEngine::Editor::Importers;
using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Loggers;

Model& AssimpModelImporter::Import(std::string const& path, bool gamma)
{
    m_model = {};
    m_path = path;
    m_directory = std::filesystem::path(path).parent_path().string();
    m_texturePaths = {};

    std::ranges::replace(m_path, '\\', '/');

    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  // if is Not Zero
    {
        std::string error = importer.GetErrorString();
        LoggerSingleton::Instance().LogError("AssimpModelImporter::Import -> Assimp error: " + error);

        return m_model;
    }
    // retrieve the directory path of the filepath
    //m_directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);

    return m_model;
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void AssimpModelImporter::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_model.meshes.push_back(ProcessMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh* AssimpModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;
    
    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3
            vector;  // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0])  // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture*> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture*> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::Height);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture*> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::Normal);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    Mesh* meshFromRenderAPI =  Application::Instance().GetRenderAPI()->CreateMesh(vertices, indices, textures);
    meshFromRenderAPI->name = mesh->mName.C_Str();

    return meshFromRenderAPI;
}

std::vector<Texture*> AssimpModelImporter::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType)
{
    std::vector<Texture*> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString pathFromAssimp;

        mat->GetTexture(type, i, &pathFromAssimp);

        std::string path = m_directory + '/' + std::string(pathFromAssimp.C_Str());

        // is already loaded
        if (std::ranges::find(m_texturePaths, path) != m_texturePaths.end())
            continue;

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        if (data != nullptr)
        {
            Texture* texture = Application::Instance().GetRenderAPI()->CreateTexture(data, width, height, nrComponents);
            texture->type = textureType;
            texture->name = pathFromAssimp.C_Str();
            texture->path = path;
            textures.push_back(texture);
            m_model.textures.push_back(texture);
            m_texturePaths.emplace_back(path);

            stbi_image_free(data);
        }
        else
            LoggerSingleton::Instance().LogWarning("AssimpModelImporter::LoadMaterialTextures -> Failed to load texture from path: " + m_path +
                                                 ".\r\nstbi failure reason: " + stbi_failure_reason());    
    }

    return textures;
}
