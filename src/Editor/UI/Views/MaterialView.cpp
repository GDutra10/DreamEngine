#include "MaterialView.h"

#include "../../../Core/Resources/GlobalResourceManager.h"
#include "../../Helpers/ImGuiHelper.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Views;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::Singletons;

void MaterialView::Draw(Material& material, const function<void(Material*)>& callbackChange)
{
    if (ImGuiHelper::BeginTable("material.view.table", 2))
    {
        ImGuiHelper::PrepareRow("Material");

        if (ImGui::BeginCombo("##material.view.combo", material.name.c_str()))
        {
            for (auto& [_, mat] : GlobalResourceManager::Instance().GetMaterials())
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