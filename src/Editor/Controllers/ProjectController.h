#ifndef EDITOR_CONTROLLERS_PROJECT_CONTROLLER_H_
#define EDITOR_CONTROLLERS_PROJECT_CONTROLLER_H_

#include "../Models/ProjectConfiguration.h"
#include "../Models/Result.h"
#include <filesystem>
#include "../EditorContext.h"
#include "ResourceController.h"
#include "ScriptController.h"

namespace DreamEngine::Editor::Controllers
{
    using namespace DreamEngine::Editor::Models;
class ProjectController
{
   public:
    ProjectController(EditorContext& editorContext, ResourceController& resourceController, ScriptController& scriptController);
    ResultWithData<ProjectConfiguration> CreateProjectConfiguration(std::filesystem::path selectedPath, std::string projectName);
    void LoadProjectConfiguration();
    void LoadProjectConfiguration(ProjectConfiguration& projectConfiguration);
   private:
    EditorContext& m_editorContext;
    ResourceController& m_resourceController;
    ScriptController& m_scriptController;
    void LoadDefaultResources();
    void LoadResourcesFromProject(const ProjectConfiguration& projectConfiguration);
    bool TryAddDreamEngineReferenceInCsproj(const std::string& projectPath, const std::string& projectName);

};

}  // namespace DreamEngine::Editor::Controllers
#endif