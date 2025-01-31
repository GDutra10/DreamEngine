#include "EditorSingleton.h"
#include "../../Core/Application.h"
#include "../../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor;
using namespace DreamEngine::Editor::Singletons;

EditorSingleton* EditorSingleton::m_sEditorSingleton = nullptr;

EditorSingleton& EditorSingleton::Instance()
{
    if (m_sEditorSingleton == nullptr)
        throw std::exception("Editor Singleton not initialized!");

    return *m_sEditorSingleton;
}

void EditorSingleton::Initialize(ProjectConfiguration& projectConfig, EditorConfiguration& editorConfig, Scene* pEditorScene)
{
    if (m_sEditorSingleton == nullptr)
        m_sEditorSingleton = new EditorSingleton(projectConfig, editorConfig, pEditorScene);

    LoggerSingleton::Instance().Attach(Instance().GetEditorLogger());
}

ProjectConfiguration& EditorSingleton::GetProjectConfiguration() const
{
    return m_projectConfig;
}

path EditorSingleton::GetSelectedPath() const
{
    return m_selectedPath;
}

std::string EditorSingleton::GetSelectedPathByProject() const
{
    return m_selectedPathByProject;
}

path EditorSingleton::GetSelectedScenePath() const
{
    return m_selectedScenePath;
}

path EditorSingleton::GetSelectedMaterialPath() const
{
    return m_selectedMaterialPath;
}

EntityManager* EditorSingleton::GetEntityManager() const
{
    return m_pEntityManager;
}

Entity* EditorSingleton::GetSelectedEntity() const
{
    return m_pSelectedEntity;
}

Scene* EditorSingleton::GetEditorScene() const
{
    return m_pEditorScene;
}

bool EditorSingleton::IsViewSceneData() const
{
    return m_isViewSceneData;
}

EditorLogger* EditorSingleton::GetEditorLogger() const
{
    return m_editorLogger;
}

ProjectController& EditorSingleton::GetProjectController()
{
    return m_projectController;
}

ResourceController& EditorSingleton::GetResourceController()
{
    return m_resourceController;
}

ScriptController& EditorSingleton::GetScriptController()
{
    return m_scriptController;
}

void EditorSingleton::SetSelectedPath(const path& path)
{
    m_selectedPath = path;
    m_selectedPathByProject = m_selectedPath.string().substr(m_projectConfig.projectPath.size());
}

void EditorSingleton::SetEntityManager(EntityManager* entityManager)
{
    m_pEntityManager = entityManager;
    m_pEntityManager->Reset();
}

void EditorSingleton::SetSelectedEntity(Entity* entity)
{
    m_pSelectedEntity = entity;
    m_isViewSceneData = false;
}

void EditorSingleton::SetProjectConfiguration(const ProjectConfiguration& projectConfig) const
{
    m_projectConfig = projectConfig;
}

EditorSingleton::EditorSingleton(ProjectConfiguration& projectConfig, EditorConfiguration& editorConfig, Scene* pEditorScene)
    : m_editorLogger(new EditorLogger()),
      m_projectConfig(projectConfig),
      m_editorConfig(editorConfig),
      m_pEntityManager(nullptr),
      m_pEditorScene(pEditorScene){}