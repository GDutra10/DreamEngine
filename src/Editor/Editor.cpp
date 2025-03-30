// Editor.cpp : Defines the entry point for the application.
//
#include "Editor.h"

#include <thread>

#include "EditorScene.h"
#include "../Core/Application.h"
#include "../Profiling/Profiler.h"
#include "../Core/Timer.h"
#include "../Core/Loggers/LoggerSingleton.h"
#include "../Core/Loggers/ConsoleLogger.h"
#include "Vendors/imgui/imgui.h"
#include "Loggers/EditorLogger.h"
#include "Models/EditorConfiguration.h"
#include "Models/ProjectConfiguration.h"
#include "Singletons/EditorSingleton.h"


using namespace std;
using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::Loggers;
using namespace DreamEngine::Editor::Singletons;

//void some_function()
//{
//    PROFILE_FUNCTION()
//}
//
//void iterator_function()
//{
//    PROFILE_FUNCTION()
//
//    for (auto i = 0; i < 10; i++) 
//    {
//        PROFILE_SCOPE("LoopInteration")
//        std::this_thread::sleep_for(100ms);
//    }
//}

int main()
{
    // TODO: came in args
    //std::string projectPath = "C:/Users/gui_d/source/cplusplus/DreamEngine/out/build/x64-debug/src/Editor/Projects/MyAwesomeProject/"; // TODO: parametrized this
    //std::string projectName = "MyAwesomeProject.deproj"; // TODO: parametrized this
    EditorConfiguration editorConfiguration = EditorConfiguration::LoadEditorConfiguration();
    ProjectConfiguration projectConfiguration = {}; //ProjectConfiguration::LoadProjectConfiguration(projectPath, projectName);

    // adding logs
    #ifdef _DEBUG
        LoggerSingleton::Instance().Attach(new ConsoleLogger());
    #endif

    // init game
    std::map<std::string, Scene*> scenes;
    auto editorScene = new EditorScene(
        projectConfiguration.projectName,
        projectConfiguration,
        editorConfiguration
    );

    editorScene->SetShowCursor(true);
    scenes.emplace("editor", editorScene);
    const auto gameEditor = new Game(editorConfiguration.windowSize.x,
                                     editorConfiguration.windowSize.y,
                                    projectConfiguration.csProjectDebugPath,
                                    scenes);

    // run application
    Application::Instance().Run(
        editorConfiguration.windowSize.x, 
        editorConfiguration.windowSize.y,
        "DreamEngine Editor - " + projectConfiguration.projectName, 
        openGL,
        gameEditor);

	return 0;
}
