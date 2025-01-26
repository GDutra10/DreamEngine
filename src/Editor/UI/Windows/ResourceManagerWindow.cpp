#include "ResourceManagerWindow.h"

#include "../../../Core/Resources/GlobalResourceManager.h"
#include "../../Helpers/ImGuiHelper.h"

using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Render;

#ifndef COMBO_RESOURCE_SHADER
#define COMBO_RESOURCE_SHADER "Shader##resource.manager.window.shader"
#endif

#ifndef COMBO_RESOURCE_MATERIAL
#define COMBO_RESOURCE_MATERIAL "Material##resource.manager.window.material"
#endif

#ifndef COMBO_RESOURCE_MESH
#define COMBO_RESOURCE_MESH "Mesh##resource.manager.window.mesh"
#endif

#ifndef COMBO_RESOURCE_TEXTURE
#define COMBO_RESOURCE_TEXTURE "Texture##resource.manager.window.texture"
#endif

#ifndef COMBO_RESOURCE_SCRIPT
#define COMBO_RESOURCE_SCRIPT "Script##resource.manager.window.script"
#endif

vector<string> ResourceManagerWindow::m_sResourcesNames = {
    COMBO_RESOURCE_MATERIAL,
    COMBO_RESOURCE_MESH,
    COMBO_RESOURCE_SHADER,
    COMBO_RESOURCE_TEXTURE,
    COMBO_RESOURCE_SCRIPT
};

ResourceManagerWindow::ResourceManagerWindow(const string& title) : BaseWindow(std::move(title))
{
    m_resourceSelected = COMBO_RESOURCE_MATERIAL;
}

void ResourceManagerWindow::DrawContent()
{
    ImGui::Text("Resource");
    ImGui::SameLine();

    if (ImGui::BeginCombo("##resource.manager.window.resource.combo", m_resourceSelected.c_str()))
    {
        if (ImGui::Selectable(COMBO_RESOURCE_MATERIAL, m_resourceSelected == COMBO_RESOURCE_MATERIAL))
            m_resourceSelected = COMBO_RESOURCE_MATERIAL;

        if (ImGui::Selectable(COMBO_RESOURCE_MESH, m_resourceSelected == COMBO_RESOURCE_MESH))
            m_resourceSelected = COMBO_RESOURCE_MESH;

        if (ImGui::Selectable(COMBO_RESOURCE_SHADER, m_resourceSelected == COMBO_RESOURCE_SHADER))
            m_resourceSelected = COMBO_RESOURCE_SHADER;

        if (ImGui::Selectable(COMBO_RESOURCE_TEXTURE, m_resourceSelected == COMBO_RESOURCE_TEXTURE))
            m_resourceSelected = COMBO_RESOURCE_TEXTURE;

        if (ImGui::Selectable(COMBO_RESOURCE_SCRIPT, m_resourceSelected == COMBO_RESOURCE_SCRIPT))
            m_resourceSelected = COMBO_RESOURCE_SCRIPT;

        ImGui::EndCombo();
    }

    DrawMaterials();
    DrawMeshes();
    DrawShaders();
    DrawTextures();
    DrawScripts();
}

void ResourceManagerWindow::DrawMaterials()
{
    if (m_resourceSelected != COMBO_RESOURCE_MATERIAL)
        return;

    if (ImGuiHelper::BeginTable("resource.manager.materials", 2))
    {
        unsigned int count = 0;

        for (const pair<const string, Resource*> keyValue: GlobalResourceManager::Instance().GetMaterials())
        {
            ImGuiHelper::NextRow("resource.manager.material." + count, keyValue.second->name.c_str(), keyValue.second->resourceId, false);
            count++;
        }

        ImGui::EndTable();
    }
}

void ResourceManagerWindow::DrawMeshes()
{
    if (m_resourceSelected != COMBO_RESOURCE_MESH)
        return;

    if (ImGuiHelper::BeginTable("resource.manager.meshes", 2))
    {
        unsigned int count = 0;

        for (const pair<const string, Resource*> keyValue : GlobalResourceManager::Instance().GetMeshes())
        {
            ImGuiHelper::NextRow("resource.manager.mesh." + count, keyValue.second->name.c_str(), keyValue.second->resourceId, false);
            count++;
        }

        ImGui::EndTable();
    }
}

void ResourceManagerWindow::DrawShaders()
{
    if (m_resourceSelected != COMBO_RESOURCE_SHADER)
        return;

    if (ImGuiHelper::BeginTable("resource.manager.shaders", 2))
    {
        unsigned int count = 0;

        for (const pair<const string, Resource*> keyValue : GlobalResourceManager::Instance().GetShaders())
        {
            ImGuiHelper::NextRow("resource.manager.shader." + count, keyValue.second->name.c_str(), keyValue.second->resourceId, false);
            count++;
        }

        ImGui::EndTable();
    }
}

void ResourceManagerWindow::DrawTextures()
{
    if (m_resourceSelected != COMBO_RESOURCE_TEXTURE)
        return;

    if (ImGuiHelper::BeginTable("resource.manager.textures", 2))
    {
        unsigned int count = 0;

        for (const pair<const string, Resource*> keyValue : GlobalResourceManager::Instance().GetTextures())
        {
            ImGuiHelper::NextRow("resource.manager.texture." + count, keyValue.second->name.c_str(), keyValue.second->resourceId, false);
            count++;
        }

        ImGui::EndTable();
    }
}

void ResourceManagerWindow::DrawScripts()
{
    if (m_resourceSelected != COMBO_RESOURCE_SCRIPT)
        return;

    if (ImGuiHelper::BeginTable("resource.manager.script", 2))
    {
        unsigned int count = 0;

        for (const pair<const string, Resource*> keyValue : GlobalResourceManager::Instance().GetScripts())
        {
            ImGuiHelper::NextRow("resource.manager.script." + count, keyValue.second->name.c_str(), keyValue.second->resourceId, false);
            count++;
        }

        ImGui::EndTable();
    }
}
