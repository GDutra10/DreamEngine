#ifndef CORE_RESOURCES_GLOBAL_RESOURCE_MANAGER_H_
#define CORE_RESOURCES_GLOBAL_RESOURCE_MANAGER_H_

#include <string>
#include "ResourceManager.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"
#include "../Render/Material.h"
#include "../Render/Mesh.h"

namespace DreamEngine::Core::Resources
{

using namespace DreamEngine::Core::Render;

class CORE_API GlobalResourceManager
{
   public:
    static GlobalResourceManager& Instance();

    void AddShader(Shader* shader);
    void AddShader(const std::string& resourceId, Shader* shader);
    void AddMaterial(Material* material);
    void AddMaterial(const std::string& resourceId, Material* material);
    void AddTexture(Texture* texture);
    void AddTexture(const std::string& resourceId, Texture* texture);
    void AddMesh(Mesh* mesh);
    void AddMesh(const std::string& resourceId, Mesh* mesh);
    void AddScript(Script* script);
    void AddScript(const std::string& resourceId, Script* script);
    void Clear();
    void RemoveMaterial(const Material* material);
    void RemoveScript(const Script* script);
    Shader* GetShader(const std::string& resourceId);
    Material* GetMaterial(const std::string& resourceId);
    Texture* GetTexture(const std::string& resourceId);
    Mesh* GetMesh(const std::string& resourceId);
    std::map<std::string, Shader*>& GetShaders();
    std::map<std::string, Material*>& GetMaterials();
    std::map<std::string, Texture*>& GetTextures();
    std::map<std::string, Mesh*>& GetMeshes();
    std::map<std::string, Script*>& GetScripts();
   private:
    ResourceManager m_resourceManager;
    GlobalResourceManager() = default;
};
}  // namespace DreamEngine::Core::Resources
#endif