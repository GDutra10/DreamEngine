#include "EditorLogger.h"
#include "../../Core/Helpers/DateHelper.h"
#include "../../Core/Helpers/LoggerHelper.h"

using namespace DreamEngine::Editor::Loggers;
using namespace DreamEngine::Core::Helpers;

#ifndef MAXIMUM_LOG_IN_VECTOR
#define MAXIMUM_LOG_IN_VECTOR 1000
#endif

EditorLogger::EditorLogger()
{
    m_logs = std::vector<Log>();
}

void EditorLogger::Process(const Log log)
{
    Log newLog = log;
    newLog.message = 
        DateHelper::ToString(log.dateTime) + 
        " " +
        LoggerHelper::GetLogLevel(log) +
        " " +
        log.message;

    if (m_logs.size() >= MAXIMUM_LOG_IN_VECTOR)
    {
        m_logs.erase(m_logs.begin());
    }

    m_logs.push_back(newLog);
}

void EditorLogger::Clear()
{
    m_logs.clear();
}

std::vector<Log>& EditorLogger::GetLogs()
{
    return m_logs;
}