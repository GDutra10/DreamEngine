#include "GetNameModal.h"

#include "../../Vendors/imgui/imgui.h"

using namespace DreamEngine::Editor::UI::Modals;

GetNameModal::GetNameModal(std::string title) : BaseModal(std::move(title)){}

void GetNameModal::Open(std::string title, const std::function<Result(std::string name)>& callbackOk)
{
    m_name[0] = '\0';
    m_errorMessage = "";
    m_title = std::move(title);
    m_callbackOk = callbackOk;

    BaseModal::Open();
}

void GetNameModal::DrawContent() 
{

    ImGui::Text("Name");
    ImGui::SameLine();

    ImGui::InputText("##get.name.popup.name", m_name, sizeof(m_name));

    if (!m_errorMessage.empty())
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), m_errorMessage.c_str());

    if (ImGui::Button("  OK  "))
    {
        Result result = m_callbackOk(m_name);

        if (result.isOk)
            Close();
        else
            m_errorMessage = result.errorMessage;
    }
}
