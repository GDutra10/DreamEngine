#include "FileDialogModal.h"
#include <filesystem>
#include <vector>
#include <string>

#include "../../Helpers/ImGuiHelper.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Modals;
using namespace DreamEngine::Editor::Singletons;

FileDialogModal::FileDialogModal(const std::string& title) : BaseModal(title) {}

void FileDialogModal::Open(const std::function<bool(std::string)>& callbackOk, const FileDialogConfig& config)
{
    selectedFile = "";
    m_callbackOk = callbackOk;
    m_config = config;
    m_config.shouldRenderOnlyTheCurrentPath = config.shouldRenderOnlyTheCurrentPath;
    m_config.shouldRenderOnlyFolder = config.shouldRenderOnlyFolder;
    currentPath = config.shouldUseCurrentPath
        ? EditorSingleton::Instance().GetSelectedPath().string()
        : current_path().root_path().string();

    BaseModal::Open();
}

void FileDialogModal::DrawContent()
{
    if (m_config.shouldRenderOnlyTheCurrentPath)
    {
        std::vector<std::string> files;

        for (const auto& entry : directory_iterator(currentPath))
        {
            files.push_back(entry.path().filename().string());
        }

        ImGui::Text("Current Path: %s", currentPath.c_str());

        if (ImGui::BeginListBox("##file.dialog.modal.file.list"))
        {
            for (const auto& file : files)
            {
                if (m_config.shouldRenderOnlyFolder && !is_directory(file))
                    continue;

                if (!is_directory(file) && m_config.shouldRenderByExtension && file.find(m_config.extension) == std::string::npos)
                    continue;

                if (ImGui::Selectable(file.c_str()))
                    selectedFile = file;
            }
            ImGui::EndListBox();
        }

        ImGui::Text(selectedFile.c_str());

        if (ImGui::Button("OK"))
        {
            if (m_callbackOk(currentPath.empty() || selectedFile.empty() ? "" : currentPath + "/" + selectedFile))
                Close();
        }
    }
    else
    {
        ImGui::BeginGroup();
        Helpers::ImGuiHelper::DrawDirectoryTree(currentPath, selectedFile, [this](const path& path)
        {
            if (m_callbackOk(path.string()))
            {
                Close();
            }
        }, m_config);
        ImGui::EndGroup();
    }
}