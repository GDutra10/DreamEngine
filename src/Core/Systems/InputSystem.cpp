#include "InputSystem.h"

using namespace DreamEngine::Core::Systems;

std::array<ButtonState, Key::KEY_COUNT> InputSystem::m_keys{};
std::array<ButtonState, MouseButton::MOUSE_BUTTON_COUNT> InputSystem::m_mouseButtons{};
glm::vec2 InputSystem::m_mousePosition{};

void InputSystem::SetKey(const Key key, bool down)
{
    m_keys[static_cast<size_t>(key)].current = down;
}

void InputSystem::SetMouseButton(const MouseButton mouseButton, bool down)
{
    m_mouseButtons[static_cast<size_t>(mouseButton)].current = down;
}

void InputSystem::SetMousePosition(const glm::vec2 position)
{
    m_mousePosition = position;
}

void InputSystem::EndFrame() 
{
    for (auto& state : m_keys)
        state.previous = state.current;

    for (auto& state : m_mouseButtons)
        state.previous = state.current;
}

glm::vec2 InputSystem::GetMousePosition()
{
    return m_mousePosition;
}

ButtonState InputSystem::GetButtonState(Key key)
{
    return m_keys[key];
}

ButtonState InputSystem::GetMouseButtonState(MouseButton mouseButton)
{
    return m_mouseButtons[mouseButton];
}
