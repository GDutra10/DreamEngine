#ifndef EDITOR_CONTROLLERS_SCENE_CONTROLLER_H_
#define EDITOR_CONTROLLERS_SCENE_CONTROLLER_H_

#include <filesystem>
#include "../../Core/GameSystem/Camera.h"
#include "../Models/SceneData.h"
#include "../../Core/ECS/EntityManager.h"

namespace DreamEngine::Editor::Controllers
{
using namespace std::filesystem;
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Editor::Models;

class SceneController
{
   public:
    bool ShouldLoadSceneData(path& path);
    void LoadSceneData(path& path, EntityManager* entityManager);
   private:
    SceneData* ReadSceneFile(const std::string& filePath);
};

}  // namespace DreamEngine::Editor::Controllers
#endif