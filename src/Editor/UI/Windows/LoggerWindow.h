#ifndef EDITOR_UI_WINDOWS_LOGGER_WINDOW_H_
#define EDITOR_UI_WINDOWS_LOGGER_WINDOW_H_

#include "BaseWindow.h"
#include "../../../Core/Loggers/Log.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace DreamEngine::Core::Loggers;
class LoggerWindow : public BaseWindow
{
   public:
    LoggerWindow(const std::string& title);

   protected:
    void DrawContent() override;

   private:
    char m_searchText[128] = "";
    int m_currentLogLevel = 0;
    std::vector<Log> m_logs;
    std::vector<Log> m_filteredLogs;
    bool m_mustApplyFilter = false;
    bool m_mustFilterTrace = false;
    bool m_mustFilterDebug = true;
    bool m_mustFilterInfo = true;
    bool m_mustFilterWarning = true;
    bool m_mustFilterError = true;
    [[nodiscard]] bool SearchTextIsEmpty() const;
    void ApplyFilter();
    void TryAddByLogLevel(const Log& log);
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif