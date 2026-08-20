#include <iostream>
#include <fstream>
#include <filesystem>

#include "ResourceController.h"

#include "Application.h"
#include "../../Core/EngineDefine.h"
#include "../../Core/ECS/Components/ChildrenComponent.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../../Core/Resources/ResourceManager.h"
#include "../../Core/Serializers/AudioClipSerializer.h"
#include "../../Core/Serializers/MaterialSerializer.h"
#include "../../Core/Serializers/TextureSerializer.h"
#include "../../Core/Serializers/UiContentSerializer.h"
#include "../../Core/Serializers/PrefabEntityDefinitionSerializer.h"
#include "../../Core/Serializers/SceneDefinitionSerializer.h"
#include "../Serializers/ModelSerializer.h"
#include "../EditorDefine.h"
#include "../Importers/AssimpModelImporter.h"
#include "../Helpers/FileHelper.h"
#include <ECS/Components/UiComponent.h>
#include "../Importers/TextureImporter.h"
#include "../../Core/GameSystem/Prefab.h"
#include "../../Core/GameSystem/Definitions/PrefabEntityDefinition.h"
#include "../../Core/IO/File.h"

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::GameSystem::Definitions;
using namespace DreamEngine::Core::Serializers;
using namespace DreamEngine::Core::Sync;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Serializers;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::Importers;
using namespace DreamEngine::Editor::Serializers;

ResourceController::ResourceController(EditorContext& editorContext) 
    : m_editorContext(editorContext)
    , m_modelImporter(new AssimpModelImporter()) {}

Result ResourceController::CreateMaterialFile(const std::string& filename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::CreateMaterialFile -> Start");

    Material* newMaterial = nullptr;
    Result result = { "", true };
    string pathAndFileName = m_editorContext.GetSelectedPath().string() + "\\" + filename + EDITOR_DEFAULT_MATERIAL_FILE_EXTENSION;

    // validations
    if (filename.empty())
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::CreateMaterialFile -> Filename is empty");
        return {"File name is empty", false};
    }

    if (exists(pathAndFileName))
    {
        std::string validation = "File already exists in this directory";
        LoggerSingleton::Instance().LogWarning(validation);
        return {validation, false};
    }

    try
    {
        // Add the material to the resource manager
        newMaterial = new Material(*ResourceManager::Instance().GetMaterial(DEFAULT_MATERIAL_NAME));
        newMaterial->name = filename;
        result = TryAddToResourceManager(newMaterial, true);

        if (!result.isOk)
            return result;

        // Create File
        LoggerSingleton::Instance().LogTrace("ResourceController::CreateMaterialFile -> Creating and saving the file");
        std::ofstream file(pathAndFileName);

        if (!result.isOk)
            return result;

        if (file.is_open())
        {
            file << MaterialSerializer::Serialize(*newMaterial);
            file.close();

            LoggerSingleton::Instance().LogTrace("ResourceController::CreateMaterialFile -> Material '" + newMaterial->name + "' saved");
        }
        else
        {
            LoggerSingleton::Instance().LogError("Failed to open the file");
            result.errorMessage = "Failed to open the file";
            result.isOk = false;
        }
    }
    catch (const std::exception& e)
    {
        LoggerSingleton::Instance().LogError(e.what());
        result.errorMessage = "An error occured! Please check the Output Window to see the error";
        result.isOk = false;
    }

    if (result.isOk)
        LoggerSingleton::Instance().LogInfo("Material '" + filename + "' created successfully");
    else
        DeleteMaterialFromResourceManager(newMaterial);

    return result;
}

Result ResourceController::CreateTextureFile(const std::string& filename)
{
    Result result;

    return result;
}

Result ResourceController::CreateMeshFileFromModelFile(const std::string& filename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::CreateMeshFileFromModelFile -> Start");

    Result result = {"", true};
    Model& model = m_modelImporter->Import(filename);

    for (Texture* texture : model.textures)
        ResourceManager::Instance().AddTexture(texture);

    // add mesh to the global resource manager
    for (Mesh* mesh : model.meshes)
        ResourceManager::Instance().AddMesh(mesh);

    std::string pathAndFileName = filename;

    if (pathAndFileName.find(".fbx") != std::string::npos)
        pathAndFileName.replace(pathAndFileName.find(".fbx"), 4, ".model");
    else if (pathAndFileName.find(".obj") != std::string::npos)
        pathAndFileName.replace(pathAndFileName.find(".obj"), 4, ".model");

    try
    {
        LoggerSingleton::Instance().LogTrace("ResourceController::CreateMeshFileFromModelFile -> Creating and saving the file");
        std::ofstream file(pathAndFileName);

        if (!result.isOk)
            return result;

        if (file.is_open())
        {
            file << ModelSerializer::Serialize(model);
            file.close();

            LoggerSingleton::Instance().LogTrace("ResourceController::CreateMeshFileFromModelFile -> Model saved");
        }
        else
        {
            LoggerSingleton::Instance().LogError("Failed to open the file");
            result.errorMessage = "Failed to open the file";
            result.isOk = false;
        }
    }
    catch (const std::exception& e)
    {
        LoggerSingleton::Instance().LogError(e.what());
        result.errorMessage = "An error occured! Please check the Output Window to see the error";
        result.isOk = false;
    }

    if (result.isOk)
        LoggerSingleton::Instance().LogInfo("Model imported successfully!");
    else
    {
        // TODO: delete all meshes and textures
        //DeleteMaterialFromResourceManager(newMaterial);
    }

    return result;
}

Result ResourceController::CreateSceneFile(const std::string& filename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::CreateSceneFile -> Start");
    Result result = {"", true};
    string pathAndFileName = m_editorContext.GetSelectedPath().string() + "\\" + filename + EDITOR_DEFAULT_SCENE_FILE_EXTENSION;

    // validations
    if (filename.empty())
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::CreateSceneFile -> Filename is empty");
        return {"File name is empty", false};
    }

    if (exists(pathAndFileName))
    {
        std::string validation = "File already exists in this directory";
        LoggerSingleton::Instance().LogWarning(validation);
        return {validation, false};
    }

    try
    {
        // Create File
        LoggerSingleton::Instance().LogTrace("ResourceController::CreateSceneFile -> Creating and saving the file");
        std::ofstream file(pathAndFileName);

        if (file.is_open())
        {
            SceneDefinition sceneDefinition;
            sceneDefinition.globalLight.transform.position = {0.f, 500.f, 0.f};
            sceneDefinition.globalLight.transform.rotation= {0.f, 0.f, 0.f};
            sceneDefinition.globalLight.transform.scale = {0.f, 0.f, 0.f};
            sceneDefinition.globalLight.directionalLight.color = {1.f, 1.f, 1.f};
            sceneDefinition.globalLight.directionalLight.specular = {1.f, 1.f, 1.f};
            sceneDefinition.globalLight.directionalLight.influence = 2.0f;

            file << SceneDefinitionSerializer::Serialize(sceneDefinition).dump(4);
            file.close();

            LoggerSingleton::Instance().LogTrace("ResourceController::CreateSceneFile -> Scene '" + filename + "' saved");
        }
        else
        {
            LoggerSingleton::Instance().LogError("Failed to open the file");
            result.errorMessage = "Failed to create the file";
            result.isOk = false;
        }
    }
    catch (const std::exception& e)
    {
        LoggerSingleton::Instance().LogError(e.what());
        result.errorMessage = "An error occured! Please check the Output Window to see the error";
        result.isOk = false;
    }

    return result;
}

Result DreamEngine::Editor::Controllers::ResourceController::CreateUIFile(const std::string& filename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::CreateUIFile -> Start");

    Result result = {"", true};
    string pathAndFileName = m_editorContext.GetSelectedPath().string() + "\\" + filename + EDITOR_DEFAULT_UI_FILE_EXTENSION;

    // validations
    if (filename.empty())
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::CreateUIFile -> Filename is empty");
        return {"File name is empty", false};
    }

    if (exists(pathAndFileName))
    {
        std::string validation = "File already exists in this directory";
        LoggerSingleton::Instance().LogWarning(validation);
        return {validation, false};
    }

    try
    {
        // Create File
        LoggerSingleton::Instance().LogTrace("ResourceController::CreateUIFile -> Creating and saving the file");
        std::ofstream file(pathAndFileName);

        if (file.is_open())
        {
            string contentStr = ""
                "<rml>\n"
                "<head>\n"
                "    <title>" + filename + "</title>\n"
                "    <style>\n"
                "        body {\n"
                "            margin: 0px;\n"
                "            font-family: 'Roboto';\n"
                "            font-size: 36px;\n"
                "        }\n"
                "    </style>\n"
                "</head>\n"
                "<body>\n"
                "   <div id=\"hud-root\" data-model=\"" + filename + ".rml\">\n"
                "       <label>HUD</label>\n"
                "   </div>\n"
                "</body>\n"
                "</rml>\n";
            file << contentStr;
            file.close();

            UiContent* hudUiContent = new UiContent();
            hudUiContent->name = filename;
            hudUiContent->text = contentStr;

            ResourceManager::Instance().AddUiContent(hudUiContent);

            LoggerSingleton::Instance().LogTrace("ResourceController::CreateUIFile -> UI '" + filename + "' saved");
        }
        else
        {
            LoggerSingleton::Instance().LogError("Failed to open the file");
            result.errorMessage = "Failed to create the file";
            result.isOk = false;
        }
    }
    catch (const std::exception& e)
    {
        LoggerSingleton::Instance().LogError(e.what());
        result.errorMessage = "An error occured! Please check the Output Window to see the error";
        result.isOk = false;
    }

    return result;
}

Result ResourceController::CreateAudioClipFromAudioFile(const std::string& filename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::CreateAudioClipFromAudioFile -> Start");

    path path(filename);

    if (!FileHelper::IsExpectedExtension(filename, std::vector<std::string>{EDITOR_SUPPORTED_AUDIO_FILE_EXTENSIONS}))
    {
        return {"File not supported", false};
    }

    AudioClip* audioClip = new AudioClip();
    audioClip->data = DreamEngine::Core::IO::File::ReadBinary(filename);
    audioClip->name = path.filename().string();
    audioClip->filePath = m_editorContext.GetSelectedPath().string() + "\\" + path.filename().string();
    
    if (Result res = TryAddToResourceManager(audioClip, true); !res.isOk)
    {
        delete audioClip;

        return res;    
    }

    auto json = AudioClipSerializer::Serialize(*audioClip);
    auto jsonText = json.dump(4);

    if (Result res = FileHelper::CreateFile(m_editorContext.GetSelectedPath().string() + "\\", path.filename().string() + ".audio", jsonText); !res.isOk)
        return res;

    return { "", true };
}

void ResourceController::SaveMaterialFile(const Material* material, const std::string& pathAndFilename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::SaveMaterialFile -> Start");

    if (material == nullptr)
    {
        LoggerSingleton::Instance().LogError("ResourceController::SaveMaterialFile -> Material is null");
        return;
    }

    std::ofstream file(pathAndFilename);

    if (file.is_open())
    {
        file << MaterialSerializer::Serialize(*material);
        file.close();

        LoggerSingleton::Instance().LogInfo("Material '" + material->name + "' saved successfully");
    }
    else
    {
        LoggerSingleton::Instance().LogError("Failed to open the file");
    }
}

void ResourceController::DeleteMaterialFile(const std::string& pathAndFilename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::CreateMaterialFile -> Start");

    try
    {
        Material* material = LoadMaterial(pathAndFilename);

        if (material != nullptr)
        {
            Material* materialFromManager = ResourceManager::Instance().GetMaterial(material->resourceId);

            delete material;

            DeleteMaterialFromResourceManager(materialFromManager);
        }

        if (std::filesystem::remove(pathAndFilename))
            LoggerSingleton::Instance().LogInfo("Material '" + pathAndFilename + "' deleted");
        else
            LoggerSingleton::Instance().LogWarning("Material '" + pathAndFilename + "' can't be deleted!");
    }
    catch (const std::exception& e)
    {
        LoggerSingleton::Instance().LogError("ResourceController::DeleteMaterialFile -> Exception: " + std::string(e.what()));
    }
}

void ResourceController::DeleteMaterialFromResourceManager(const Material* material)
{
    if (material == nullptr)
    {
        LoggerSingleton::Instance().LogTrace("ResourceController::DeleteMaterialFromResourceManager -> Material is null");

        return;
    }

    LoggerSingleton::Instance().LogTrace("ResourceController::DeleteMaterialFromResourceManager -> Deleting material '" + material->name + "' (" + material->resourceId + ")");

    std::string materialResourceId = material->resourceId;

    ResourceManager::Instance().RemoveMaterial(material);

    LoggerSingleton::Instance().LogDebug("Removed material '" + materialResourceId + "' from Resource manager");
}

Material* ResourceController::LoadMaterial(const std::string pathAndFilename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadMaterial -> Loading material '" + pathAndFilename + "'");

    std::ifstream stream(pathAndFilename);

    return MaterialSerializer::Deserialize(stream);
}

Texture* ResourceController::LoadTexture(const std::string pathAndFilename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadTexture -> Loading texture '" + pathAndFilename + "'");

    std::ifstream stream(pathAndFilename);

    return nullptr;
}

Model& ResourceController::LoadModel(const std::string pathAndFilename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadModel -> Loading model '" + pathAndFilename + "'");

    std::ifstream stream(pathAndFilename);

    return ModelSerializer::Deserialize(stream);
}

UiContent* ResourceController::LoadUiContent(const std::string pathAndFilename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadUiContent -> Loading ui '" + pathAndFilename + "'");

    std::ifstream stream(pathAndFilename);

    return UiContentSerializer::Deserialize(stream);
}

Prefab* ResourceController::LoadPrefab(const std::string pathAndFilename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadPrefab -> Loading prefab '" + pathAndFilename + "'");

    std::ifstream stream(pathAndFilename);

    Prefab* prefab = new Prefab();
    prefab->root = PrefabEntityDefinitionSerializer::Deserialize(stream);
    prefab->resourceId = prefab->root.resourceId;

    return prefab;
}

AudioClip* ResourceController::LoadAudio(const std::string pathAndFilename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadAudio -> Loading audio '" + pathAndFilename + "'");
    std::ifstream stream(pathAndFilename);

    AudioClip* audio = AudioClipSerializer::Deserialize(stream);
    std::string filePath = audio->filePath;
    
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadAudio -> Loading audio from '" + filePath + "'");

    audio->data = DreamEngine::Core::IO::File::ReadBinary(filePath);

    return audio;
}

void ResourceController::LoadMaterials(const std::vector<std::string>& materialFiles)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadMaterials -> Start");

    for (const std::string& materialFile : materialFiles)
    {
        Material* material = LoadMaterial(materialFile);

        if (material == nullptr)
        {
            LoggerSingleton::Instance().LogError("ResourceController::LoadMaterials -> Material is null");
            continue;
        }

        TryAddToResourceManager(material, false);
    }
}

void ResourceController::LoadTextures(const std::vector<std::string>& textureFiles)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadTextures -> Start");

    for (const std::string& textureFile : textureFiles)
    {
        Texture* texture = LoadTexture(textureFile);

        if (texture == nullptr)
        {
            LoggerSingleton::Instance().LogError("ResourceController::LoadMaterials -> Material is null");
            continue;
        }

        TryAddToResourceManager(texture, false);
    }
}

void ResourceController::LoadModels(const std::vector<std::string>& modelFiles)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadModels -> Start");

    for (const std::string& modelFile : modelFiles)
    {
        Model& model = LoadModel(modelFile);

        // add mesh to the global resource manager
        for (Mesh* mesh : model.meshes)
        {
            std::vector<Texture*> textures;

            for (Texture* texture : mesh->textures)
            {
                Texture* textureFromResourceManager = ResourceManager::Instance().GetTexture(texture->resourceId);

                if (textureFromResourceManager == nullptr)
                {
                    textureFromResourceManager = TextureImporter::Import(texture->path, texture->name.c_str(), texture->type);
                    textureFromResourceManager->resourceId = texture->resourceId;
                    TryAddToResourceManager(textureFromResourceManager, false);
                }

                textures.push_back(textureFromResourceManager);
            }

            size_t count = mesh->textures.size();

            for (int i = 0; i < count; i++)
                delete mesh->textures[i];

            mesh->textures = textures;

            TryAddToResourceManager(mesh, false);
        }
            
    }
}

void ResourceController::LoadUiContents(const std::vector<std::string>& uiFiles)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadUiContents -> Start");

    for (const std::string& uiFile : uiFiles)
    {
        UiContent* uiContent = LoadUiContent(uiFile);
        
        const path p = path(uiFile);
        const ProjectConfiguration& projectConfig = m_editorContext.GetProjectConfiguration();
        uiContent->name = p.filename().string();
        uiContent->resourceId = FileHelper::GetRelativePathByProject(uiFile, projectConfig).string();  

        TryAddToResourceManager(uiContent, false);
    }
}

void ResourceController::LoadPrefabs(const std::vector<std::string>& prefabFiles)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadPrefabs -> Start");

    for (const std::string& prefabFile : prefabFiles)
    {
        Prefab* prefab = LoadPrefab(prefabFile);

        const path p = path(prefabFile);
        const ProjectConfiguration& projectConfig = m_editorContext.GetProjectConfiguration();
        prefab->name = p.filename().string();

        TryAddToResourceManager(prefab, false);
    }
}

void ResourceController::LoadAudios(const std::vector<std::string>& audioFiles) 
{
    LoggerSingleton::Instance().LogTrace("ResourceController::LoadAudios -> Start");

    for (const std::string& audioFile : audioFiles)
    {
        std::ifstream stream(audioFile);
        
        AudioClip* audioClip = LoadAudio(audioFile);

        const path p = path(audioFile);
        audioClip->name = p.filename().string();

        TryAddToResourceManager(audioClip, false);
    }
}

void ResourceController::AddScripts(const std::vector<ScriptInfo>& scriptInfos)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::AddScripts -> Start");

    const std::map<std::string, Script*>& scripts = ResourceManager::Instance().GetScripts();

    LoggerSingleton::Instance().LogTrace("ResourceController::AddScripts -> Adding new Scripts");

    // add if not exists
    for (auto scriptInfo : scriptInfos)
    {
        // check if exists in the ResourceManager
        auto it = ranges::find_if(scripts, [&scriptInfo](const pair<const string, Script*>& pair)
        {
            return scriptInfo.AssemblyName == pair.second->GetAssemblyName() &&
                scriptInfo.Name == pair.second->GetClassName();
        });

        // if not, create a new Script object and add to the ResourceManager
        if (it == scripts.end())
        {
            string nameStr = string(scriptInfo.Name);
            size_t dotPos = nameStr.find('.');
            nameStr = (dotPos != std::string::npos) ? nameStr.substr(dotPos + 1) : nameStr;

            Script* newScript = new Script(scriptInfo.Name, scriptInfo.AssemblyName);
            newScript->name = nameStr;
            ResourceManager::Instance().AddScript(scriptInfo.Name, newScript);
        }
    }

    std::vector<Script*> scriptToRemove;

    LoggerSingleton::Instance().LogTrace("ResourceController::AddScripts -> Removing remaining Scripts that not exists anymore");

    // remove if not exits
    for (auto [id, pScript] : scripts)
    {
        // check if exists in the ResourceManager
        auto it = ranges::find_if(scriptInfos, [&pScript](const ScriptInfo scriptInfo)
        {
            return
                scriptInfo.AssemblyName == pScript->GetAssemblyName() &&
                scriptInfo.Name == pScript->GetClassName();
        });

        // if not, add to the list to remove later
        if (it == scriptInfos.end())
        {
            scriptToRemove.push_back(pScript);
        }
    }

    for (const Script* script : scriptToRemove)
        ResourceManager::Instance().RemoveScript(script);

    LoggerSingleton::Instance().LogTrace("ResourceController::AddScripts -> Finish");
}

void ResourceController::SavePrefab(Entity* entity)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::SavePrefab -> Start");
    LoggerSingleton::Instance().LogTrace("ResourceController::SavePrefab -> saving entity as prefab -> " + std::to_string(entity->GetId()));

    Prefab* prefab = new Prefab();
    prefab->name = entity->GetName();
    prefab->root = GetPrefabEntityData(entity);
    ResourceManager::Instance().AddPrefab(prefab);
    prefab->root.resourceId = prefab->resourceId;
    std::string content = PrefabEntityDefinitionSerializer::Serialize(prefab->root).dump(4);
    Result result = FileHelper::CreateFile(m_editorContext.GetSelectedPath().string(), prefab->name + EDITOR_DEFAULT_PREFAB_FILE_EXTENSION, content);

    if (!result.isOk) 
    {
        LoggerSingleton::Instance().LogError("ResourceController::SavePrefab -> " + result.errorMessage);
        ResourceManager::Instance().RemovePrefab(prefab);
    }
}

void ResourceController::UnloadAllResources()
{
    ResourceManager::Instance().Clear();
}

void ResourceController::ReloadUiComponent(const path uiFile) 
{
    const UiContent* content = ResourceController::LoadUiContent(uiFile.string());
    const ProjectConfiguration& projectConfig = m_editorContext.GetProjectConfiguration();
    const std::string resourceId = FileHelper::GetRelativePathByProject(uiFile, projectConfig).string();
    const map<string, UiContent*>& contents = ResourceManager::Instance().GetUiContents();

    if (const auto it = contents.find(resourceId); it != contents.end() && it->second != nullptr)
    {
        it->second->text = content->text;

        for (Entity* entity : m_editorContext.GetEntityManager()->GetEntities())
        {
            if (entity->HasComponent<UiComponent>())
            {
                UiComponent& uiComponent = entity->GetComponent<UiComponent>();

                if (uiComponent.content->resourceId != resourceId)
                    continue;

                if (uiComponent.instance == nullptr)
                    continue;

                UiManager::Destroy(uiComponent.instance);
                uiComponent.instance = nullptr;
            }
        }
    }

    delete content;
}

Result ResourceController::TryAddToResourceManager(Material* material, const bool mustGenerateResourceId)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::TryAddToResourceManager -> Start");

    if (material == nullptr)
    {
        LoggerSingleton::Instance().LogError("ResourceController::TryAddToResourceManager -> Material is null");
        return {"Material is null", false};
    }

    if (!mustGenerateResourceId && ResourceManager::Instance().GetMaterial(material->resourceId) != nullptr)
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::TryAddToResourceManager -> Material already exists in the resource manager");
        return {"Material already exists in the resource manager", false};
    }

    if (mustGenerateResourceId)
        ResourceManager::Instance().AddMaterial(material);
    else
        ResourceManager::Instance().AddMaterial(material->resourceId, material);

    LoggerSingleton::Instance().LogDebug("Material '" + material->name + "' (" + material->resourceId + ") added to the resource manager");

    return {"", true};
}

Result ResourceController::TryAddToResourceManager(Texture* texture, const bool mustGenerateResourceId)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::TryAddToResourceManager -> Start");

    if (texture == nullptr)
    {
        LoggerSingleton::Instance().LogError("ResourceController::TryAddToResourceManager -> Texture is null");
        return {"Texture is null", false};
    }

    if (!mustGenerateResourceId && ResourceManager::Instance().GetTexture(texture->resourceId) != nullptr)
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::TryAddToResourceManager -> Texture already exists in the resource manager");
        return {"Texture already exists in the resource manager", false};
    }

    if (mustGenerateResourceId)
        ResourceManager::Instance().AddTexture(texture);
    else
        ResourceManager::Instance().AddTexture(texture->resourceId, texture);

    LoggerSingleton::Instance().LogDebug("Texture '" + texture->name + "' (" + texture->resourceId + ") added to the resource manager");

    return {"", true};
}

Result ResourceController::TryAddToResourceManager(Mesh* mesh, const bool mustGenerateResourceId)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::TryAddToResourceManager -> Start");

    if (mesh == nullptr)
    {
        LoggerSingleton::Instance().LogError("ResourceController::TryAddToResourceManager -> Mesh is null");
        return {"Mesh is null", false};
    }

    if (!mustGenerateResourceId && ResourceManager::Instance().GetMesh(mesh->resourceId) != nullptr)
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::TryAddToResourceManager -> Mesh already exists in the resource manager");
        return {"Mesh already exists in the resource manager", false};
    }

    if (mustGenerateResourceId)
        ResourceManager::Instance().AddMesh(mesh);
    else
        ResourceManager::Instance().AddMesh(mesh->resourceId, mesh);

    LoggerSingleton::Instance().LogDebug("Mesh '" + mesh->name + "' (" + mesh->resourceId + ") added to the resource manager");

    return {"", true};
}

Result ResourceController::TryAddToResourceManager(UiContent* uiContent, const bool mustGenerateResourceId)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::TryAddToResourceManager -> Start");

    if (uiContent == nullptr)
    {
        LoggerSingleton::Instance().LogError("ResourceController::TryAddToResourceManager -> Mesh is null");
        return {"UIContent is null", false};
    }

    if (!mustGenerateResourceId && ResourceManager::Instance().GetMesh(uiContent->resourceId) != nullptr)
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::TryAddToResourceManager -> UIContent already exists in the resource manager");
        return {"UIContent already exists in the resource manager", false};
    }

    if (mustGenerateResourceId)
        ResourceManager::Instance().AddUiContent(uiContent);
    else
        ResourceManager::Instance().AddUiContent(uiContent->resourceId, uiContent);

    LoggerSingleton::Instance().LogDebug("UIContent '" + uiContent->name + "' (" + uiContent->resourceId + ") added to the resource manager");

    return {"", true};
}

Result ResourceController::TryAddToResourceManager(Prefab* prefab, const bool mustGenerateResourceId)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::TryAddToResourceManager -> Start");

    if (prefab == nullptr)
    {
        LoggerSingleton::Instance().LogError("ResourceController::TryAddToResourceManager -> Prefab is null");
        return {"Prefab is null", false};
    }

    if (!mustGenerateResourceId && ResourceManager::Instance().GetMesh(prefab->resourceId) != nullptr)
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::TryAddToResourceManager -> Prefab already exists in the resource manager");
        return {"Prefab already exists in the resource manager", false};
    }

    if (mustGenerateResourceId)
        ResourceManager::Instance().AddPrefab(prefab);
    else
        ResourceManager::Instance().AddPrefab(prefab->resourceId, prefab);

    LoggerSingleton::Instance().LogDebug("Prefab '" + prefab->name + "' (" + prefab->resourceId + ") added to the resource manager");

    return {"", true};
}

Result ResourceController::TryAddToResourceManager(AudioClip* audioClip, const bool mustGenerateResourceId)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::TryAddToResourceManager -> Start");

    if (audioClip == nullptr)
    {
        LoggerSingleton::Instance().LogError("ResourceController::TryAddToResourceManager -> audioClip is null");
        return {"AudioClip is null", false};
    }

    if (!mustGenerateResourceId && ResourceManager::Instance().GetAudio(audioClip->resourceId) != nullptr)
    {
        LoggerSingleton::Instance().LogWarning("ResourceController::TryAddToResourceManager -> AudioClip already exists in the resource manager");
        return {"AudioClip already exists in the resource manager", false};
    }

    if (mustGenerateResourceId)
        ResourceManager::Instance().AddAudio(audioClip);
    else
        ResourceManager::Instance().AddAudio(audioClip->resourceId, audioClip);

    LoggerSingleton::Instance().LogDebug("AudioClip '" + audioClip->name + "' (" + audioClip->resourceId + ") added to the resource manager");

    return {"", true};
}

PrefabEntityDefinition ResourceController::GetPrefabEntityData(Entity* entity)
{
    PrefabEntityDefinition prefabEntityData{};
    prefabEntityData.entity = entity->GetDefinition();

    ChildrenComponent& childrenComponent = entity->GetComponent<ChildrenComponent>();

    for (Entity* child : childrenComponent.children)
    {
        PrefabEntityDefinition childPrefabEntityData = GetPrefabEntityData(child);
        prefabEntityData.children.push_back(childPrefabEntityData);
    }

    return prefabEntityData;
}