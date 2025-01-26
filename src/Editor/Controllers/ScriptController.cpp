#include "ScriptController.h"

#include <string>
#include "../Singletons/EditorSingleton.h"
#include "../../Core/Application.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../../Core/ECS/Components/ScriptComponent.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Singletons;

void ScriptController::ReloadScripts()
{
    LoggerSingleton::Instance().LogTrace("ScriptController::ReloadScripts -> Start");

    if (!BuildSolution())
        return;

    ProjectConfiguration projectConfig = EditorSingleton::Instance().GetProjectConfiguration();
    ScriptEngine* scriptEngine = Application::Instance().GetScriptEngine();

    for (Entity* entity : EditorSingleton::Instance().GetEntityManager()->GetEntities())
    {
        ScriptComponent& scriptComponent = entity->GetComponent<ScriptComponent>();

        if (scriptComponent.has && scriptComponent.script != nullptr)
            scriptComponent.instance = scriptComponent.script->Destroy(scriptComponent.instance);
    }

    scriptEngine->UnloadAssembly();
    scriptEngine->LoadAssembly(projectConfig.csProjectDebugPath + "\\" + projectConfig.csProjectDebugDll);

    std::vector<ScriptInfo> scriptInfo = scriptEngine->GetClassInfoList();
    EditorSingleton::Instance().GetResourceController().AddScripts(scriptInfo);

    LoggerSingleton::Instance().LogTrace("ScriptController::ReloadScripts -> Finish");
}

bool ScriptController::BuildSolution()
{
    LoggerSingleton::Instance().LogTrace("ScriptController::BuildSolution -> Start");

    std::string command = "dotnet build ";
    auto& projectConfig = EditorSingleton::Instance().GetProjectConfiguration();

    command += projectConfig.csProjectPath + "\\" + projectConfig.csSolution;

    // Execute the command
    int result = std::system(command.c_str());

    if (result == 0)
    {
        LoggerSingleton::Instance().LogInfo("Build succeeded!");

        return true;
    }
    
    LoggerSingleton::Instance().LogError("Build failed with error code: " + result);

    return false;
}
