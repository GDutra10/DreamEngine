#include "LogView.h"

#include "../../Vendors/imgui/imgui.h"
#include "../../../Core/Helpers/DateHelper.h"

using namespace DreamEngine::Editor::UI::Views;
using namespace DreamEngine::Core::Helpers;

void LogView::Draw(Log& log)
{
    switch (log.logLevel)
    {
        case LogLevel::Trace:
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), log.message.c_str());
            break;
        case LogLevel::Debug:
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), log.message.c_str());
            break;
        case LogLevel::Info:
            ImGui::TextColored(ImVec4(0.2f, 0.4f, 1.0f, 1.0f), log.message.c_str());
            break;
        case LogLevel::Warning:
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), log.message.c_str());
            break;
        case LogLevel::Error:
            ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), log.message.c_str());
            break;
        default:
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), log.message.c_str());
            break;
    }
}
