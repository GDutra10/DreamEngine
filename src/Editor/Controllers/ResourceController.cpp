#include <iostream>
#include <fstream>
#include <filesystem>

#include "ResourceController.h"

#include "Application.h"
#include "../../Core/EngineDefine.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../../Core/Resources/ResourceManager.h"
#include "../Singletons/EditorSingleton.h"
#include "../Serializers/MaterialSerializer.h"
#include "../Serializers/ModelSerializer.h"
#include "../Serializers/TextureSerializer.h"
#include "../Serializers/UiContentSerializer.h"
#include "../EditorDefine.h"
#include "../Importers/AssimpModelImporter.h"
#include "../Serializers/SceneDataSerializer.h"

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Singletons;
using namespace DreamEngine::Editor::Serializers;

BaseModelImporter* ResourceController::m_modelImporter = new AssimpModelImporter();

Result ResourceController::CreateMaterialFile(const std::string& filename)
{
    LoggerSingleton::Instance().LogTrace("ResourceController::CreateMaterialFile -> Start");

    Material* newMaterial = nullptr;
    Result result = { "", true };
    string pathAndFileName = EditorSingleton::Instance().GetSelectedPath().string() + "\\" + filename + EDITOR_DEFAULT_MATERIAL_FILE_EXTENSION;

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
    string pathAndFileName = EditorSingleton::Instance().GetSelectedPath().string() + "\\" + filename + EDITOR_DEFAULT_SCENE_FILE_EXTENSION;

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
            SceneData sceneData;
            sceneData.globalLight.transform.position = {0.f, 500.f, 0.f};
            sceneData.globalLight.transform.rotation= {0.f, 0.f, 0.f};
            sceneData.globalLight.transform.scale = {0.f, 0.f, 0.f};
            sceneData.globalLight.directionalLight.color = {1.f, 1.f, 1.f};
            sceneData.globalLight.directionalLight.specular = {1.f, 1.f, 1.f};
            sceneData.globalLight.directionalLight.influence = 2.0f;

            file << SceneDataSerializer::Serialize(sceneData);
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
                    textureFromResourceManager = new Texture(*texture);
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
        uiContent->name = p.filename().string();
        uiContent->resourceId = uiFile; // TODO: uiFile should remove the path before the root project!

        TryAddToResourceManager(uiContent, false);
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
            ResourceManager::Instance().AddScript(newScript);
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

void ResourceController::UnloadAllResources()
{
    ResourceManager::Instance().Clear();
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