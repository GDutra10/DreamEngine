#ifndef EDITOR_CONTROLLERS_CAMERA_EDITOR_CONTROLLER_H_
#define EDITOR_CONTROLLERS_CAMERA_EDITOR_CONTROLLER_H_

#include "../../Core/GameSystem/Camera.h"
#include "../EditorContext.h"

namespace DreamEngine::Editor::Controllers
{
using namespace DreamEngine::Core::GameSystem;

class CameraEditorController
{
   public:
    float yaw = -90.0f;
    float pitch = 0.0f;
    float cameraSpeed = 2.0f;
    float mouseSensitivity = 0.05f;

    CameraEditorController(EditorContext& editorContext);
    ~CameraEditorController();

    void Update(bool isSceneWindowFocused);
    Camera* GetCamera() const;

    private:
     EditorContext& m_editorContext;
     Camera* m_pCamera;
     glm::vec2 m_lastMousePositionWhenRightClick;
     void Move(float deltaTime) const;
     void Rotate();
     void UpdateCameraVectors() const;
};

}  // namespace DreamEngine::Editor::Helpers
#endif