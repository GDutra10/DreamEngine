#include "ProjectWindow.h"

#include "../../Helpers/ImGuiHelper.h"
#include "../../Vendors/imgui/imgui.h"

#include <string>
#include <filesystem>

using namespace std::filesystem;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::UI::Windows;

ProjectWindow::ProjectWindow(const std::string& title, EditorContext& editorContext, ResourceController& resourceController) 
    : BaseWindow(title, editorContext)
    , m_importResourcePopup(new ImportResourceModal("Import Resource", editorContext, resourceController))    
    {
        m_fileDialogConfig = {true, true, true};
    }

void ProjectWindow::DrawContent()
{
    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;

    string projectName = m_editorContext.GetProjectConfiguration().projectName;
    string projectPath = m_editorContext.GetProjectConfiguration().projectPath;
    string selectedPath = m_editorContext.GetSelectedPath().string();

    if (ImGui::Button("   Add   "))
        m_importResourcePopup->Open();

    m_importResourcePopup->Draw();

    ImGuiHelper::DrawDirectoryTree(
        projectPath, 
        selectedPath, 
        [this](const path& path) { m_editorContext.SetSelectedPath(path); }, 
        m_fileDialogConfig
    );
}
