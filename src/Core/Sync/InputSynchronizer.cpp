#include "InputSynchronizer.h"

#include "Application.h"
#include "Systems/InputSystem.h"

using namespace DreamEngine::Core::Sync;
using namespace DreamEngine::Core::Inputs;
using namespace DreamEngine::Core::Systems;

InputData InputSynchronizer::Synchronize()
{
    InputData inputData = Application::Instance().GetGameData()->inputData;
    // mouse position
    glm::vec2 mousePosition = InputSystem::GetMousePosition();
    inputData.mousePositionX = mousePosition.x;
    inputData.mousePositionY = mousePosition.y;

    // key states
    inputData.keyUpState = InputSystem::GetButtonState(Key::UP).ToButtonStateData();
    inputData.keyDownState = InputSystem::GetButtonState(Key::DOWN).ToButtonStateData();
    inputData.keyLeftState = InputSystem::GetButtonState(Key::LEFT).ToButtonStateData();
    inputData.keyRightState = InputSystem::GetButtonState(Key::RIGHT).ToButtonStateData();

    inputData.keyKpNum0State = InputSystem::GetButtonState(Key::KP_NUM_0).ToButtonStateData();
    inputData.keyKpNum1State = InputSystem::GetButtonState(Key::KP_NUM_1).ToButtonStateData();
    inputData.keyKpNum2State = InputSystem::GetButtonState(Key::KP_NUM_2).ToButtonStateData();
    inputData.keyKpNum3State = InputSystem::GetButtonState(Key::KP_NUM_3).ToButtonStateData();
    inputData.keyKpNum4State = InputSystem::GetButtonState(Key::KP_NUM_4).ToButtonStateData();
    inputData.keyKpNum5State = InputSystem::GetButtonState(Key::KP_NUM_5).ToButtonStateData();
    inputData.keyKpNum6State = InputSystem::GetButtonState(Key::KP_NUM_6).ToButtonStateData();
    inputData.keyKpNum7State = InputSystem::GetButtonState(Key::KP_NUM_7).ToButtonStateData();
    inputData.keyKpNum8State = InputSystem::GetButtonState(Key::KP_NUM_8).ToButtonStateData();
    inputData.keyKpNum9State = InputSystem::GetButtonState(Key::KP_NUM_9).ToButtonStateData();

    inputData.keyNum0State = InputSystem::GetButtonState(Key::NUM_0).ToButtonStateData();
    inputData.keyNum1State = InputSystem::GetButtonState(Key::NUM_1).ToButtonStateData();
    inputData.keyNum2State = InputSystem::GetButtonState(Key::NUM_2).ToButtonStateData();
    inputData.keyNum3State = InputSystem::GetButtonState(Key::NUM_3).ToButtonStateData();
    inputData.keyNum4State = InputSystem::GetButtonState(Key::NUM_4).ToButtonStateData();
    inputData.keyNum5State = InputSystem::GetButtonState(Key::NUM_5).ToButtonStateData();
    inputData.keyNum6State = InputSystem::GetButtonState(Key::NUM_6).ToButtonStateData();
    inputData.keyNum7State = InputSystem::GetButtonState(Key::NUM_7).ToButtonStateData();
    inputData.keyNum8State = InputSystem::GetButtonState(Key::NUM_8).ToButtonStateData();
    inputData.keyNum9State = InputSystem::GetButtonState(Key::NUM_9).ToButtonStateData();

    inputData.keyAState = InputSystem::GetButtonState(Key::A).ToButtonStateData();
    inputData.keyBState = InputSystem::GetButtonState(Key::B).ToButtonStateData();
    inputData.keyCState = InputSystem::GetButtonState(Key::C).ToButtonStateData();
    inputData.keyDState = InputSystem::GetButtonState(Key::D).ToButtonStateData();
    inputData.keyEState = InputSystem::GetButtonState(Key::E).ToButtonStateData();
    inputData.keyFState = InputSystem::GetButtonState(Key::F).ToButtonStateData();
    inputData.keyGState = InputSystem::GetButtonState(Key::G).ToButtonStateData();
    inputData.keyHState = InputSystem::GetButtonState(Key::H).ToButtonStateData();
    inputData.keyIState = InputSystem::GetButtonState(Key::I).ToButtonStateData();
    inputData.keyJState = InputSystem::GetButtonState(Key::J).ToButtonStateData();
    inputData.keyKState = InputSystem::GetButtonState(Key::K).ToButtonStateData();
    inputData.keyLState = InputSystem::GetButtonState(Key::L).ToButtonStateData();
    inputData.keyMState = InputSystem::GetButtonState(Key::M).ToButtonStateData();
    inputData.keyNState = InputSystem::GetButtonState(Key::N).ToButtonStateData();
    inputData.keyOState = InputSystem::GetButtonState(Key::O).ToButtonStateData();
    inputData.keyPState = InputSystem::GetButtonState(Key::P).ToButtonStateData();
    inputData.keyQState = InputSystem::GetButtonState(Key::Q).ToButtonStateData();
    inputData.keyRState = InputSystem::GetButtonState(Key::R).ToButtonStateData();
    inputData.keySState = InputSystem::GetButtonState(Key::S).ToButtonStateData();
    inputData.keyTState = InputSystem::GetButtonState(Key::T).ToButtonStateData();
    inputData.keyUState = InputSystem::GetButtonState(Key::U).ToButtonStateData();
    inputData.keyVState = InputSystem::GetButtonState(Key::V).ToButtonStateData();
    inputData.keyWState = InputSystem::GetButtonState(Key::W).ToButtonStateData();
    inputData.keyXState = InputSystem::GetButtonState(Key::X).ToButtonStateData();
    inputData.keyYState = InputSystem::GetButtonState(Key::Y).ToButtonStateData();
    inputData.keyZState = InputSystem::GetButtonState(Key::Z).ToButtonStateData();

    inputData.keyF1State = InputSystem::GetButtonState(Key::F1).ToButtonStateData();
    inputData.keyF2State = InputSystem::GetButtonState(Key::F2).ToButtonStateData();
    inputData.keyF3State = InputSystem::GetButtonState(Key::F3).ToButtonStateData();
    inputData.keyF4State = InputSystem::GetButtonState(Key::F4).ToButtonStateData();
    inputData.keyF5State = InputSystem::GetButtonState(Key::F5).ToButtonStateData();
    inputData.keyF6State = InputSystem::GetButtonState(Key::F6).ToButtonStateData();
    inputData.keyF7State = InputSystem::GetButtonState(Key::F7).ToButtonStateData();
    inputData.keyF8State = InputSystem::GetButtonState(Key::F8).ToButtonStateData();
    inputData.keyF9State = InputSystem::GetButtonState(Key::F9).ToButtonStateData();
    inputData.keyF10State = InputSystem::GetButtonState(Key::F10).ToButtonStateData();
    inputData.keyF11State = InputSystem::GetButtonState(Key::F11).ToButtonStateData();
    inputData.keyF12State = InputSystem::GetButtonState(Key::F12).ToButtonStateData();

    inputData.keyTabState = InputSystem::GetButtonState(Key::TAB).ToButtonStateData();
    inputData.keyEscapeState = InputSystem::GetButtonState(Key::ESCAPE).ToButtonStateData();
    inputData.keyLeftAltState = InputSystem::GetButtonState(Key::LEFT_ALT).ToButtonStateData();
    inputData.keyRightAltState = InputSystem::GetButtonState(Key::RIGHT_ALT).ToButtonStateData();
    inputData.keyLeftControlState = InputSystem::GetButtonState(Key::LEFT_CONTROL).ToButtonStateData();
    inputData.keyRightControlState = InputSystem::GetButtonState(Key::RIGHT_CONTROL).ToButtonStateData();
    inputData.keyLeftShiftState = InputSystem::GetButtonState(Key::LEFT_SHIFT).ToButtonStateData();
    inputData.keyRightShiftState = InputSystem::GetButtonState(Key::RIGHT_SHIFT).ToButtonStateData();
    inputData.keySpaceState = InputSystem::GetButtonState(Key::SPACE).ToButtonStateData();
    inputData.keyBackspaceState = InputSystem::GetButtonState(Key::BACKSPACE).ToButtonStateData();
    inputData.keySlashState = InputSystem::GetButtonState(Key::SLASH).ToButtonStateData();
    inputData.keyBackslashState = InputSystem::GetButtonState(Key::BACKSLASH).ToButtonStateData();
    inputData.keyCapsLockState = InputSystem::GetButtonState(Key::CAPS_LOCK).ToButtonStateData();
    inputData.keyEnterState = InputSystem::GetButtonState(Key::ENTER).ToButtonStateData();
    inputData.keyApostropheState = InputSystem::GetButtonState(Key::APOSTROPHE).ToButtonStateData();
    inputData.keyCommaState = InputSystem::GetButtonState(Key::COMMA).ToButtonStateData();
    inputData.keyDeleteState = InputSystem::GetButtonState(Key::DELETE).ToButtonStateData();
    inputData.keyPageUpState = InputSystem::GetButtonState(Key::PAGE_UP).ToButtonStateData();
    inputData.keyPageDownState = InputSystem::GetButtonState(Key::PAGE_DOWN).ToButtonStateData();
    inputData.keyInsertState = InputSystem::GetButtonState(Key::INSERT).ToButtonStateData();
    inputData.keyHomeState = InputSystem::GetButtonState(Key::HOME).ToButtonStateData();
    inputData.keyEndState = InputSystem::GetButtonState(Key::END).ToButtonStateData();
    inputData.keyPrintScreenState = InputSystem::GetButtonState(Key::PRINT_SCREEN).ToButtonStateData();
    inputData.keyScrollLockState = InputSystem::GetButtonState(Key::SCROLL_LOCK).ToButtonStateData();
    inputData.keyPauseState = InputSystem::GetButtonState(Key::PAUSE).ToButtonStateData();
    inputData.keyEqualState = InputSystem::GetButtonState(Key::EQUAL).ToButtonStateData();
    inputData.keyGraveAccentState = InputSystem::GetButtonState(Key::GRAVE_ACCENT).ToButtonStateData();
    inputData.keySemicolonState = InputSystem::GetButtonState(Key::SEMICOLON).ToButtonStateData();
    inputData.keyMinusState = InputSystem::GetButtonState(Key::MINUS).ToButtonStateData();
    inputData.keyLeftBracketState = InputSystem::GetButtonState(Key::LEFT_BRACKET).ToButtonStateData();
    inputData.keyRightBracketState = InputSystem::GetButtonState(Key::RIGHT_BRACKET).ToButtonStateData();
    inputData.keyPeriodState = InputSystem::GetButtonState(Key::PERIOD).ToButtonStateData();
    inputData.keyWorld1State = InputSystem::GetButtonState(Key::WORLD_1).ToButtonStateData();
    inputData.keyWorld2State = InputSystem::GetButtonState(Key::WORLD_2).ToButtonStateData();

    // mouse button states
    inputData.mouseButtonLeftState = InputSystem::GetMouseButtonState(MouseButton::MOUSE_BUTTON_LEFT).ToButtonStateData();
    inputData.mouseButtonMiddleState = InputSystem::GetMouseButtonState(MouseButton::MOUSE_BUTTON_MIDDLE).ToButtonStateData();
    inputData.mouseButtonRightState = InputSystem::GetMouseButtonState(MouseButton::MOUSE_BUTTON_RIGHT).ToButtonStateData();
    inputData.mouseButton4State = InputSystem::GetMouseButtonState(MouseButton::MOUSE_BUTTON_4).ToButtonStateData();
    inputData.mouseButton5State = InputSystem::GetMouseButtonState(MouseButton::MOUSE_BUTTON_5).ToButtonStateData();
    inputData.mouseButton6State = InputSystem::GetMouseButtonState(MouseButton::MOUSE_BUTTON_6).ToButtonStateData();
    inputData.mouseButton7State = InputSystem::GetMouseButtonState(MouseButton::MOUSE_BUTTON_7).ToButtonStateData();
    inputData.mouseButton8State = InputSystem::GetMouseButtonState(MouseButton::MOUSE_BUTTON_8).ToButtonStateData();

    return inputData;
}
