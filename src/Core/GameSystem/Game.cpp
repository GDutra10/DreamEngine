#include "Game.h"

#include "../Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::Loggers;

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
