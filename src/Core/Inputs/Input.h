#ifndef CORE_INPUTS_INPUT_H_
#define CORE_INPUTS_INPUT_H_

#include <unordered_map>
#include "Key.h"
#include "KeyState.h"
#include "MouseButton.h"
#include "glm/vec2.hpp"

namespace DreamEngine::Core::Inputs
{
class Input
{
   public:
    static glm::vec2 mousePosition;
    static std::unordered_map<int, KeyState> keyStates;
    static std::unordered_map<int, KeyState> mouseButtonStates;
    static glm::vec2 GetMousePosition();
    static bool IsPressed(Key key);
    static bool IsPressed(MouseButton mouseButton);
    static void SetKeyState(const Key key, const KeyState keyState);
    static void SetMouseState(const MouseButton mouseButton, const KeyState keyState);
    static void SetMousePosition(glm::vec2 position);

   private:
    
};
}  // namespace DreamEngine::Core
#endif