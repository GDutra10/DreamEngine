#include "SceneWindow.h"

#include "../../Singletons/EditorSingleton.h"
#include "../../Core/Application.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../../Core/Math/Math.h"

#include "../../Vendors/imgui/imgui.h"
#include "../../Vendors/imgui/ImGuizmo.h"
#include "../../Vendors/glm/glm/gtc/type_ptr.hpp"
#include "../../Core/ECS/Components/ParentComponent.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Singletons;

SceneWindow::SceneWindow(const std::string& title) : BaseWindow(title){}

void SceneWindow::DrawContent()
{
    m_availableSize = ImGui::GetWindowViewport()->Size;
    m_windowPosition = ImGui::GetCursorScreenPos();

    /*ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(),
        ImGui::GetWindowHeight()*/

    //ImGui::SetCursorPos(ImVec2(0, 0));
    if (ImGui::BeginChild("viewport.image", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        //ImGui::SetCursorPos({0, 0});
        //if (ImGui::InvisibleButton("SceneImage", m_availableSize, ImGuiButtonFlags_MouseButtonLeft))
        //{
        //    // Logic for clicking on the image (e.g., selecting objects)
        //    LoggerSingleton::Instance().LogInfo("Clicked on the scene image!");
        //}

        // Invisible button to handle clicks
        //ImGui::SetItemAllowOverlap();
        ImVec2 size = ImGui::GetContentRegionAvail();
        
        // Avoid zero / negative size
        int desiredW = std::max(1, (int)size.x);
        int desiredH = std::max(1, (int)size.y);

        RenderView* sceneRenderView = EditorSingleton::Instance().GetSceneRenderView();
        sceneRenderView->width = desiredW;
        sceneRenderView->height = desiredH;
        sceneRenderView->frameBuffer->Rescale(desiredW, desiredH);

        ImGui::SetCursorPos({0, 0});
        ImGui::Image((void*)(intptr_t)sceneRenderView->frameBuffer->textureColorBuffer, size, ImVec2(0, 1), ImVec2(1, 0));

        // Check if the image is hovered
        //bool isHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

        //ImGui::SetItemAllowOverlap();
        ImGizmoRender();

        // Draw the gizmo operation buttons
        //ImGui::SetItemAllowOverlap();

        ImVec2 position = ImGui::GetWindowContentRegionMin();
        position.x += 10;
        position.y += 5;
        ImGui::SetCursorPos(position);
        ImGui::BeginGroup();
        OperationButton(-1, []() { return ImGui::Button("    h    ##scene.window.gizmo.operation.hide"); });
        ImGui::SameLine(0, 8);
        OperationButton(ImGuizmo::TRANSLATE, []() { return ImGui::Button("    T    ##scene.window.gizmo.operation.translate"); });
        ImGui::SameLine(0, 8);
        OperationButton(ImGuizmo::ROTATE, []() { return ImGui::Button("    R    ##scene.window.gizmo.operation.rotate"); });
        ImGui::SameLine(0, 8);
        OperationButton(ImGuizmo::SCALE, []() { return ImGui::Button("    S    ##scene.window.gizmo.operation.scale"); });
        ImGui::EndGroup();
        //ImGui::SetItemAllowOverlap();
    }

    ImGui::EndChild();
}

void SceneWindow::ImGizmoRender()
{
    Entity* selectedEntity = EditorSingleton::Instance().GetSelectedEntity();

    if (selectedEntity == nullptr || m_currentGizmoOperation == -1)
        return;

    Game* game = Application::Instance().GetGame();
    Camera camera = *EditorSingleton::Instance().GetCameraEditorController().GetCamera();
    TransformComponent& transformComponent = selectedEntity->GetComponent<TransformComponent>();
    ParentComponent& parentComponent = selectedEntity->GetComponent<ParentComponent>();

    // Set the ImGuizmo context
    //ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();


    auto available = ImGui::GetContentRegionAvail();
    auto windowContentMax = ImGui::GetWindowContentRegionMax();
    //LoggerSingleton::Instance().LogInfo("SceneWindow::ImGizmoRender() - x: " + std::to_string(m_windowPosition.x) + ", y: " + std::to_string(m_windowPosition.y));

    // Set the ImGuizmo window size and position
    //ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    ImGuizmo::SetRect(m_windowPosition.x, 
        m_windowPosition.y, 
        available.x, 
        windowContentMax.y);

    // Define the operation mode (translate, rotate, scale)
    glm::mat4 projection = camera.GetProjection();
    glm::mat4 transform = selectedEntity->GetTransform();

    // Flip Y-axis to fix the gizmo orientation
    //projection[1][1] *= -1;

    // just to show the guizmo
    ImGuizmo::Manipulate(
        glm::value_ptr(camera.GetView()), 
        glm::value_ptr(projection),
        static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation), 
        ImGuizmo::LOCAL, 
        glm::value_ptr(transform));

    if (ImGuizmo::IsUsing())
    {
        if (parentComponent.has && parentComponent.parent != nullptr)
        {
            glm::mat4 parentTransform = parentComponent.parent->GetTransform();
            transform = glm::inverse(parentTransform) * transform;
        }
        
        transformComponent.SetTransform(transform);    
    }
}

void SceneWindow::SelectGizmoOperation(const int gizmoOperation)
{
    m_currentGizmoOperation = gizmoOperation;
}

void SceneWindow::OperationButton(const int gizmoOperation, const function<bool()>& buttonFunc)
{
    if (m_currentGizmoOperation == gizmoOperation)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.5f, 0.2f, 1.0f));
        if (buttonFunc())
        {
            m_currentGizmoOperation = gizmoOperation;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
    else
    {
        if (buttonFunc())
        {
            m_currentGizmoOperation = gizmoOperation;
        }
    }
}
