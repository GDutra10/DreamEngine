#ifndef CORE_SYSTEM_INPUT_SYSTEM_H_
#define CORE_SYSTEM_INPUT_SYSTEM_H_

#include <array>
#include <vector>
#include <unordered_map>

#include "glm/vec2.hpp"

#include "../Inputs/ButtonState.h"
#include "../Inputs/MouseButton.h"
#include "../Inputs/Key.h"


namespace DreamEngine::Core::Systems
{
    using namespace DreamEngine::Core::Inputs;
class InputSystem
{
   public:
    static void EndFrame();
    static glm::vec2 GetMousePosition();
    static ButtonState GetButtonState(Key key);
    static ButtonState GetMouseButtonState(MouseButton mouseButton);

    static void SetKey(const Key key, bool down);
    static void SetMouseButton(const MouseButton mouseButton, bool down);
    static void SetMousePosition(glm::vec2 position);

   private:
    static std::array<ButtonState, Key::KEY_COUNT> m_keys;
    static std::array<ButtonState, MouseButton::MOUSE_BUTTON_COUNT> m_mouseButtons;
    static glm::vec2 m_mousePosition;
};
}  // namespace DreamEngine::Core::Systems
#endif