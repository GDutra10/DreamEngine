#include "ResourceManager.h"

#include "../Helpers/GUIDHelper.h"

using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Helpers;


ResourceManager& ResourceManager::Instance()
{
    static ResourceManager globalResourceManager;

    return globalResourceManager;
}

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

void ResourceManager::AddUiContent(UiContent* uiContent)
{
    AddUiContent(GUIDHelper::GenerateGUID(), uiContent);
}

void ResourceManager::AddUiContent(const std::string& resourceId, UiContent* uiContent)
{
    m_uiContents.try_emplace(resourceId, uiContent);
    uiContent->resourceId = resourceId;
}

void ResourceManager::AddFont(Font* font)
{
    AddFont(GUIDHelper::GenerateGUID(), font);
}

void ResourceManager::AddFont(const std::string& resourceId, Font* font)
{
    m_fonts.try_emplace(resourceId, font);
    font->resourceId = resourceId;
}

void ResourceManager::AddPrefab(Prefab* prefab) 
{
    AddPrefab(GUIDHelper::GenerateGUID(), prefab);
}

void ResourceManager::AddPrefab(const std::string& resourceId, Prefab* prefab) 
{
    m_prefabs.try_emplace(resourceId, prefab);
    prefab->resourceId = resourceId;
}

void ResourceManager::AddAudio(AudioClip* audio)
{
    AddAudio(GUIDHelper::GenerateGUID(), audio);
}

void ResourceManager::AddAudio(const std::string& resourceId, AudioClip* audio)
{
    m_audios.try_emplace(resourceId, audio);
    audio->resourceId = resourceId;
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

    for (auto& it : m_uiContents)
        delete it.second;

    for (auto& it : m_prefabs)
        delete it.second;

    m_materials.clear();
    m_meshes.clear();
    m_scripts.clear();
    m_shaders.clear();
    m_textures.clear();
    m_uiContents.clear();
    m_prefabs.clear();
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

void ResourceManager::RemoveUiContent(const UiContent* uiContent)
{
    m_uiContents.erase(uiContent->resourceId);
    delete uiContent;
}

void ResourceManager::RemovePrefab(const Prefab* prefab)
{
    m_prefabs.erase(prefab->resourceId);
    delete prefab;
}

void ResourceManager::RemoveAudio(const AudioClip* audio)
{
    m_audios.erase(audio->resourceId);
    delete audio;
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

Script* ResourceManager::GetScript(const std::string& className, const std::string& assemblyName)
{
    for (auto& [resourceId, script] : m_scripts)
    {
        if (script != nullptr && 
            script->GetClassName() == className && 
            script->GetAssemblyName() == assemblyName)
        {
            return script;
        }
    }

    return nullptr;
}

UiContent* ResourceManager::GetUiContent(const std::string& resourceId)
{
    auto it = m_uiContents.find(resourceId);

    if (it != m_uiContents.end())
    {
        return it->second;
    }

    return nullptr;
}

Font* ResourceManager::GetFont(const std::string& resourceId)
{
    auto it = m_fonts.find(resourceId);

    if (it != m_fonts.end())
    {
        return it->second;
    }

    return nullptr;
}

Prefab* ResourceManager::GetPrefab(const std::string& resourceId) 
{
    auto it = m_prefabs.find(resourceId);

    if (it != m_prefabs.end())
    {
        return it->second;
    }

    return nullptr;
}

AudioClip* ResourceManager::GetAudio(const std::string& resourceId)
{
    auto it = m_audios.find(resourceId);

    if (it != m_audios.end())
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

std::map<std::string, UiContent*>& ResourceManager::GetUiContents()
{
    return m_uiContents;
}

std::map<std::string, Font*>& ResourceManager::GetFonts()
{
    return m_fonts;
}

std::map<std::string, Prefab*>& ResourceManager::GetPrefabs()
{
    return m_prefabs;
}

std::map<std::string, AudioClip*>& ResourceManager::GetAudios()
{
    return m_audios;
}