#ifndef EDITOR_CONTROLLERS_SCENE_CONTROLLER_H_
#define EDITOR_CONTROLLERS_SCENE_CONTROLLER_H_

#include <filesystem>
#include "../../Core/GameSystem/Camera.h"
#include "../Models/Datas/SceneData.h"
#include "../../Core/ECS/EntityManager.h"

namespace DreamEngine::Editor::Controllers
{
using namespace std::filesystem;
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Editor::Models::Datas;

class SceneController
{
   public:
    static bool ShouldLoadSceneData(path& path);
    static void LoadSceneData(path& path, EntityManager* entityManager);
    static void SaveSceneData(EntityManager* entityManager);
   private:
    /*static SceneData* ReadSceneFile(const std::string& filePath);*/
};

}  // namespace DreamEngine::Editor::Controllers
#endif