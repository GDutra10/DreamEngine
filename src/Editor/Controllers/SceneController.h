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
    static void LoadSceneData(path& path, EntityManager* entityManager, bool isByChangeScene = false);
    static bool SaveSceneData(EntityManager* entityManager);
    static void Play(EntityManager* entityManager);
    static void Stop(EntityManager* entityManager);

private:
    static SceneData* m_pOriginalSceneData;
    static void LoadScene(EntityManager* entityManager);
    static vector<Entity*> CreateEntities(EntityManager* entityManager, SceneData* sceneData);
    static void SetParentAndChildren(const SceneData* sceneData, Entity*& mainCameraEntity, vector<Entity*> entities);
    /*static SceneData* ReadSceneFile(const std::string& filePath);*/
};

}  // namespace DreamEngine::Editor::Controllers
#endif