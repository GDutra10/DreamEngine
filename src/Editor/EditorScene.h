#ifndef EDITOR_EDITOR_SCENE_H_
#define EDITOR_EDITOR_SCENE_H_

#include <string>
#include "../Core/GameSystem/Game.h"
#include "../Core/GameSystem/Scene.h"
#include "Controllers/CameraEditorController.h"
#include "Vendors/imgui/imgui.h"
#include "Models/ProjectConfiguration.h"
#include "Models/EditorConfiguration.h"
#include "UI/Modals/OpenProjectModal.h"
#include "UI/Windows/HierarchyWindow.h"
#include "UI/Windows/LoggerWindow.h"
#include "UI/Windows/ProjectWindow.h"
#include "UI/Windows/PropertyWindow.h"
#include "UI/Windows/SceneWindow.h"
#include "UI/Windows/FileExplorerWindow.h"
#include "UI/Windows/ResourceManagerWindow.h"
#include "UI/Windows/MaterialWindow.h"

namespace DreamEngine::Editor
{
using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::UI::Modals;
using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Core::GameSystem;

class EditorScene final : public Scene
{
public:
    explicit EditorScene(
        std::string name,
        ProjectConfiguration& projectConfig,
        EditorConfiguration& editorConfig);

    void Initialize() override;
    void Unload() override;
    void Update(float deltaTime) override;

   private:
    ImGuiIO* m_io = nullptr;
    HierarchyWindow m_hierarchyWindow;
    LoggerWindow m_loggerWindow;
    PropertyWindow m_propertyWindow;
    ProjectWindow m_projectWindow;
    SceneWindow m_sceneWindow;
    FileExplorerWindow m_fileExplorerWindow;
    ResourceManagerWindow m_resourceManagerWindow;
    MaterialWindow m_materialWindow;
    OpenProjectModal m_openProjectModal;
    CameraEditorController* m_pCameraEditorController;
    ProjectConfiguration& GetProjectConfiguration() const;
    void InitializeImGui();
    void DrawMenuBar();
    void StartImGuiFrame();
    void FinishImGuiFrame();
    void UpdateBackgroundColor() const;
    void SetStyleOne();
    void SetStyleUnreal();
    void SetStyleUnreal2();
    void SetStyleEngine();
};

}  // namespace DreamEngine::Editor::Loggers
#endif