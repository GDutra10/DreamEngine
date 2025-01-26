#include "Input.h"

using namespace DreamEngine::Core::Inputs;

std::unordered_map<int, KeyState> Input::keyStates;
std::unordered_map<int, KeyState> Input::mouseButtonStates;
glm::vec2 Input::mousePosition;

glm::vec2 Input::GetMousePosition()
{
    return mousePosition;
}

bool Input::IsPressed(const Key key)
{
    return keyStates[key] == KeyState::Press;
}

bool Input::IsPressed(const MouseButton mouseButton)
{
    return mouseButtonStates[mouseButton] == KeyState::Press;
}

void Input::SetKeyState(const Key key, const KeyState keyState)
{
    keyStates[key] = keyState;
}

void Input::SetMouseState(const MouseButton mouseButton, const KeyState keyState)
{
    mouseButtonStates[mouseButton] = keyState;
}

void Input::SetMousePosition(const glm::vec2 position)
{
    mousePosition = position;
}
