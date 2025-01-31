#include "ImGuiHelper.h"

#include <filesystem>

#include "../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::Helpers;

ImVec4 ImGuiHelper::m_sImVec4Red = {188 / 255.0f, 31 / 255.0f, 43 / 255.0f, 1.0f};
ImVec4 ImGuiHelper::m_sImVec4Green = {55 / 255.0f, 172 / 255.0f, 54 / 255.0f, 1.0f};
ImVec4 ImGuiHelper::m_sImVec4Blue = {30 / 255.0f, 64 / 255.0f, 199 / 255.0f, 1.0f};

void ImGuiHelper::PrepareRow(const char* label)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    // Center-align the text
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
    ImGui::Text(label);
    ImGui::TableNextColumn();

    // Set the next item width to fill the column
    ImGui::SetNextItemWidth(-FLT_MIN);
}

void ImGuiHelper::NextRow(const char* id, const char* label, std::string& value, bool editable)
{
    PrepareRow(label);

    // Create a buffer to hold the name
    char nameBuffer[256];
    strncpy(nameBuffer, value.c_str(), sizeof(nameBuffer));
    nameBuffer[sizeof(nameBuffer) - 1] = '\0';  // Ensure null-termination

    if (ImGui::InputText(id, nameBuffer, sizeof(nameBuffer), editable ? ImGuiInputTextFlags_AlwaysOverwrite : ImGuiInputTextFlags_ReadOnly))
    {
        // Update the entity name if it was modified
        value = std::string(nameBuffer);
    }
}

bool ImGuiHelper::BeginTable(const char* id, const unsigned int columns)
{
    const bool beginTable = ImGui::BeginTable(id, columns, ImGuiTableFlags_Resizable);

    if (beginTable)
    {
        // Set the table width to the window width
        for (int i = 0; i < columns; i++)
        {
            ImGui::PushID(i);

            if (i == 0)
                ImGui::TableSetupColumn(std::to_string(i).c_str(), ImGuiTableColumnFlags_WidthFixed, 90.0f);
            else
                ImGui::TableSetupColumn(std::to_string(i).c_str(), ImGuiTableColumnFlags_WidthStretch);

            ImGui::PopID();
        }
    }

    return beginTable;
}

void ImGuiHelper::DrawDirectoryTree(const std::filesystem::path& directoryPath, std::string& selectedPath, const std::function<void(std::filesystem::path)>& selectedPathCallback,
                                    FileDialogConfig config)
{
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            if (ImGui::TreeNode(entry.path().filename().string().c_str()))
            {
                if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    selectedPathCallback(entry.path());

                DrawDirectoryTree(entry.path(), selectedPath, selectedPathCallback, config);

                ImGui::TreePop();
            }
        }
        else if (entry.is_regular_file() && !config.shouldRenderOnlyFolder)
        {
            if (config.shouldRenderByExtension && entry.path().extension() != config.extension)
                continue;

            if (ImGui::Selectable(entry.path().filename().string().c_str(), selectedPath == entry.path()))
            {
                selectedPath = entry.path().string();
                selectedPathCallback(entry.path());
            }
        }
    }
}
