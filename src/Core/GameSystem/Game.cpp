#include "Game.h"

#include "../Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::Loggers;

void Game::ChangeActiveScene()
{
    ChangeActiveScene(m_scenes.begin()->first);
}

void Game::ChangeActiveScene(const std::string& sceneName)
{
    if (const auto it = m_scenes.find(sceneName); it != m_scenes.end())
    {
        if (m_activeScene != nullptr)
            m_activeScene->Unload();

        m_activeScene = it->second;
        m_activeScene->Initialize();
    }
    else
        LoggerSingleton::Instance().LogWarning("Scene not found");
}
