#ifndef EDITOR_EDITOR_SINGLETONS_EDITOR_SINGLETON_H_
#define EDITOR_EDITOR_SINGLETONS_EDITOR_SINGLETON_H_

#include <filesystem>

#include "../Controllers/ProjectController.h"
#include "../Controllers/ResourceController.h"
#include "../Controllers/ScriptController.h"
#include "../Controllers/CameraEditorController.h"
#include "../Models/ProjectConfiguration.h"
#include "../Models/EditorConfiguration.h"
#include "../Models/Datas/SceneData.h"
#include "../Loggers/EditorLogger.h"
#include "../../Core/GameSystem/Scene.h"
#include "../../Core/Render/FrameBuffer.h"

namespace DreamEngine::Editor::Singletons
{
    using namespace DreamEngine::Editor::Controllers;
    using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::Models::Datas;
    using namespace DreamEngine::Editor::Loggers;
    using namespace DreamEngine::Core::ECS;
    using namespace DreamEngine::Core::Render;
    using namespace std::filesystem;

class EditorSingleton final
{
   public:
    SceneData* sceneData = nullptr;
    static EditorSingleton& Instance();
    static void Initialize(ProjectConfiguration& projectConfig, EditorConfiguration& editorConfig, Scene* pEditorScene);
    [[nodiscard]] ProjectConfiguration& GetProjectConfiguration() const;
    [[nodiscard]] path GetSelectedPath() const;
    [[nodiscard]] std::string GetSelectedPathByProject() const;
    [[nodiscard]] path GetSelectedScenePath() const;
    [[nodiscard]] path GetSelectedMaterialPath() const;
    [[nodiscard]] EntityManager* GetEntityManager() const;
    [[nodiscard]] Entity* GetSelectedEntity() const;
    [[nodiscard]] Scene* GetEditorScene() const;
    [[nodiscard]] bool IsViewSceneData() const;
    [[nodiscard]] EditorLogger* GetEditorLogger() const;
    [[nodiscard]] ProjectController& GetProjectController();
    [[nodiscard]] ResourceController& GetResourceController();
    [[nodiscard]] ScriptController& GetScriptController();
    [[nodiscard]] CameraEditorController& GetCameraEditorController();
    [[nodiscard]] RenderView* GetSceneRenderView() const;
    [[nodiscard]] RenderView* GetGameRenderView() const;
    void SetSelectedPath(const path& path);
    void SetSelectedScenePath(const path& scene) { m_selectedScenePath = scene; }
    void SetSelectedMaterialPath(const path& material) { m_selectedMaterialPath = material; }
    void SetEntityManager(EntityManager* entityManager);
    void SetSelectedEntity(Entity* entity);
    void SetIsViewSceneData(bool value) { m_isViewSceneData = value; }
    void SetProjectConfiguration(const ProjectConfiguration& projectConfig) const;
    void SetSceneRenderView(RenderView* renderView);
    void SetGameRenderView(RenderView* renderView);
   private:
    EditorSingleton(ProjectConfiguration& projectConfig, EditorConfiguration& editorConfig, Scene* pEditorScene);
    static EditorSingleton* m_sEditorSingleton;
    EditorLogger* m_editorLogger;
    ProjectConfiguration& m_projectConfig;
    EditorConfiguration& m_editorConfig;
    path m_selectedPath;
    path m_selectedScenePath;
    path m_selectedMaterialPath;
    EntityManager* m_pEntityManager;
    Entity* m_pSelectedEntity = nullptr;
    Scene* m_pEditorScene;
    bool m_isViewSceneData = false;
    std::string m_selectedPathByProject;
    ProjectController m_projectController;
    ResourceController m_resourceController;
    ScriptController m_scriptController;
    CameraEditorController m_cameraEditorController;
    RenderView* m_pSceneRenderView = nullptr;
    RenderView* m_pGameRenderView = nullptr;
    void SetOutlineComponent(Entity* entity, bool hasComponent);
};

}  // namespace DreamEngine::Editor
#endif