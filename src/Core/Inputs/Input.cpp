#include "Input.h"

#include "../Systems/InputSystem.h"

using namespace DreamEngine::Core::Inputs;

using namespace DreamEngine::Core::Systems;

glm::vec2 Input::GetMousePosition()
{
    return InputSystem::GetMousePosition();
}

bool Input::IsKeyPressed(Key key)
{
    const auto& state = InputSystem::GetButtonState(key);

    return state.current && !state.previous;
}

bool Input::IsKeyDown(Key key)
{
    return InputSystem::GetButtonState(key).current;
}

bool Input::IsKeyReleased(Key key)
{
    const auto& state = InputSystem::GetButtonState(key);

    return !state.current && state.previous;
}

bool Input::IsMouseClickPressed(MouseButton mouseButton)
{
    const auto& state = InputSystem::GetMouseButtonState(mouseButton);

    return state.current && !state.previous;
}

bool Input::IsMouseClickDown(MouseButton mouseButton)
{
    return InputSystem::GetMouseButtonState(mouseButton).current;
}

bool Input::IsMouseClickReleased(MouseButton mouseButton)
{
    const auto& state = InputSystem::GetMouseButtonState(mouseButton);

    return !state.current && state.previous;
}