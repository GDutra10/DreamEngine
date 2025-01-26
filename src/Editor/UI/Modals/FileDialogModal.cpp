#include "FileDialogModal.h"
#include <filesystem>
#include <vector>
#include <string>
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Modals;
using namespace DreamEngine::Editor::Singletons;

FileDialogModal::FileDialogModal(const std::string& title) : BaseModal(title){}

void FileDialogModal::Open(const std::function<void(std::string)>& callbackOk)
{
    m_callbackOk = callbackOk;
    selectedFile = "";
    currentPath = EditorSingleton::Instance().GetSelectedPath().string();

    BaseModal::Open();
}

void FileDialogModal::DrawContent()
{
    std::vector<std::string> files;

    for (const auto& entry : std::filesystem::directory_iterator(currentPath))
    {
        files.push_back(entry.path().filename().string());
    }

    ImGui::Text("Current Path: %s", currentPath.c_str());

    if (ImGui::BeginListBox("##file.dialog.modal.file.list"))
    {
        for (const auto& file : files)
        {
            if (ImGui::Selectable(file.c_str()))
                selectedFile = file;
        }
        ImGui::EndListBox();
    }

    ImGui::Text(selectedFile.c_str());

    if (ImGui::Button("OK"))
    {
        m_callbackOk(currentPath.empty() || selectedFile.empty() ? "" : currentPath + "/" + selectedFile);
        Close();
    }
}