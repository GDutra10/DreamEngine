#include "../../Core/Application.h"
#include "../../Core/ECS/Components/ChildrenComponent.h"
#include "../../Core/ECS/Components/UiComponent.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../../Core/Resources/ResourceManager.h"
#include "EditorDefine.h"
#include "Helpers/FileHelper.h"
#include "EditorContext.h"
#include "Render/RenderViewProvider.h"
#include "Models/Datas/SceneData.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::Models::Datas;

EditorContext::EditorContext(ProjectConfiguration& projectConfig, EditorConfiguration& editorConfig, Scene* pEditorScene)
    : m_editorLogger(new EditorLogger()), m_projectConfig(projectConfig), m_editorConfig(editorConfig), m_pEntityManager(pEditorScene->GetEntityManager()), m_pEditorScene(pEditorScene)
{
    LoggerSingleton::Instance().Attach(m_editorLogger);
}

DreamEngine::Editor::Models::Datas::SceneData* DreamEngine::Editor::EditorContext::GetSceneData() const
{
    return m_sceneData;
}

ProjectConfiguration& EditorContext::GetProjectConfiguration() const
{
    return m_projectConfig;
}

const path& EditorContext::GetSelectedPath() const
{
    return m_selectedPath;
}

const std::string& EditorContext::GetSelectedPathByProject() const
{
    return m_selectedPathByProject;
}

const path& EditorContext::GetSelectedScenePath() const
{
    return m_selectedScenePath;
}

const path& EditorContext::GetSelectedMaterialPath() const
{
    return m_selectedMaterialPath;
}

const path& EditorContext::GetFileTextEditorPath() const
{
    return m_selectedFileTextEditorPath;
}

EntityManager* EditorContext::GetEntityManager() const
{
    return m_pEntityManager;
}

Entity* EditorContext::GetSelectedEntity() const
{
    return m_pSelectedEntity;
}

Scene* EditorContext::GetEditorScene() const
{
    return m_pEditorScene;
}

bool EditorContext::IsViewSceneData() const
{
    return m_isViewSceneData;
}

EditorLogger* EditorContext::GetEditorLogger() const
{
    return m_editorLogger;
}

RenderView* EditorContext::GetSceneRenderView() const
{
    return m_pSceneRenderView;
}

RenderView* EditorContext::GetGameRenderView() const
{
    return m_pGameRenderView;
}

void EditorContext::SetSelectedPath(const path& path)
{
    m_selectedPath = path;
    m_selectedPathByProject = m_selectedPath.string().substr(m_projectConfig.projectPath.size());
}

void EditorContext::SetEntityManager(EntityManager* entityManager)
{
    m_pEntityManager = entityManager;
    m_pEntityManager->Reset();
}

void EditorContext::SetSelectedEntity(Entity* entity)
{
    m_isViewSceneData = false;
    m_pSelectedEntity = entity;
}

void EditorContext::SetProjectConfiguration(const ProjectConfiguration& projectConfig)
{
    m_projectConfig = projectConfig;
}

void EditorContext::SetSceneRenderView(RenderView* renderView)
{
    m_pSceneRenderView = renderView;
    RenderViewProvider::Add(m_pSceneRenderView);
}

void EditorContext::SetGameRenderView(RenderView* renderView)
{
    m_pGameRenderView = renderView;
    RenderViewProvider::Add(m_pGameRenderView);
}
