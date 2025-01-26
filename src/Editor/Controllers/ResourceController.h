#ifndef EDITOR_CONTROLLERS_RESOURCE_CONTROLLER_H_
#define EDITOR_CONTROLLERS_RESOURCE_CONTROLLER_H_

#include <string>
#include "../Models/Result.h"
#include "../Importers/BaseModelImporter.h"
#include "../../Core/Render/Material.h"
#include "../../Core/Scripting/ScriptInfo.h"

namespace DreamEngine::Editor::Controllers
{
using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::Scripting;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::Importers;

class ResourceController
{
   public:
    ResourceController() = default;
    static Result CreateMaterialFile(const std::string& filename);
    static Result CreateTextureFile(const std::string& filename);
    static Result CreateMeshFileFromModelFile(const std::string& filename);
    static void SaveMaterialFile(const Material* material, const std::string& pathAndFilename);
    static void DeleteMaterialFile(const std::string& pathAndFilename);
    static Material* LoadMaterial(const std::string pathAndFilename);
    static Texture* LoadTexture(const std::string pathAndFilename);
    static Model& LoadModel(const std::string pathAndFilename);
    static void LoadMaterials(const std::vector<std::string>& materialFiles);
    static void LoadTextures(const std::vector<std::string>& textureFiles);
    static void LoadModels(const std::vector<std::string>& modelFiles);
    static void AddScripts(const std::vector<ScriptInfo>& scriptInfos);
   private:
    static BaseModelImporter* m_modelImporter;
    static void DeleteMaterialFromResourceManager(const Material* material);
    static Result TryAddToResourceManager(Material* material, const bool mustGenerateResourceId);
    static Result TryAddToResourceManager(Texture* texture, const bool mustGenerateResourceId);
    static Result TryAddToResourceManager(Mesh* mesh, const bool mustGenerateResourceId);
};

}  // namespace DreamEngine::Editor::Controllers
#endif