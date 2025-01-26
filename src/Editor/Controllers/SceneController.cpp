#include "SceneController.h"

#include <fstream>
#include <regex>

#include "../Singletons/EditorSingleton.h"
#include "../../Core/Application.h"
#include "../../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Singletons;

bool SceneController::ShouldLoadSceneData(path& path)
{
    // Check if path is scene
    return path != EditorSingleton::Instance().GetSelectedScenePath();
}

void SceneController::LoadSceneData(path& path, EntityManager* entityManager)
{
    entityManager->Reset();
    //delete EditorSingleton::Instance().sceneData;  // ?Delete previous scene data?
    SceneData* sceneData = ReadSceneFile(path.string());
    EditorSingleton::Instance().sceneData = sceneData;
    Scene* scene = Application::Instance().GetGame()->GetActiveScene();
    Camera& camera = scene->GetCamera();

    // set camera props
    camera.fovDegree = sceneData->config.camera.fovDegree;
    camera.far = sceneData->config.camera.far;
    camera.front = sceneData->config.camera.front;
    camera.near = sceneData->config.camera.near;
    camera.position = sceneData->config.camera.position;
    camera.up = sceneData->config.camera.up;

    for (EntityConfig& entityConfig : EditorSingleton::Instance().sceneData->entities)
    {
        LoggerSingleton::Instance().LogInfo("Loading Entity: " + entityConfig.name);

        Entity* entity = entityManager->AddEntity(entityConfig.tag);
        entity->GetName().assign(entityConfig.name);

        for (const string& component : entityConfig.components)
        {
            // TODO: set components
            if (component == "Transform")
            {
                TransformComponent& transformComponent = entity->GetComponent<TransformComponent>();
                transformComponent.has = true;
                transformComponent.SetPosition(entityConfig.transform.GetPosition());
                transformComponent.SetScale(entityConfig.transform.GetScale());
                transformComponent.SetRotation(entityConfig.transform.GetRotation());
            }
        }
    }
}

SceneData* SceneController::ReadSceneFile(const std::string& filePath)
{
    SceneData* sceneData = new SceneData();
    std::ifstream file(filePath);
    std::string line;
    std::regex sceneRegex(R"(\s*Scene\s*\{\s*)");
    std::regex backgroundColorRegex(R"(\s*BackgroundColor:\s*\(([^,]+),\s*([^,]+),\s*([^\)]+)\)\s*)");
    std::regex cameraRegex(R"(\s*Camera\s*\{\s*)");
    std::regex entityRegex(R"(\s*Entity\s*\{\s*)");
    std::regex idRegex(R"(\s*id:\s*(\d+)\s*)");
    std::regex tagRegex(R"(\s*tag:\s*\"([^\"]+)\"\s*)");
    std::regex nameRegex(R"(\s*name:\s*\"([^\"]+)\"\s*)");
    std::regex componentRegex(R"(\s*(Transform|Mesh|AI)\s*\{\s*)");
    std::regex positionRegex(R"(\s*position:\s*\(([^,]+),\s*([^,]+),\s*([^\)]+)\)\s*)");
    std::regex scaleRegex(R"(\s*scale:\s*\(([^,]+),\s*([^,]+),\s*([^\)]+)\)\s*)");
    std::regex rotationRegex(R"(\s*rotation:\s*\(([^,]+),\s*([^,]+),\s*([^\)]+)\)\s*)");
    std::regex frontRegex(R"(\s*front:\s*\(([^,]+),\s*([^,]+),\s*([^\)]+)\)\s*)");
    std::regex upRegex(R"(\s*up:\s*\(([^,]+),\s*([^,]+),\s*([^\)]+)\)\s*)");
    std::regex fovRegex(R"(\s*fov:\s*\"([^\"]+)\"\s*)");
    std::regex nearRegex(R"(\s*near:\s*\"([^\"]+)\"\s*)");
    std::regex farRegex(R"(\s*far:\s*\"([^\"]+)\"\s*)");

    EntityConfig currentEntity;
    bool inEntity = false;
    bool inCamera = false;

    while (std::getline(file, line))
    {
        std::smatch match;
        if (std::regex_match(line, sceneRegex))
        {
            // Scene configuration block
            while (std::getline(file, line) && line.find('}') == std::string::npos)
            {
                if (std::regex_match(line, match, backgroundColorRegex))
                {
                    sceneData->config.backgroundColor = glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));
                }
                else if (std::regex_match(line, cameraRegex))
                {
                    inCamera = true;
                    sceneData->config.camera = Camera();
                }
                else if (inCamera)
                {
                    if (std::regex_match(line, match, positionRegex))
                    {
                        sceneData->config.camera.position = glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));
                    }
                    else if (std::regex_match(line, match, frontRegex))
                    {
                        sceneData->config.camera.front = glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));
                    }
                    else if (std::regex_match(line, match, upRegex))
                    {
                        sceneData->config.camera.up = glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));
                    }
                    else if (std::regex_match(line, match, fovRegex))
                        sceneData->config.camera.fovDegree = std::stof(match[1].str());
                    else if (std::regex_match(line, match, nearRegex))
                        sceneData->config.camera.near = std::stof(match[1].str());
                    else if (std::regex_match(line, match, farRegex))
                        sceneData->config.camera.far = std::stof(match[1].str());
                    else if (line.find('}') != std::string::npos)
                        inCamera = false;
                }
            }
        }
        else if (std::regex_match(line, entityRegex))
        {
            inEntity = true;
            currentEntity = EntityConfig();  // Reset for new entity
        }
        else if (inEntity)
        {
            if (std::regex_match(line, match, idRegex))
            {
                currentEntity.id = std::stoul(match[1].str());
            }
            else if (std::regex_match(line, match, tagRegex))
            {
                currentEntity.tag = match[1].str();
            }
            else if (std::regex_match(line, match, nameRegex))
            {
                currentEntity.name = match[1].str();
            }
            else if (std::regex_match(line, match, componentRegex))
            {
                std::string componentType = match[1].str();

                if (componentType == "Transform")
                {
                    currentEntity.components.push_back(componentType);

                    while (std::getline(file, line) && line.find('}') == std::string::npos)
                    {
                        if (std::regex_match(line, match, positionRegex))
                        {
                            currentEntity.transform.SetPosition(glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str())));
                        }
                        else if (std::regex_match(line, match, scaleRegex))
                        {
                            currentEntity.transform.SetScale(glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str())));
                        }
                        else if (std::regex_match(line, match, rotationRegex))
                        {
                            currentEntity.transform.SetRotation(glm::vec3(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str())));
                        }
                    }
                }
            }
            else if (line.find('}') != std::string::npos)
            {
                sceneData->entities.push_back(currentEntity);
                inEntity = false;
            }
        }
    }

    file.close();
    return sceneData;
}