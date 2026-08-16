#ifndef EDITOR_CONTROLLERS_SCENE_CONTROLLER_H_
#define EDITOR_CONTROLLERS_SCENE_CONTROLLER_H_

#include <filesystem>
#include "../../Core/GameSystem/Camera.h"
#include "../../Core/GameSystem/Definitions/SceneDefinition.h"
#include "../../Core/ECS/EntityManager.h"
#include "../EditorContext.h"

namespace DreamEngine::Editor::Controllers
{
using namespace std::filesystem;
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::GameSystem::Definitions;

class SceneController
{
   public:
    SceneController(EditorContext& editorContext);
    bool ShouldLoadSceneData(path& path);
    void LoadSceneData(path& path, bool isByChangeScene);
    bool SaveSceneData();
    void Play();
    void Stop();

private:
    EditorContext& m_editorContext;
    SceneDefinition* m_pOriginalSceneData;
    void LoadScene();
    vector<Entity*> CreateEntities();
    void SetParentAndChildren(Entity*& mainCameraEntity, vector<Entity*> entities);
    /*static SceneData* ReadSceneFile(const std::string& filePath);*/
};

}  // namespace DreamEngine::Editor::Controllers
#endif