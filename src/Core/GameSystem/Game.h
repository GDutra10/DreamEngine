#ifndef CORE_GAME_SYSTEM_GAME_H_
#define CORE_GAME_SYSTEM_GAME_H_

#include <string>
#include <map>
#include "Scene.h"

namespace DreamEngine::Core::GameSystem
{

class Game
{
   public:
    int width;
    int height;
    std::string path;
    bool hasScriptEngine = true;
    Game(const int width, const int height, const std::string& path, std::map<std::string, Scene*> scenes)
        : width(width), height(height), path(path), m_scenes(std::move(scenes)){}
    Scene* GetActiveScene() { return m_activeScene; }
    void ChangeActiveScene();
    void ChangeActiveScene(const std::string& sceneName);

   private:
    Scene* m_activeScene = nullptr;
    std::map<std::string, Scene*> m_scenes;
};
}  // namespace DreamEngine::Core::GameSystem
#endif