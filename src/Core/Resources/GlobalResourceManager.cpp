#include "GlobalResourceManager.h"

#include "../Render/OpenGL/OpenGLMesh.h"
#include "../Helpers/GUIDHelper.h"

using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Helpers;

GlobalResourceManager& GlobalResourceManager::Instance()
{
    static GlobalResourceManager globalResourceManager;

    return globalResourceManager;
}

void GlobalResourceManager::AddShader(Shader* shader)
{
    AddShader(GUIDHelper::GenerateGUID(), shader);
}

void GlobalResourceManager::AddShader(const std::string& resourceId, Shader* shader)
{
    m_resourceManager.AddShader(resourceId, shader);
}

void GlobalResourceManager::AddMaterial(Material* material)
{
    AddMaterial(GUIDHelper::GenerateGUID(), material);
}

void GlobalResourceManager::AddMaterial(const std::string& resourceId, Material* material)
{
    m_resourceManager.AddMaterial(resourceId, material);
}

void GlobalResourceManager::AddTexture(Texture* texture)
{
    AddTexture(GUIDHelper::GenerateGUID(), texture);
}

void GlobalResourceManager::AddTexture(const std::string& resourceId, Texture* texture)
{
    m_resourceManager.AddTexture(resourceId, texture);
}

void GlobalResourceManager::AddMesh(Mesh* mesh)
{
    AddMesh(GUIDHelper::GenerateGUID(), mesh);
}

void GlobalResourceManager::AddMesh(const std::string& resourceId, Mesh* mesh)
{
    m_resourceManager.AddMesh(resourceId, mesh);
}

void GlobalResourceManager::AddScript(Script* script)
{
    m_resourceManager.AddScript(script);
}

void GlobalResourceManager::AddScript(const std::string& resourceId, Script* script)
{
    m_resourceManager.AddScript(resourceId, script);
}

void GlobalResourceManager::Clear()
{
    m_resourceManager.Clear();
}

void GlobalResourceManager::RemoveMaterial(const Material* material)
{
    m_resourceManager.RemoveMaterial(material);
}

void GlobalResourceManager::RemoveScript(const Script* script)
{
    m_resourceManager.RemoveScript(script);
}

Shader* GlobalResourceManager::GetShader(const std::string& resourceId)
{
    return m_resourceManager.GetShader(resourceId);
}

Material* GlobalResourceManager::GetMaterial(const std::string& resourceId)
{
    return m_resourceManager.GetMaterial(resourceId);
}

Texture* GlobalResourceManager::GetTexture(const std::string& resourceId)
{
    return m_resourceManager.GetTexture(resourceId);
}

Mesh* GlobalResourceManager::GetMesh(const std::string& resourceId)
{
    Mesh* mesh = m_resourceManager.GetMesh(resourceId);

    return mesh;
}

Script* GlobalResourceManager::GetScript(const std::string& resourceId)
{
    Script* script = m_resourceManager.GetScript(resourceId);

    return script;
}

std::map<std::string, Material*>& GlobalResourceManager::GetMaterials()
{
    return m_resourceManager.GetMaterials();
}

std::map<std::string, Shader*>& GlobalResourceManager::GetShaders()
{
    return m_resourceManager.GetShaders();
}


std::map<std::string, Texture*>& GlobalResourceManager::GetTextures()
{
    return m_resourceManager.GetTextures();
}

std::map<std::string, Mesh*>& GlobalResourceManager::GetMeshes()
{
    return m_resourceManager.GetMeshes();
}

std::map<std::string, Script*>& GlobalResourceManager::GetScripts()
{
    return m_resourceManager.GetScripts();
}
