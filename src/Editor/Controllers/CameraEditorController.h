#ifndef EDITOR_CONTROLLERS_CAMERA_EDITOR_CONTROLLER_H_
#define EDITOR_CONTROLLERS_CAMERA_EDITOR_CONTROLLER_H_

#include "../../Core/GameSystem/Camera.h"

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

    CameraEditorController(Camera& camera);

    void Update(bool isSceneWindowFocused);

    private:
     Camera& m_camera;
     static glm::vec2 m_sLastMousePositionWhenRightClick;
     void Move(float deltaTime) const;
     void Rotate();
     void UpdateCameraVectors() const;
};

}  // namespace DreamEngine::Editor::Helpers
#endif