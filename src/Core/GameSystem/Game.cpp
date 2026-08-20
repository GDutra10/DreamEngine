#include "Game.h"

#include "../Loggers/LoggerSingleton.h"
#include "../Systems/AudioSystem.h"
#include "../Audio/Backends/MiniaudioBackend.h"

using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::Loggers;

Game::Game(const int width, const int height, const std::string& path, std::map<std::string, Scene*> scenes) : 
    width(width),
    height(height),
    path(path),
    m_scenes(std::move(scenes))
{
    auto* audioBackend = new DreamEngine::Core::Audio::Backends::MiniaudioBackend();

    if (audioBackend->Initialize())
        AudioSystem::Instance().SetBackend(audioBackend);
}

bool Game::ChangeActiveScene()
{
    return ChangeActiveScene(m_scenes.begin()->first);
}

bool Game::ChangeActiveScene(const std::string& sceneName)
{
    if (const auto it = m_scenes.find(sceneName); it != m_scenes.end())
    {
        if (m_activeScene != nullptr)
            m_activeScene->Unload();

        m_activeScene = it->second;
        m_activeScene->Initialize();

        return true;
    }
    
    LoggerSingleton::Instance().LogWarning("Scene '" + sceneName + "'not found");
    
    return false;
}
