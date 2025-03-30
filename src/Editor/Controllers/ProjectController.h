#ifndef EDITOR_CONTROLLERS_PROJECT_CONTROLLER_H_
#define EDITOR_CONTROLLERS_PROJECT_CONTROLLER_H_

#include "../Models/ProjectConfiguration.h"
#include "../Models/Result.h"
#include <filesystem>

namespace DreamEngine::Editor::Controllers
{
    using namespace DreamEngine::Editor::Models;
class ProjectController
{
   public:
    ProjectController() = default;
    ResultWithData<ProjectConfiguration> CreateProjectConfiguration(std::filesystem::path selectedPah, std::string projectName);
    void LoadProjectConfiguration();
    void LoadProjectConfiguration(ProjectConfiguration& projectConfiguration);
   private:
    void LoadDefaultResources();
    void LoadResourcesFromProject(ProjectConfiguration& projectConfiguration);
    bool TryAddDreamEngineReferenceInCsproj(const std::string& projectPath, const std::string& projectName);

};

}  // namespace DreamEngine::Editor::Controllers
#endif