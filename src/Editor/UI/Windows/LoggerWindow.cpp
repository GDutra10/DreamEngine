#include "LoggerWindow.h"
#include "../../Vendors/imgui/imgui.h"
#include "../../Loggers/EditorLogger.h"
#include "../../Singletons/EditorSingleton.h"
#include "../Views/LogView.h"    

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor::Loggers;
using namespace DreamEngine::Editor::UI::Views; 
using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Singletons;

LoggerWindow::LoggerWindow(const std::string& title) : BaseWindow(title) {}

void LoggerWindow::DrawContent()
{
    vector<Log>& logsFromEditorLogger = EditorSingleton::Instance().GetEditorLogger()->GetLogs();
    bool mustScrollDown = false;
    const bool hasDifferentSize = m_logs.size() != logsFromEditorLogger.size();
    const bool hasDifferentDateTime = !hasDifferentSize
        && !m_logs.empty()
        && !logsFromEditorLogger.empty()
        && m_logs[0].dateTime != logsFromEditorLogger[0].dateTime;

    if (hasDifferentSize || hasDifferentDateTime)
    {
        mustScrollDown = true;
        m_mustApplyFilter = true;
        m_logs = EditorSingleton::Instance().GetEditorLogger()->GetLogs();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(200);
    if (ImGui::InputTextWithHint("##logger.window.search.text", "Search Log", m_searchText, sizeof(m_searchText)))
        m_mustApplyFilter = true;
    ImGui::PopItemWidth();

    // log level filter
    ImGui::SameLine();
    ImGui::Text("Trace:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##trace", &m_mustFilterTrace))
        m_mustApplyFilter = true;

    ImGui::SameLine();
    ImGui::Text("Debug:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##debug", &m_mustFilterDebug))
        m_mustApplyFilter = true;

    ImGui::SameLine();
    ImGui::Text("Info:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##info", &m_mustFilterInfo))
        m_mustApplyFilter = true;

    ImGui::SameLine();
    ImGui::Text("Warning:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##warning", &m_mustFilterWarning))
        m_mustApplyFilter = true;

    ImGui::SameLine();
    ImGui::Text("Error:");
    ImGui::SameLine();
    if (ImGui::Checkbox("##error", &m_mustFilterError))
        m_mustApplyFilter = true;

    ImGui::SameLine();
    if (ImGui::Button("    Clear    "))
        EditorSingleton::Instance().GetEditorLogger()->Clear();

    ApplyFilter();

    // draw logs
    ImGui::BeginChild("Logger", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (Log log : m_filteredLogs)
        LogView::Draw(log);

    if (mustScrollDown)
        ImGui::SetScrollHereY(1.0f); // scroll to bottom

    ImGui::EndChild();
}

bool LoggerWindow::SearchTextIsEmpty() const
{
    return m_searchText[0] == '\0';
}

void LoggerWindow::ApplyFilter()
{
    if (!m_mustApplyFilter)
        return;

    const bool mustAddBySearchText = !SearchTextIsEmpty();

    m_filteredLogs.clear();

    for (const Log& log : m_logs)
    {
        if ((mustAddBySearchText && log.message.find(m_searchText) != std::string::npos) || !mustAddBySearchText)
            TryAddByLogLevel(log);
    }

    m_mustApplyFilter = false;
}

void LoggerWindow::TryAddByLogLevel(const Log& log)
{
    if (m_mustFilterTrace && log.logLevel == LogLevel::Trace)
        m_filteredLogs.push_back(log);

    if (m_mustFilterDebug && log.logLevel == LogLevel::Debug)
        m_filteredLogs.push_back(log);

    if (m_mustFilterInfo && log.logLevel == LogLevel::Info)
        m_filteredLogs.push_back(log);

    if (m_mustFilterWarning && log.logLevel == LogLevel::Warning)
        m_filteredLogs.push_back(log);

    if (m_mustFilterError && log.logLevel == LogLevel::Error)
        m_filteredLogs.push_back(log);
}
