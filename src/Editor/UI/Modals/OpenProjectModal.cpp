#include "OpenProjectModal.h"

#include <filesystem>

#include "../../EditorDefine.h"
#include "../../Serializers/ProjectConfigurationSerializer.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"
#include "Loggers/LoggerSingleton.h"

using namespace DreamEngine::Editor::UI::Modals;

OpenProjectModal::OpenProjectModal(std::string title) : BaseModal(std::move(title))
    , m_fileDialog("Select File")
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
            Loggers::LoggerSingleton::Instance().LogDebug("Open project selected!");

            std::filesystem::path path(filePath);

            Loggers::LoggerSingleton::Instance().LogDebug(path.extension().string());

            if (path.extension().string() != EDITOR_PROJECT_FILE_EXTENSION)
            {
                Loggers::LoggerSingleton::Instance().LogError("The file is not a project file");
                return false;
            }

            Controllers::ProjectController projectController = Singletons::EditorSingleton::Instance().GetProjectController();
            std::ifstream stream = std::ifstream(path.string());

            if (!stream.is_open())
            {
                Loggers::LoggerSingleton::Instance().LogError("Can't open the editor project file!");

                return false;
            }

            ProjectConfiguration& projectConfiguration = Serializers::ProjectConfigurationSerializer::Deserialize(stream);
            projectController.LoadProjectConfiguration(projectConfiguration);

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
            Controllers::ProjectController projectController = Singletons::EditorSingleton::Instance().GetProjectController();
            Models::ResultWithData<Models::ProjectConfiguration> result = projectController.CreateProjectConfiguration(m_selectedPath.string(), std::string(m_newProjectName));

            if (result.isOk)
            {
                projectController.LoadProjectConfiguration(result.data);
                this->Close();
            }
        }
    }
}
