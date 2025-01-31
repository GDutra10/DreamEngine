#include "ProjectController.h"

#include <fstream>

#include "Application.h"
#include "DotNetCliController.h"
#include "EngineDefine.h"
#include "Loggers/LoggerSingleton.h"

#include "../EditorDefine.h"
#include "../Helpers/FileHelper.h"
#include "../Serializers/ProjectConfigurationSerializer.h"
#include "../Singletons/EditorSingleton.h"

#include "../Vendors/assimp/contrib/pugixml/src/pugixml.hpp"
#include "IO/File.h"
#include "Render/Factories/MeshFactory.h"
#include "Resources/GlobalResourceManager.h"

#include "../Vendors/stb_image.h"

using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::Serializers;

ResultWithData<ProjectConfiguration> ProjectController::CreateProjectConfiguration(std::filesystem::path selectedPah, std::string projectName)
{
    ResultWithData<ProjectConfiguration> result = { };
    ProjectConfiguration projectConfiguration;
    std::string projectPath = selectedPah.string() + "\\" + projectName;

    if (!std::filesystem::create_directories(projectPath))
    {
        result.isOk = false;
        result.errorMessage = "Could not create project directory";

        return result;
    }

    const int newSolutionResult = DotNetCliController::NewSolution(projectPath, projectName);

    if (newSolutionResult != EDITOR_DOTNET_CLI_COMMAND_RESULT_SUCCESS)
    {
        result.isOk = false;
        result.errorMessage = "Could not create the solution";

        return result;
    }

    const int newClassLibResult = DotNetCliController::NewClassLib(projectPath, projectName);

    if (newClassLibResult != EDITOR_DOTNET_CLI_COMMAND_RESULT_SUCCESS)
    {
        result.isOk = false;
        result.errorMessage = "Could not create CS project";

        return result;
    }
    else
    {
        // remove default class created by dotnet cli
        std::filesystem::remove(projectPath + "\\Class1.cs");

        if (!TryAddDreamEngineReferenceInCsproj(projectPath, projectName))
        {
            result.isOk = false;
            result.errorMessage = "Could not add DreamEngine reference in csproj";
            return result;
        }
    }

    const int addProjectToSolutionResult = DotNetCliController::AddProjectToSolution(projectPath, projectName, projectName);

    if (addProjectToSolutionResult != EDITOR_DOTNET_CLI_COMMAND_RESULT_SUCCESS)
    {
        result.isOk = false;
        result.errorMessage = "Could not add project in solution";

        return result;
    }

    projectConfiguration.projectName = projectName + EDITOR_PROJECT_FILE_EXTENSION;
    projectConfiguration.projectPath = projectPath;
    projectConfiguration.csSolution = projectName + ".sln";
    projectConfiguration.csProjectPath = projectConfiguration.projectPath;
    projectConfiguration.csProjectDebugDll = projectName + ".dll";
    projectConfiguration.csProjectDebugPath = projectConfiguration.projectPath + "\\bin\\Debug\\net9.0";

    std::ofstream file(projectConfiguration.projectPath + "\\" + projectConfiguration.projectName);

    if (file.is_open())
    {
        file << ProjectConfigurationSerializer::Serialize(projectConfiguration);
        file.close();

        result.data = projectConfiguration;
        result.isOk = true;

        Core::Loggers::LoggerSingleton::Instance().LogTrace("ProjectController::CreateProjectConfiguration -> Created project successfully");
    }
    else
    {
        Core::Loggers::LoggerSingleton::Instance().LogError("Failed to open the file");
        result.errorMessage = "Failed to open the file";
        result.isOk = false;
    }

    return result;
}

void ProjectController::LoadProjectConfiguration()
{
    ProjectConfiguration& projectConfiguration = Singletons::EditorSingleton::Instance().GetProjectConfiguration();

    if (projectConfiguration.isLoaded && !projectConfiguration.projectName.empty())
        LoadProjectConfiguration(projectConfiguration);
    else
        Loggers::LoggerSingleton::Instance().LogWarning("ProjectController::LoadProjectConfiguration -> There is no project loaded");
}

void ProjectController::LoadProjectConfiguration(ProjectConfiguration& projectConfiguration)
{
    Loggers::LoggerSingleton::Instance().LogTrace("ProjectController::LoadProjectConfiguration -> Loading project configuration -> Start");

    // reset entities
    Singletons::EditorSingleton::Instance().GetEntityManager()->Reset();

    // Unload all resources from previous project
    ResourceController::UnloadAllResources();

    projectConfiguration.isLoaded = true;
    Singletons::EditorSingleton::Instance().SetProjectConfiguration(projectConfiguration);

    // Load Resources
    LoadDefaultResources();
    LoadResourcesFromProject(projectConfiguration);

    Loggers::LoggerSingleton::Instance().LogTrace("ProjectController::LoadProjectConfiguration -> Loading project configuration -> Finish");
}

void ProjectController::LoadDefaultResources()
{
    // add default shader
    const std::string vertexShader = Core::IO::File::ReadAllText("Assets/Shaders/default.vert.glsl");
    const std::string fragmentShader = Core::IO::File::ReadAllText("Assets/Shaders/default.frag.glsl");
    Shader* shader = Core::Application::Instance().GetRenderAPI()->CreateShader(DEFAULT_SHADER_NAME, vertexShader, fragmentShader);
    shader->name = DEFAULT_SHADER_NAME;
    GlobalResourceManager::Instance().AddShader(shader->name, shader);

    // add default material
    Material* material = new Material();
    material->name = DEFAULT_MATERIAL_NAME;
    material->shader = shader;
    material->specular = {0.5f, 0.5f, 0.5f};
    material->ambient = {0.2f, 0.2f, 0.2f};
    material->diffuse = {0.8f, 0.8f, 0.8f};
    material->shininess = 32.0f;
    GlobalResourceManager::Instance().AddMaterial(DEFAULT_MATERIAL_NAME, material);

    // add default texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Assets/Textures/defaulttexture.jpg", &width, &height, &nrChannels, 0);
    Texture* texture = Core::Application::Instance().GetRenderAPI()->CreateTexture(data, width, height, nrChannels);
    texture->name = DEFAULT_TEXTURE_NAME;
    stbi_image_free(data);

    if (texture != nullptr)
    {
        texture->type = Diffuse;
        GlobalResourceManager::Instance().AddTexture(DEFAULT_TEXTURE_NAME, texture);
    }

    Mesh* cubeMesh = Factories::MeshFactory::CreateMesh(Cube);
    cubeMesh->name = DEFAULT_CUBE_MESH_NAME;
    GlobalResourceManager::Instance().AddMesh(DEFAULT_CUBE_MESH_NAME, cubeMesh);

    Mesh* sphereMesh = Factories::MeshFactory::CreateMesh(Sphere);
    sphereMesh->name = DEFAULT_SPHERE_MESH_NAME;
    GlobalResourceManager::Instance().AddMesh(DEFAULT_SPHERE_MESH_NAME, sphereMesh);

    Mesh* capsuleMesh = Factories::MeshFactory::CreateMesh(Capsule);
    capsuleMesh->name = DEFAULT_CAPSULE_MESH_NAME;
    GlobalResourceManager::Instance().AddMesh(DEFAULT_CAPSULE_MESH_NAME, capsuleMesh);

    Mesh* cylinderMesh = Factories::MeshFactory::CreateMesh(Cylinder);
    cylinderMesh->name = DEFAULT_CYLINDER_MESH_NAME;
    GlobalResourceManager::Instance().AddMesh(DEFAULT_CYLINDER_MESH_NAME, cylinderMesh);

    Mesh* planeMesh = Factories::MeshFactory::CreateMesh(Plane);
    planeMesh->name = DEFAULT_PLANE_MESH_NAME;
    GlobalResourceManager::Instance().AddMesh(DEFAULT_PLANE_MESH_NAME, planeMesh);
}

void ProjectController::LoadResourcesFromProject(ProjectConfiguration& projectConfiguration)
{
    // TODO: load all resources from the project

    Singletons::EditorSingleton::Instance().GetScriptController().ReloadScripts();

    std::vector<std::string> materialFiles = Helpers::FileHelper::GetFilesWithExtension(projectConfiguration.projectPath, EDITOR_DEFAULT_MATERIAL_FILE_EXTENSION);
    ResourceController::LoadMaterials(materialFiles);

    std::vector<std::string> modelFiles = Helpers::FileHelper::GetFilesWithExtension(projectConfiguration.projectPath, EDITOR_DEFAULT_MODEL_FILE_EXTENSION);
    ResourceController::LoadModels(modelFiles);
}

bool ProjectController::TryAddDreamEngineReferenceInCsproj(const std::string& projectPath, const std::string& projectName)
{
    std::string csprojPath = projectPath + "\\" + projectName + ".csproj";
    std::string editorPath = FileHelper::GetExecutablePath().string();
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(csprojPath.c_str());
    if (result)
    {
        pugi::xml_node projectNode = doc.child("Project");

        if (projectNode)
        {
            pugi::xml_node itemGroup = projectNode.append_child("ItemGroup");
            pugi::xml_node reference = itemGroup.append_child("Reference");
            reference.append_attribute("Include") = "DreamEngine";
            pugi::xml_node hintPath = reference.append_child("HintPath");
            hintPath.text().set((editorPath + "\\DreamEngine.dll").c_str());

            doc.save_file(csprojPath.c_str());

            return true;
        }
    }

    return false;
}
