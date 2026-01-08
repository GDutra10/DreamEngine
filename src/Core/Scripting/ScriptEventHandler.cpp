#include "ScriptEventHandler.h"
#include "ScriptEngine.h"

using namespace DreamEngine::Core::Scripting;

std::queue<uint32_t> ScriptEventHandler::m_sEventIds;

void ScriptEventHandler::QueueEvent(uint32_t eventId)
{
    m_sEventIds.emplace(eventId);
}

void ScriptEventHandler::Process()
{
    while (!m_sEventIds.empty())
    {
        uint32_t eventId = m_sEventIds.front();
        m_sEventIds.pop();

        // Process the event with eventId
        ScriptEngine::ProcessEvent(eventId);
    }
}