#ifndef CORE_RESOURCES_RESOURCE_MANAGER_H_
#define CORE_RESOURCES_RESOURCE_MANAGER_H_

#include <map>
#include <string>
#include "../Render/Texture.h"
#include "../Render/Material.h"
#include "../Render/Mesh.h"
#include "../Scripting/Script.h"
#include "CoreExport.h"

namespace DreamEngine::Core::Resources
{

using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::Scripting;

class CORE_API ResourceManager
{
   public:
    ResourceManager() = default;
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
    Script* GetScript(const std::string& resourceId);
    std::map<std::string, Shader*>& GetShaders();
    std::map<std::string, Material*>& GetMaterials();
    std::map<std::string, Texture*>& GetTextures();
    std::map<std::string, Mesh*>& GetMeshes();
    std::map<std::string, Script*>& GetScripts();
   protected:
    std::map<std::string, Shader*> m_shaders;
    std::map<std::string, Material*> m_materials;
    std::map<std::string, Texture*> m_textures;
    std::map<std::string, Mesh*> m_meshes;
    std::map<std::string, Script*> m_scripts;
};
}  // namespace DreamEngine::Core::Resources
#endif