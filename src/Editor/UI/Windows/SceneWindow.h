#ifndef EDITOR_UI_WINDOWS_SCENE_WINDOW_H_
#define EDITOR_UI_WINDOWS_SCENE_WINDOW_H_

#include <functional>

#include "BaseWindow.h"

#include "../../Vendors/imgui/imgui.h"
#include "../../Vendors/imgui/ImGuizmo.h"
#include "../../Controllers/CameraEditorController.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std;
using namespace DreamEngine::Editor::Controllers;

class SceneWindow : public BaseWindow
{
   public:
    SceneWindow(const string& title, EditorContext& editorContext, CameraEditorController& cameraEditorController);

   protected:
    void DrawContent() override;

   private:
    CameraEditorController& m_cameraEditorController;
    ImVec2 m_availableSize = ImVec2(0, 0);
    ImVec2 m_windowPosition = ImVec2(0, 0);
    ImVec2 m_sceneResolution = ImVec2(1920.f, 1080.f);
    int m_currentGizmoOperation = ImGuizmo::TRANSLATE;
    void ImGizmoRender();
    void SelectGizmoOperation(const int gizmoOperation);
    void OperationButton(const int gizmoOperation, const function<bool()>& buttonFunc);
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif