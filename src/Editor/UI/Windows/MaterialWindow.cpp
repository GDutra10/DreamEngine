#include "MaterialWindow.h"

#include "../Views/ColorView.h"
#include "../../Helpers/ImGuiHelper.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"
#include "../../../Core/Resources/ResourceManager.h"

using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::UI::Views;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::Singletons;
using namespace DreamEngine::Core::Resources;

MaterialWindow::MaterialWindow(const std::string& title) : BaseWindow(title)
    , m_pMaterial(nullptr)
    , m_originalMaterial(nullptr)
{}

void MaterialWindow::DrawContent()
{
    if (m_sSelectedMaterialPath != EditorSingleton::Instance().GetSelectedMaterialPath())
    {
        m_sSelectedMaterialPath = EditorSingleton::Instance().GetSelectedMaterialPath();

        if (!m_sSelectedMaterialPath.empty())
        {
            LoadMaterialFromFile();

            if (m_originalMaterial != nullptr)
            {
                m_pMaterial = ResourceManager::Instance().GetMaterial(m_originalMaterial->resourceId);
            }
        }
    }

    if (m_pMaterial == nullptr)
        return;

    if (ImGuiHelper::BeginTable("table.selected.entity", 2))
    {
        ImGuiHelper::NextRow("##selected.entity.name", "Name", m_pMaterial->name, false);
        ImGui::EndTable();
    }

    ColorView::Draw("Ambient", m_pMaterial->ambient);
    ColorView::Draw("Diffuse", m_pMaterial->diffuse);
    ColorView::Draw("Specular", m_pMaterial->specular);

    if (ImGui::BeginTable("table.material", 2, ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Column1", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableSetupColumn("Column2", ImGuiTableColumnFlags_WidthStretch);

        // shininess
        ImGuiHelper::PrepareRow("Shininess");
        ImGui::InputFloat("##material.shininess", &m_pMaterial->shininess, 0, 0, "%.2f");

        // end table
        ImGui::EndTable();
    }

    if (ImGui::Button("    Save    "))
    {
        EditorSingleton::Instance()
            .GetResourceController()
            .SaveMaterialFile(m_pMaterial, m_sSelectedMaterialPath.string());

       LoadMaterialFromFile();
    }

    ImGui::SameLine();

    if (ImGui::Button("   Cancel   "))
    {
        m_pMaterial->ambient = m_originalMaterial->ambient;
        m_pMaterial->diffuse = m_originalMaterial->diffuse;
        m_pMaterial->specular = m_originalMaterial->specular;
        m_pMaterial->shininess = m_originalMaterial->shininess;
    }
}

void MaterialWindow::LoadMaterialFromFile()
{
    m_originalMaterial = EditorSingleton::Instance().GetResourceController().LoadMaterial(m_sSelectedMaterialPath.string());
}

