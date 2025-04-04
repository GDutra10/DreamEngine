#ifndef EDITOR_MODELS_PROJECT_CONFIGURATION_H_
#define EDITOR_MODELS_PROJECT_CONFIGURATION_H_

#include <map>
#include <string>

namespace DreamEngine::Editor::Models
{
struct ProjectConfiguration
{
    std::string projectName;
    std::string projectPath;
    std::string csSolution;
    std::string csProjectPath;
    std::string csProjectDebugPath;
    std::string csProjectDebugDll;
    std::map<std::string, std::string> sceneDefinitions;
    bool isLoaded = false;
};

}  // namespace DreamEngine::Editor::Models
#endif