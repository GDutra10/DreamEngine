#include "FileExplorerWindow.h"

#include "EngineDefine.h"
#include "../../EditorDefine.h"
#include "../../Core/Loggers/LoggerSingleton.h"
#include "../../Helpers/FileHelper.h"
#include "../../Helpers/TextureHelper.h"
#include "../../Serializers/ModelSerializer.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"
#include "ECS/Components/MaterialComponent.h"
#include "ECS/Components/UiComponent.h"
#include "Resources/ResourceManager.h"

using namespace std;
using namespace std::filesystem;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Singletons;
using namespace DreamEngine::Editor::Serializers;


FileExplorerWindow::FileExplorerWindow(const std::string& title) : BaseWindow(title) {}

void FileExplorerWindow::DrawContent()
{
    if (!m_wasInitialized)
    {
        const Texture* folderTexture = TextureHelper::LoadTextureFromFile("Assets/Icons/folder.png");
        m_mFileIcons["folder"] = {folderTexture->id, {64, 64}};

        const Texture* fileTexture = TextureHelper::LoadTextureFromFile("Assets/Icons/file.png");
        m_mFileIcons["file"] = {fileTexture->id, {64, 64}};

        m_wasInitialized = true;
    }

    if (EditorSingleton::Instance().GetSelectedPath().empty())
        return;

    DrawBreadcrumbNavigation(EditorSingleton::Instance().GetSelectedPath());
    FileSystemBrowser();

    // Print all collected files
    /*std::vector<path> files = FileHelper::GetAllFilesInCurrentDirectory(EditorSingleton::Instance().GetSelectedPath());

    for (const auto& file : files)
    {
        const bool isSelected = (file == m_sSelectedFile);

        if (ImGui::Selectable(("   " + file.filename().string()).c_str(), isSelected, ImGuiSelectableFlags_None, ImVec2(150, 180)))
        {
            m_sSelectedFile = file;

            LoggerSingleton::Instance().LogTrace("FileExplorerWindow::DrawContent -> Selected file: " + m_sSelectedFile.string());
        }

        
    }*/

    // Right Click File Popup
    if (ImGui::BeginPopup("##file.explorer.file.right.click.popup"))
    {
        if (ImGui::MenuItem("Delete"))
        {
            if (m_sRightClickFile.extension() == EDITOR_DEFAULT_MATERIAL_FILE_EXTENSION)
                EditorSingleton::Instance().GetResourceController().DeleteMaterialFile(m_sRightClickFile.string());
            else
                LoggerSingleton::Instance().LogWarning("FileExplorerWindow::DrawContent -> File type not supported");

            
            m_sRightClickFile.clear();

            ImGui::CloseCurrentPopup();
        }

        if (m_sRightClickFile.extension() == EDITOR_DEFAULT_MODEL_FILE_EXTENSION)
        {
            if (ImGui::MenuItem("Add in Current Scene"))
            {
                LoggerSingleton::Instance().LogTrace("FileExplorerWindow::DrawContent -> Add in Current Scene: " + m_sRightClickFile.string());

                std::ifstream stream(m_sRightClickFile);
                Model& model = ModelSerializer::Deserialize(stream);
                EntityManager* entityManager = EditorSingleton::Instance().GetEntityManager();
                Entity* entity = entityManager->AddEntity("entity");
                entity->GetName() = m_sRightClickFile.filename().string();

                for (const Mesh* mesh : model.meshes)
                {
                    Entity* meshEntity = entityManager->AddEntity("entity", entity);
                    meshEntity->GetName() = mesh->name;

                    MeshComponent& meshComponent = meshEntity->GetComponent<MeshComponent>();
                    meshComponent.has = true;
                    meshComponent.mesh = ResourceManager::Instance().GetMesh(mesh->resourceId);

                    MaterialComponent& materialComponent = meshEntity->GetComponent<MaterialComponent>();
                    materialComponent.has = true;
                    materialComponent.material = ResourceManager::Instance().GetMaterial(DEFAULT_MATERIAL_NAME);    

                    // delete this mesh because it will be used from the resource manager
                    //delete mesh;
                }

                m_sRightClickFile.clear();
                ImGui::CloseCurrentPopup();
            }
        }

        if (m_sRightClickFile.extension() == EDITOR_DEFAULT_UI_FILE_EXTENSION)
        {
            if (ImGui::MenuItem("Reload"))
            {
                const UiContent* content = ResourceController::LoadUiContent(m_sRightClickFile.string());
                const std::string resourceId = FileHelper::GetRelativePathByProject(m_sRightClickFile).string();
                const map<string, UiContent*>& contents = ResourceManager::Instance().GetUiContents();

                if (const auto it = contents.find(resourceId); it != contents.end() && it->second != nullptr)
                {
                    it->second->text = content->text;

                    for (Entity* entity : EditorSingleton::Instance().GetEntityManager()->GetEntities())
                    {
                        if (entity->HasComponent<UiComponent>())
                        {
                            UiComponent& uiComponent = entity->GetComponent<UiComponent>();

                            if (uiComponent.content->resourceId != resourceId)
                                continue;
                            
                            if (uiComponent.instance == nullptr)
                                continue;

                            UiManager::Destroy(uiComponent.instance);
                            uiComponent.instance = nullptr;
                        }
                    }
                }

                m_sRightClickFile.clear();
                ImGui::CloseCurrentPopup();
                delete content;
            }
        }

        ImGui::EndPopup();
    }

    // Check if the selected file is a scene file
    if (m_sSelectedFile.extension() == EDITOR_DEFAULT_SCENE_FILE_EXTENSION && m_sSelectedFile != EditorSingleton::Instance().GetSelectedScenePath())
        EditorSingleton::Instance().SetSelectedScenePath(m_sSelectedFile);

    // Check if the selected file is a material file
    if (m_sSelectedFile.extension()== EDITOR_DEFAULT_MATERIAL_FILE_EXTENSION && m_sSelectedFile != EditorSingleton::Instance().GetSelectedMaterialPath())
        EditorSingleton::Instance().SetSelectedMaterialPath(m_sSelectedFile);
}

void FileExplorerWindow::DrawBreadcrumbNavigation(const path& currentPath)
{
    path current = "/";
    const std::string projectName = EditorSingleton::Instance().GetProjectConfiguration().projectName;
    bool foundProjectName = false;

    for (const auto& part : currentPath)
    {
        current /= part;

        if (!foundProjectName)
        {
            if (part.string() == projectName)
                foundProjectName = true;

            continue;
        }

        if (ImGui::Button(part.string().c_str()))
            EditorSingleton::Instance().SetSelectedPath(current);

        ImGui::SameLine();
        ImGui::Text("/");
        ImGui::SameLine();
    }
    ImGui::NewLine();
}

void FileExplorerWindow::FileSystemBrowser()
{
    // File and Folder Display
    static float thumbnailSize = 128;
    static float padding = 8.0f;
    static int columns = 5;

    //ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 32.0f, 128.0f);
    //ImGui::SliderFloat("Padding", &padding, 2.0f, 16.0f);

    columns = (int)(ImGui::GetContentRegionAvail().x / (thumbnailSize + padding));
    if (columns < 1)
        columns = 1;

    ImGui::Columns(columns, nullptr, false);

    // Right panel: File list
    for (const auto& entry : directory_iterator(EditorSingleton::Instance().GetSelectedPath()))
    {
        bool isFolder = entry.is_directory();
        std::string name = entry.path().filename().string();

        // Get the appropriate icon
        FileIcon& icon = m_mFileIcons[isFolder ? "folder" : "file"];

        // Draw file/folder entry
        DrawFileEntry(name, icon, m_sSelectedFile == entry.path().string(), [&]() {
            if (isFolder) {
                EditorSingleton::Instance().SetSelectedPath(entry.path().string());
            } else {
                m_sSelectedFile = entry.path().string();
            }
        });

        ImGui::NextColumn();

    }
}

void FileExplorerWindow::DrawFileEntry(const std::string& name, const FileIcon& icon, bool isSelected, const function<void()>& onClick)
{
    ImGui::BeginGroup();

   // Draw the icon
    ImGui::Image((void*)(intptr_t)icon.textureID, icon.size);

    // Check for selection
    if (ImGui::IsItemClicked())
    {
        onClick();
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        m_sRightClickFile = m_sSelectedFile;
        ImGui::OpenPopup("##file.explorer.file.right.click.popup");
    }

    // Draw the file/folder name (centered)
    ImGui::TextWrapped("%s", name.c_str());

    ImGui::EndGroup();
}
