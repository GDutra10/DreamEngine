#include "ResourceManager.h"

#include "../Helpers/GUIDHelper.h"

using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Helpers;


void ResourceManager::AddShader(Shader* shader)
{
    AddShader(GUIDHelper::GenerateGUID(), shader);
}

void ResourceManager::AddShader(const std::string& resourceId, Shader* shader)
{
    m_shaders.try_emplace(resourceId, shader);
    shader->resourceId = resourceId;
}

void ResourceManager::AddMaterial(Material* material)
{
    AddMaterial(GUIDHelper::GenerateGUID(), material);
}

void ResourceManager::AddMaterial(const std::string& resourceId, Material* material)
{
    m_materials.try_emplace(resourceId, material);
    material->resourceId = resourceId;
}

void ResourceManager::AddTexture(Texture* texture)
{
    AddTexture(GUIDHelper::GenerateGUID(), texture);
}

void ResourceManager::AddTexture(const std::string& resourceId, Texture* texture)
{
    m_textures.try_emplace(resourceId, texture);
    texture->resourceId = resourceId;
}

void ResourceManager::AddMesh(Mesh* mesh)
{
    AddMesh(GUIDHelper::GenerateGUID(), mesh);
}

void ResourceManager::AddMesh(const std::string& resourceId, Mesh* mesh)
{
    m_meshes.try_emplace(resourceId, mesh);
    mesh->resourceId = resourceId;
}

void ResourceManager::AddScript(Script* script)
{
    AddScript(GUIDHelper::GenerateGUID(), script);
}

void ResourceManager::AddScript(const std::string& resourceId, Script* script)
{
    m_scripts.try_emplace(resourceId, script);
    script->resourceId = resourceId;
}

void ResourceManager::Clear()
{
    for (auto& it : m_materials)
        delete it.second;

    for (auto& it : m_meshes)
        delete it.second;

    for (auto& it : m_scripts)
        delete it.second;

    for (auto& it : m_shaders)
        delete it.second;

    for (auto& it : m_textures)
        delete it.second;

    m_materials.clear();
    m_meshes.clear();
    m_scripts.clear();
    m_shaders.clear();
    m_textures.clear();
}

void ResourceManager::RemoveMaterial(const Material* material)
{
    m_materials.erase(material->resourceId);
    delete material;
}

void ResourceManager::RemoveScript(const Script* script)
{
    m_scripts.erase(script->resourceId);
    delete script;
}

Shader* ResourceManager::GetShader(const std::string& resourceId)
{
    auto it = m_shaders.find(resourceId);

    if (it != m_shaders.end())
    {
        return it->second;
    }

    return nullptr;
}

Material* ResourceManager::GetMaterial(const std::string& resourceId)
{
    auto it = m_materials.find(resourceId);

    if (it != m_materials.end())
    {
        return it->second;
    }

    return nullptr; 
}

Texture* ResourceManager::GetTexture(const std::string& resourceId)
{
    auto it = m_textures.find(resourceId);

    if (it != m_textures.end())
    {
        return it->second;
    }

    return nullptr;
}

Mesh* ResourceManager::GetMesh(const std::string& resourceId)
{
    auto it = m_meshes.find(resourceId);

    if (it != m_meshes.end())
    {
        return it->second;
    }

    return nullptr;
}

Script* ResourceManager::GetScript(const std::string& resourceId)
{
    auto it = m_scripts.find(resourceId);

    if (it != m_scripts.end())
    {
        return it->second;
    }

    return nullptr;
}

std::map<std::string, Shader*>& ResourceManager::GetShaders()
{
    return m_shaders;
}

std::map<std::string, Material*>& ResourceManager::GetMaterials()
{
    return m_materials;
}

std::map<std::string, Texture*>& ResourceManager::GetTextures()
{
    return m_textures;
}

std::map<std::string, Mesh*>& ResourceManager::GetMeshes()
{
    return m_meshes;
}

std::map<std::string, Script*>& ResourceManager::GetScripts()
{
    return m_scripts;
}