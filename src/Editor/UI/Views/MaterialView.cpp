#include "MaterialView.h"

#include "../../../Core/Resources/ResourceManager.h"
#include "../../Helpers/ImGuiHelper.h"
#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Views;
using namespace DreamEngine::Editor::Helpers;

void MaterialView::Draw(Material& material, const function<void(Material*)>& callbackChange)
{
    if (ImGuiHelper::BeginTable("material.view.table", 2))
    {
        ImGuiHelper::PrepareRow("Material");

        if (ImGui::BeginCombo("##material.view.combo", material.name.c_str()))
        {
            for (auto& [_, mat] : ResourceManager::Instance().GetMaterials())
            {
                const bool isSelected = material.name == mat->name;

                if (ImGui::Selectable(mat->name.c_str(), isSelected))
                {
                    callbackChange(mat);
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
        
        ImGui::EndTable();
    }
}