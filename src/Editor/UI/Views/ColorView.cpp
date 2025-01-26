#include "ColorView.h"
#include "../../Vendors/imgui/imgui.h"
#include "../../Helpers/ImGuiHelper.h"

using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::UI::Views;

// Draw a color view to show what the color is picked
void ColorView::Draw(const std::string& text, Color& color)
{
    const char* id = text.c_str();

    if (ImGuiHelper::BeginTable(id, 2))
    {
        ImGuiHelper::PrepareRow(id);

        ImGui::PushID(text.c_str());
        if (glm::vec4 colorVec4 = color.ToVec4(); ImGui::ColorEdit4("", &colorVec4.x))
        {
            color.red = colorVec4.r;
            color.green = colorVec4.g;
            color.blue = colorVec4.b;
            color.alpha = colorVec4.a;
        }
        ImGui::PopID();

        ImGui::EndTable();
    }
}

// Draw a vec3 color view to show what the vec3 color is picked
void ColorView::Draw(const std::string& text, glm::vec3& color)
{
    const char* id = text.c_str();

    if (ImGuiHelper::BeginTable(id, 2))
    {
        ImGuiHelper::PrepareRow(id);

        ImGui::PushID(text.c_str());
        ImGui::ColorEdit3("", &color.x);
        ImGui::PopID();

        ImGui::EndTable();
    }
}
