#ifndef CORE_INPUTS_INPUT_H_
#define CORE_INPUTS_INPUT_H_

#include "glm/vec2.hpp"

#include "Key.h"
#include "MouseButton.h"

namespace DreamEngine::Core::Inputs
{
class CORE_API Input
{
   public:
    static glm::vec2 GetMousePosition();

    // first frame the key goes down
    static bool IsKeyPressed(Key key);
    // every frame while held
    static bool IsKeyDown(Key key);
    // first frame the key goes up
    static bool IsKeyReleased(Key key);

    // first frame the key goes down
    static bool IsMouseClickPressed(MouseButton mouseButton);
    // every frame while held
    static bool IsMouseClickDown(MouseButton mouseButton);
    // first frame the key goes down
    static bool IsMouseClickReleased(MouseButton mouseButton);
   private:
    
};
}  // namespace DreamEngine::Core
#endif