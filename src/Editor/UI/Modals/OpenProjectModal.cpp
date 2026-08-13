#include "OpenProjectModal.h"

#include <filesystem>

#include "../../EditorDefine.h"
#include "../../Serializers/ProjectConfigurationSerializer.h"
#include "../../../Core/Loggers/Logger.h"
#include "../../Vendors/imgui/imgui.h"
#include "Loggers/LoggerSingleton.h"

using namespace DreamEngine::Editor::UI::Modals;
using namespace DreamEngine::Core::Loggers;

OpenProjectModal::OpenProjectModal(std::string title, EditorContext& editorContext, ProjectController& projectController) 
    : BaseModal(std::move(title), editorContext)
    , m_projectController(projectController)
    , m_fileDialog("Select File", editorContext)
{
    m_newProjectName[0] = '\0';
    minWidth = 700.0f;
    maxWidth = 700.0f;
    minHeight = 400.0f;
    maxHeight = 400.0f;

    m_fileDialog.maxHeight = 600.0f;
    m_fileDialog.maxWidth = 200.0f;
}

void OpenProjectModal::DrawContent()
{
    // popup
    m_fileDialog.Draw();

    ImGui::Text("Open Project");
    ImGui::Separator();

    ImGui::Text("Select a project file to open");
    ImGui::SameLine();
    if (ImGui::Button("Open"))
    {
        FileDialogConfig config = {false, false, false, true, EDITOR_PROJECT_FILE_EXTENSION};

        m_fileDialog.Open( [this](const std::string& filePath) -> bool
        {
            LoggerSingleton::Instance().LogDebug("Open project selected!");

            std::filesystem::path path(filePath);

            LoggerSingleton::Instance().LogDebug(path.extension().string());

            if (path.extension().string() != EDITOR_PROJECT_FILE_EXTENSION)
            {
                LoggerSingleton::Instance().LogError("The file is not a project file");
                return false;
            }

            std::ifstream stream = std::ifstream(path.string());

            if (!stream.is_open())
            {
                LoggerSingleton::Instance().LogError("Can't open the editor project file!");
                return false;
            }

            ProjectConfiguration& projectConfiguration = Serializers::ProjectConfigurationSerializer::Deserialize(stream);
            m_projectController.LoadProjectConfiguration(projectConfiguration);

            this->Close();

            return true;
        }, config);
    }

    ImGui::Separator();
    ImGui::Text("Create Project");

    ImGui::Text("Project Name: ");
    ImGui::SameLine();
    ImGui::InputText("##open.project.modal.new.project.name", m_newProjectName, 100);

    ImGui::Text("Folder: ");
    ImGui::SameLine();
    ImGui::Text(m_selectedPath.string().c_str());
    ImGui::SameLine();
    if (ImGui::Button("Select"))
    {
        m_fileDialog.Open([this](const std::string& filePath) -> bool
        {
            m_selectedPath = std::filesystem::path(filePath);
            return true;
        }, {false, false, true});
    }

    if (ImGui::Button("Create"))
    {
        if (m_selectedPath.string() == "" || m_newProjectName[0] == '\0')
            Core::Loggers::LoggerSingleton::Instance().LogWarning("selected path or project name is empty");
        else
        {
            Models::ResultWithData<Models::ProjectConfiguration> result = m_projectController.CreateProjectConfiguration(m_selectedPath.string(), std::string(m_newProjectName));

            if (result.isOk)
            {
                m_projectController.LoadProjectConfiguration(result.data);
                this->Close();
            }
        }
    }
}
