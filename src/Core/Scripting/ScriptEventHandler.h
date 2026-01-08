#ifndef CORE_SCRIPTING_SCRIPT_EVENT_HANDLER_H_
#define CORE_SCRIPTING_SCRIPT_EVENT_HANDLER_H_

#include <queue>

namespace DreamEngine::Core::Scripting
{
class ScriptEventHandler
{
public:
    static void QueueEvent(uint32_t eventId);
    static void Process();
   private:
 static std::queue<uint32_t> m_sEventIds;
};
}  // namespace DreamEngine::Core::Scripting
#endif