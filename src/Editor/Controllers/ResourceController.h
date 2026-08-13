#ifndef EDITOR_CONTROLLERS_RESOURCE_CONTROLLER_H_
#define EDITOR_CONTROLLERS_RESOURCE_CONTROLLER_H_

#include <string>
#include "../Models/Result.h"
#include "../Importers/BaseModelImporter.h"
#include "../../Core/Render/Material.h"
#include "../../Core/Scripting/ScriptInfo.h"
#include "UI/UiContent.h"
#include "../EditorContext.h"

namespace DreamEngine::Editor::Controllers
{
using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::UI;
using namespace DreamEngine::Core::Scripting;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::Importers;

class ResourceController
{
   public:
    ResourceController(EditorContext& editorContext);
    Result CreateMaterialFile(const std::string& filename);
    Result CreateTextureFile(const std::string& filename);
    Result CreateMeshFileFromModelFile(const std::string& filename);
    Result CreateSceneFile(const std::string& filename);
    Result CreateUIFile(const std::string& filename);
    void SaveMaterialFile(const Material* material, const std::string& pathAndFilename);
    void DeleteMaterialFile(const std::string& pathAndFilename);
    Material* LoadMaterial(const std::string pathAndFilename);
    Texture* LoadTexture(const std::string pathAndFilename);
    Model& LoadModel(const std::string pathAndFilename);
    UiContent* LoadUiContent(const std::string pathAndFilename);
    void LoadMaterials(const std::vector<std::string>& materialFiles);
    void LoadTextures(const std::vector<std::string>& textureFiles);
    void LoadModels(const std::vector<std::string>& modelFiles);
    void LoadUiContents(const std::vector<std::string>& uiFiles);
    void AddScripts(const std::vector<ScriptInfo>& scriptInfos);
    void UnloadAllResources();
    void ReloadUiComponent(const path uiFile);
   private:
    EditorContext& m_editorContext;
    BaseModelImporter* m_modelImporter;
    void DeleteMaterialFromResourceManager(const Material* material);
    Result TryAddToResourceManager(Material* material, const bool mustGenerateResourceId);
    Result TryAddToResourceManager(Texture* texture, const bool mustGenerateResourceId);
    Result TryAddToResourceManager(Mesh* mesh, const bool mustGenerateResourceId);
    Result TryAddToResourceManager(UiContent* mesh, const bool mustGenerateResourceId);
};

}  // namespace DreamEngine::Editor::Controllers
#endif