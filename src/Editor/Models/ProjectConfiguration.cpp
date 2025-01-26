#include "ProjectConfiguration.h"
#include <fstream>
#include <iostream>

using namespace DreamEngine::Editor::Models;

ProjectConfiguration ProjectConfiguration::LoadProjectConfiguration(const std::string& projectPath, const std::string& projectName)
{
    ProjectConfiguration projectConfiguration;
    projectConfiguration.projectPath = projectPath;

    std::ifstream stream(projectPath + projectName);
    std::string temp;

    if (!stream)
        throw std::exception("Could not open file");

    while (std::getline(stream, temp))
    {
        if (temp.find("ProjectName") != std::string::npos)
            projectConfiguration.projectName = temp.substr(temp.find(":") + 2);

        if (temp.find("CSSolution") != std::string::npos)
            projectConfiguration.csSolution = temp.substr(temp.find(":") + 2);

        if (temp.find("CSProjectPath") != std::string::npos)
            projectConfiguration.csProjectPath = temp.substr(temp.find(": ") + 2);

        if (temp.find("CSProjectDebugPath") != std::string::npos)
            projectConfiguration.csProjectDebugPath = temp.substr(temp.find(": ") + 2);

        if (temp.find("CSProjectDebugDll") != std::string::npos)
            projectConfiguration.csProjectDebugDll = temp.substr(temp.find(": ") + 2);

        if (temp.find("Scenes") != std::string::npos)
        {
            while (std::getline(stream, temp))
            {
                if (temp.find("}") != std::string::npos)
                    break;

                const auto delimiterPos = temp.find(':');
                auto sceneName = temp.substr(0, delimiterPos);
                auto scenePath = temp.substr(delimiterPos + 1);

                // Trim whitespace from sceneName and scenePath
                sceneName.erase(0, sceneName.find_first_not_of(" \t"));
                sceneName.erase(sceneName.find_last_not_of(" \t") + 1);
                scenePath.erase(0, scenePath.find_first_not_of(" \t"));
                scenePath.erase(scenePath.find_last_not_of(" \t") + 1);

                projectConfiguration.sceneDefinitions[sceneName] = scenePath;
            }
        }
    }

    return projectConfiguration;
}

/***
// File Example
ProjectName: MyAwesomeProject

Scenes: {
    Scene1: path/to/scene1
    Scene2: path/to/scene2
    Scene3: path/to/scene3
}
 ***/