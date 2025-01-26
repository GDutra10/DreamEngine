#include "BaseModal.h"
#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Modals;

BaseModal::BaseModal(std::string title) : m_title(std::move(title)){}

void BaseModal::Open()
{
    m_isOpen = true;
    ImGui::OpenPopup(m_title.c_str());
}

void BaseModal::Close()
{
    m_isOpen = false;
}

void BaseModal::Draw()
{
    // set min and max size of the popup
    ImGui::SetNextWindowSizeConstraints(ImVec2(minWidth, minHeight), ImVec2(maxWidth, maxHeight));

    if (ImGui::BeginPopupModal(m_title.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::BeginGroup();
        DrawContent();
        ImGui::EndGroup();

        ImGui::EndPopup();
    }
}
