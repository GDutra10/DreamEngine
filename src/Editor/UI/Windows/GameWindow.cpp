#include "GameWindow.h"

#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"
#include "Loggers/LoggerSingleton.h"

using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Singletons;

GameWindow::GameWindow(const string& title) : BaseWindow(title) {}

void GameWindow::DrawContent()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if (ImGui::BeginChild("game.image", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImVec2 size = ImGui::GetContentRegionAvail();

        // Avoid zero / negative size
        int desiredW = std::max(1, (int)size.x);
        int desiredH = std::max(1, (int)size.y);

        RenderView* gameRenderView = EditorSingleton::Instance().GetGameRenderView();
        gameRenderView->width = desiredW;
        gameRenderView->height = desiredH;
        gameRenderView->frameBuffer->Rescale(desiredW, desiredH);

        ImGui::SetCursorPos({0, 0});
        ImGui::Image((void*)(intptr_t)gameRenderView->frameBuffer->textureColorBuffer, size, ImVec2(0, 1), ImVec2(1, 0));  // flipped UVs

        // --- Capture image rect in screen space (top-left origin) ---
        ImVec2 imgMin = ImGui::GetItemRectMin();
        ImVec2 imgMax = ImGui::GetItemRectMax();
        ImVec2 mouse = ImGui::GetMousePos();

        if (const bool hovered = ImGui::IsItemHovered())
        {
            // Map mouse -> normalized inside image
            float u = (mouse.x - imgMin.x) / (imgMax.x - imgMin.x);
            float v = (mouse.y - imgMin.y) / (imgMax.y - imgMin.y);

            // Clamp (avoid negatives / overs)
            u = (u < 0.f) ? 0.f : (u > 1.f) ? 1.f : u;
            v = (v < 0.f) ? 0.f : (v > 1.f) ? 1.f : v;

            // Convert to FBO pixel coords
            int x = (int)(u * (float)gameRenderView->width);
            int y = (int)(v * (float)gameRenderView->height);

            // Now forward to RmlUi:
            ForwardToUi(x, y);
        }
    }

    ImGui::EndChild();
    ImGui::PopStyleVar(3);
}

void GameWindow::ForwardToUi(int x, int y)
{
    UiManager::ProcessMouseMove(x, y);

    // Send clicks
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        UiManager::ProcessMouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        UiManager::ProcessMouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0);

    // wheel
    float wheel = ImGui::GetIO().MouseWheel;
    if (wheel != 0.0f)
        UiManager::ProcessScroll((int)(wheel * 120), 0);
}