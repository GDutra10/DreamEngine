#ifndef EDITOR_EDITOR_CONTEXT_H_
#define EDITOR_EDITOR_CONTEXT_H_

#include <filesystem>

#include "../Core/GameSystem/Scene.h"
#include "../Core/Render/FrameBuffer.h"
#include "../Core/GameSystem//Definitions/SceneDefinition.h"
#include "Loggers/EditorLogger.h"
#include "Models/EditorConfiguration.h"
#include "Models/ProjectConfiguration.h"

namespace DreamEngine::Editor
{
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::Loggers;
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::GameSystem::Definitions;
using namespace std::filesystem;

class EditorContext final
{
   public:
    EditorContext(ProjectConfiguration& projectConfig, EditorConfiguration& editorConfig, Scene* pEditorScene);
    [[nodiscard]] SceneDefinition* GetSceneData() const;
    [[nodiscard]] ProjectConfiguration& GetProjectConfiguration() const;
    [[nodiscard]] const path& GetSelectedPath() const;
    [[nodiscard]] const std::string& GetSelectedPathByProject() const;
    [[nodiscard]] const path& GetSelectedScenePath() const;
    [[nodiscard]] const path& GetSelectedMaterialPath() const;
    [[nodiscard]] const path& GetFileTextEditorPath() const;
    [[nodiscard]] EntityManager* GetEntityManager() const;
    [[nodiscard]] Entity* GetSelectedEntity() const;
    [[nodiscard]] Scene* GetEditorScene() const;
    [[nodiscard]] bool IsViewSceneData() const;
    [[nodiscard]] EditorLogger* GetEditorLogger() const;
    [[nodiscard]] RenderView* GetSceneRenderView() const;
    [[nodiscard]] RenderView* GetGameRenderView() const;
    void SetSceneData(SceneDefinition sceneData) { m_sceneData = std::make_unique<SceneDefinition>(std::move(sceneData)); }
    void SetSelectedPath(const path& path);
    void SetSelectedScenePath(const path& scene) { m_selectedScenePath = scene; }
    void SetSelectedMaterialPath(const path& material) { m_selectedMaterialPath = material; }
    void SetFileTextEditorPath(const path& codeFilePath) { m_selectedFileTextEditorPath = codeFilePath; };
    void SetEntityManager(EntityManager* entityManager);
    void SetSelectedEntity(Entity* entity);
    void SetIsViewSceneData(bool value) { m_isViewSceneData = value; }
    void SetProjectConfiguration(const ProjectConfiguration& projectConfig);
    void SetSceneRenderView(RenderView* renderView);
    void SetGameRenderView(RenderView* renderView);

   private:
    bool m_isViewSceneData = false;
    std::string m_selectedPathByProject;

    // paths
    path m_selectedPath;
    path m_selectedScenePath;
    path m_selectedMaterialPath;
    path m_selectedFileTextEditorPath;

    // pointers
    std::unique_ptr<SceneDefinition> m_sceneData = nullptr;
    EditorLogger* m_editorLogger = nullptr;
    EntityManager* m_pEntityManager = nullptr;
    Entity* m_pSelectedEntity = nullptr;
    Scene* m_pEditorScene = nullptr;
    RenderView* m_pSceneRenderView = nullptr;
    RenderView* m_pGameRenderView = nullptr;

    // references
    ProjectConfiguration& m_projectConfig;
    EditorConfiguration& m_editorConfig;
};

}  // namespace DreamEngine::Editor
#endif