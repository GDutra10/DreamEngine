#include "GameWindow.h"

#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Singletons;

GameWindow::GameWindow(const string& title) : BaseWindow(title) {}

void GameWindow::DrawContent()
{
    if (ImGui::BeginChild("game.image", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImGui::SetCursorPos({0, 0});
        ImGui::Image((void*)(intptr_t)EditorSingleton::Instance().GetGameFbo()->textureColorBuffer, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::EndChild();
}