#include "BaseWindow.h"
#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Windows;

BaseWindow::BaseWindow(const std::string& title, EditorContext& editorContext)
    : m_title(title), m_editorContext(editorContext)
{ }

void BaseWindow::Draw()
{
    ImGui::Begin(("   " + m_title).c_str(), &isOpen);

    m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

    DrawContent();

    ImGui::End();
}

bool BaseWindow::IsFocused() const
{
    return m_isFocused;
}