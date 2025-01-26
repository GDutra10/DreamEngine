#include "ImportResourceModal.h"

#include "../../../Core/Loggers/LoggerSingleton.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Modals;
using namespace DreamEngine::Editor::Singletons;

ImportResourceModal::ImportResourceModal(std::string title) : BaseModal(std::move(title)),
    m_getNamePopup("File Name"),
    m_fileDialog("Select File")
{
    minWidth = 700.0f;
    maxWidth = 700.0f;
    minHeight = 400.0f;
    maxHeight = 400.0f;
}

void ImportResourceModal::DrawContent()
{
    static constexpr ImVec2 buttonSize = ImVec2(100, 50);

    // popup
    m_getNamePopup.Draw();
    m_fileDialog.Draw();

    ImGui::Text("Add or Import Resource to the engine");
    ImGui::Text("You are add/import the resource in: ");
    ImGui::SameLine();
    string selectedPathByProject = "..\\" + EditorSingleton::Instance().GetSelectedPathByProject();
    ImGui::Text(selectedPathByProject.c_str());

    ImGui::Separator();

    ImGui::Dummy(ImVec2(4.0f, 0.0f));
    ImGui::SameLine();

    ImGui::BeginGroup();

    // Add Material
    ImGui::Text("Material");
    if (ImGui::Button("Add##import.resource.add.material", buttonSize))
        m_getNamePopup.Open("Material File Name", [this](const std::string& name) {
            Result result = EditorSingleton::Instance().GetResourceController().CreateMaterialFile(name);

            if (result.isOk)
                Close();

            return result;
        });

    // Add Shader
    ImGui::Text("Shader");
    if (ImGui::Button("Add##import.resource.add.shader", buttonSize))
        LoggerSingleton::Instance().LogWarning("Add shader not implemented yet");
    ImGui::SameLine();
    if (ImGui::Button("Import##import.resource.import.shader", buttonSize))
        LoggerSingleton::Instance().LogWarning("Import shader not implemented yet");

    // Import Texture
    ImGui::Text("Texture");
    if (ImGui::Button("Import##import.resource.import.texture", buttonSize))
        LoggerSingleton::Instance().LogWarning("Import texture not implemented yet");

    // Import Model
    ImGui::Text("Model");
    if (ImGui::Button("Import##import.resource.import.model", buttonSize))
        m_fileDialog.Open([this](const std::string& filePath) { CreateMeshesFromFile(filePath); });

    ImGui::EndGroup();
}

void ImportResourceModal::CreateMeshesFromFile(const std::string& filePath)
{
    Result result = EditorSingleton::Instance().GetResourceController().CreateMeshFileFromModelFile(filePath);

    if (result.isOk)
        Close();
}
