#include "ProjectWindow.h"

#include "../../Helpers/ImGuiHelper.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"

#include <string>
#include <filesystem>

using namespace std::filesystem;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Singletons;

ProjectWindow::ProjectWindow(const std::string& title) : BaseWindow(title),
m_importResourcePopup(new ImportResourceModal("Import Resource"))
{}

void ProjectWindow::DrawContent()
{
    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;

    string projectName = EditorSingleton::Instance().GetProjectConfiguration().projectName;
    string projectPath = EditorSingleton::Instance().GetProjectConfiguration().projectPath;
    string selectedPath = EditorSingleton::Instance().GetSelectedPath().string();

    if (ImGui::Button("   Add   "))
        m_importResourcePopup->Open();

    m_importResourcePopup->Draw();

    ImGuiHelper::DrawDirectoryTree(projectPath, selectedPath, [this](const path& path) { EditorSingleton::Instance().SetSelectedPath(path); });
}
