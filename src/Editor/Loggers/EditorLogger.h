#ifndef EDITOR_LOGGERS_EDITOR_LOGGER_H_
#define EDITOR_LOGGERS_EDITOR_LOGGER_H_

#include "../../Core/Loggers/Logger.h"

namespace DreamEngine::Editor::Loggers
{
using namespace DreamEngine::Core::Loggers;

class EditorLogger final : public Logger
{
   public:
    EditorLogger();

    void Process(const Log log) override;

    void Clear();

    std::vector<Log>& GetLogs();

   private:
    std::vector<Log> m_logs;
};

}  // namespace DreamEngine::Editor::Loggers
#endif