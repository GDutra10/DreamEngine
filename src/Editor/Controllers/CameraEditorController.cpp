#include "CameraEditorController.h"
#include "../Singletons/EditorSingleton.h"
#include "../../Core/Application.h"
#include "../../Core/Inputs/Input.h"
#include "../../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Singletons;
using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Inputs;

glm::vec2 CameraEditorController::m_sLastMousePositionWhenRightClick;

CameraEditorController::CameraEditorController(Camera& camera) : m_camera(camera)
{}

void CameraEditorController::Update(const bool isSceneWindowFocused)
{
    UpdateCameraVectors();

    if (!isSceneWindowFocused)
        return;

    const float deltaTime = Application::Instance().GetDeltaTime();

    Move(deltaTime);
    Rotate();
}

void CameraEditorController::Move(const float deltaTime) const
{
    if (!Input::IsPressed(MouseButton::MOUSE_BUTTON_RIGHT))
        return;

    if (Input::IsPressed(Key::W))
    {
        // forward
        const glm::vec3 normalizedFront = glm::normalize(m_camera.front);
        m_camera.position += normalizedFront * cameraSpeed * deltaTime;
    }

    if (Input::IsPressed(Key::S))
    {
        // backward
        const glm::vec3 normalizedFront = glm::normalize(m_camera.front);
        m_camera.position += normalizedFront * -cameraSpeed * deltaTime;
    }

    if (Input::IsPressed(Key::A))
    {
        // move left
        const glm::vec3 left = glm::normalize(glm::cross(m_camera.up, m_camera.front));
        m_camera.position += left * cameraSpeed * deltaTime;
    }

    if (Input::IsPressed(Key::D))
    {
        // move right
        const glm::vec3 left = glm::normalize(glm::cross(m_camera.up, m_camera.front));
        m_camera.position += left * -cameraSpeed * deltaTime;
    }
}

void CameraEditorController::Rotate()
{
    Scene* editorScene = EditorSingleton::Instance().GetEditorScene();

    if (Input::IsPressed(MouseButton::MOUSE_BUTTON_RIGHT))
    {
        if (editorScene->GetShowCursor())
            editorScene->SetShowCursor(false);

        const glm::vec2 currentMousePosition = Input::GetMousePosition();

        if (m_sLastMousePositionWhenRightClick == currentMousePosition)
            return;

        m_sLastMousePositionWhenRightClick = currentMousePosition;

        const float xOffset = currentMousePosition.x * mouseSensitivity;
        const float yOffset = currentMousePosition.y * mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Calculate the new front vector
        glm::vec3 frontVec;
        frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        frontVec.y = sin(glm::radians(pitch));
        frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        m_camera.front = glm::normalize(frontVec);

        // Recalculate the right and up vector
        const glm::vec3 right = glm::normalize(glm::cross(m_camera.front, m_camera.up));
        m_camera.up = glm::normalize(glm::cross(right, m_camera.front));
    }
    else
    {
        if (!editorScene->GetShowCursor())
            editorScene->SetShowCursor(true);
    }
}

void CameraEditorController::UpdateCameraVectors() const
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_camera.right = glm::normalize(
        glm::cross(front, m_camera.up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_camera.up = glm::normalize(glm::cross(m_camera.right, m_camera.front));
}