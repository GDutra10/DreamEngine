#include "ProjectWindow.h"

#include "../../Vendors/imgui/imgui.h"
#include "../../Singletons/EditorSingleton.h"

#include <string>
#include <filesystem>

using namespace std::filesystem;
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

    DrawDirectoryTree(projectPath, selectedPath);
}

void ProjectWindow::DrawDirectoryTree(const path& directoryPath, std::string& selectedPath)
{
    for (const auto& entry : directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            if (ImGui::TreeNode(entry.path().filename().string().c_str()))
            {
                if (ImGui::IsItemClicked())
                    EditorSingleton::Instance().SetSelectedPath(entry.path());

                DrawDirectoryTree(entry.path(), selectedPath);

                ImGui::TreePop();
            }
        }
        else if (entry.is_regular_file())
        {
            if (ImGui::Selectable(entry.path().filename().string().c_str(), selectedPath == entry.path()))
            {
                selectedPath = entry.path().string();
            }
        }
    }
}

