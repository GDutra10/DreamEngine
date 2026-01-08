#ifndef CORE_UI_RML_RML_CLICK_BIND_H_
#define CORE_UI_RML_RML_CLICK_BIND_H_

#include <functional>
#include <string>

namespace DreamEngine::Core::UI::RmlUI
{
    struct RmlClickBinding
    {
        std::string elementId;
        std::function<void()> callback;
    };
}
#endif