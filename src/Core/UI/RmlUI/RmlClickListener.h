#ifndef CORE_UI_RML_RML_CLICK_LISTENER_H_
#define CORE_UI_RML_RML_CLICK_LISTENER_H_

#include "RmlUi/Core/EventListener.h"

namespace DreamEngine::Core::UI::RmlUI
{
class RmlClickListener : public Rml::EventListener
{
   public:
    explicit RmlClickListener(std::function<void()> callback) : m_callback(std::move(callback)) {}
    void ProcessEvent(Rml::Event& event) override
    {
        m_callback();
        event.StopPropagation();
    }

   private:
    std::function<void()> m_callback;
};
}  // namespace DreamEngine::Core::UI::RmlUI
#endif