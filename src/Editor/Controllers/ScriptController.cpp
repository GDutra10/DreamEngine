#include "ScriptController.h"

#include <string>

#include "DotNetCliController.h"
#include "../EditorDefine.h"
#include "../../Core/Application.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../../Core/ECS/Components/ScriptComponent.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor::Controllers;

DreamEngine::Editor::Controllers::ScriptController::ScriptController(EditorContext& editorContext, ResourceController& resourceController) 
    : m_editorContext(editorContext)
    , m_resourceController(resourceController) {}

void ScriptController::ReloadScripts()
{
    LoggerSingleton::Instance().LogTrace("ScriptController::ReloadScripts -> Start");

    if (!BuildSolution())
        return;

    ProjectConfiguration projectConfig = m_editorContext.GetProjectConfiguration();
    ScriptEngine* scriptEngine = Application::Instance().GetScriptEngine();

    scriptEngine->UnloadAssembly();
    scriptEngine->LoadAssembly(projectConfig.csProjectDebugPath + "\\" + projectConfig.csProjectDebugDll);

    std::vector<ScriptInfo> scriptInfo = scriptEngine->GetClassInfoList();
    m_resourceController.AddScripts(scriptInfo);

    LoggerSingleton::Instance().LogTrace("ScriptController::ReloadScripts -> Finish");
}

bool ScriptController::BuildSolution()
{
    LoggerSingleton::Instance().LogTrace("ScriptController::BuildSolution -> Start");

    const auto& projectConfig = m_editorContext.GetProjectConfiguration();
    const int result = DotNetCliController::Build(projectConfig.csProjectPath, projectConfig.csSolution);

    if (result == EDITOR_DOTNET_CLI_COMMAND_RESULT_SUCCESS)
    {
        LoggerSingleton::Instance().LogInfo("Build succeeded!");

        return true;
    }
    
    LoggerSingleton::Instance().LogError("Build failed with error code: " + std::to_string(result));

    return false;
}
