#include <iostream>
#include <GameSystem/Game.h>
#include <Application.h>
#include <Loggers/LoggerSingleton.h>
#include <Loggers/ConsoleLogger.h>

#include "FirstScene.h"

// force to use gpu
extern "C"
{
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::Loggers;

int main()
{
#ifdef _DEBUG
    LoggerSingleton::Instance().Attach(new ConsoleLogger());
#endif

    std::map<std::string, Scene*> scenes;

    Scene* firstScene = new FirstScene("FirstScene");
    firstScene->SetShowCursor(true);
    scenes.emplace("FirstScene", firstScene);
    
    Game* game = new Game(900, 600, "", scenes);
    game->hasScriptEngine = false;

    ApplicationOptions applicationOptions = {};
    applicationOptions.renderType = RenderType::openGL;
    applicationOptions.windowConfiguration.width = game->width;
    applicationOptions.windowConfiguration.height = game->height;
    applicationOptions.windowConfiguration.title = "Native Game";
    applicationOptions.windowConfiguration.mode = WindowMode::Fullscreen;

    Application::Instance().Run(game, applicationOptions);
}