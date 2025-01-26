#include "InputSynchronizer.h"

#include "Application.h"
#include "Inputs/Input.h"

using namespace DreamEngine::Core::Sync;
using namespace DreamEngine::Core::Inputs;

InputData* InputSynchronizer::Synchronize()
{
    InputData inputData = Application::Instance().GetGameData()->inputData;
    // mouse position
    glm::vec2 mousePosition = Input::GetMousePosition();
    inputData.mousePositionX = mousePosition.x;
    inputData.mousePositionY = mousePosition.y;

    // key states
    inputData.keyUpState = Input::keyStates[Key::UP];
    inputData.keyDownState = Input::keyStates[Key::DOWN];
    inputData.keyLeftState = Input::keyStates[Key::LEFT];
    inputData.keyRightState = Input::keyStates[Key::RIGHT];

    inputData.keyKpNum0State = Input::keyStates[Key::KP_NUM_0];
    inputData.keyKpNum1State = Input::keyStates[Key::KP_NUM_1];
    inputData.keyKpNum2State = Input::keyStates[Key::KP_NUM_2];
    inputData.keyKpNum3State = Input::keyStates[Key::KP_NUM_3];
    inputData.keyKpNum4State = Input::keyStates[Key::KP_NUM_4];
    inputData.keyKpNum5State = Input::keyStates[Key::KP_NUM_5];
    inputData.keyKpNum6State = Input::keyStates[Key::KP_NUM_6];
    inputData.keyKpNum7State = Input::keyStates[Key::KP_NUM_7];
    inputData.keyKpNum8State = Input::keyStates[Key::KP_NUM_8];
    inputData.keyKpNum9State = Input::keyStates[Key::KP_NUM_9];

    inputData.keyNum0State = Input::keyStates[Key::NUM_0];
    inputData.keyNum1State = Input::keyStates[Key::NUM_1];
    inputData.keyNum2State = Input::keyStates[Key::NUM_2];
    inputData.keyNum3State = Input::keyStates[Key::NUM_3];
    inputData.keyNum4State = Input::keyStates[Key::NUM_4];
    inputData.keyNum5State = Input::keyStates[Key::NUM_5];
    inputData.keyNum6State = Input::keyStates[Key::NUM_6];
    inputData.keyNum7State = Input::keyStates[Key::NUM_7];
    inputData.keyNum8State = Input::keyStates[Key::NUM_8];
    inputData.keyNum9State = Input::keyStates[Key::NUM_9];

    inputData.keyAState = Input::keyStates[Key::A];
    inputData.keyBState = Input::keyStates[Key::B];
    inputData.keyCState = Input::keyStates[Key::C];
    inputData.keyDState = Input::keyStates[Key::D];
    inputData.keyEState = Input::keyStates[Key::E];
    inputData.keyFState = Input::keyStates[Key::F];
    inputData.keyGState = Input::keyStates[Key::G];
    inputData.keyHState = Input::keyStates[Key::H];
    inputData.keyIState = Input::keyStates[Key::I];
    inputData.keyJState = Input::keyStates[Key::J];
    inputData.keyKState = Input::keyStates[Key::K];
    inputData.keyLState = Input::keyStates[Key::L];
    inputData.keyMState = Input::keyStates[Key::M];
    inputData.keyNState = Input::keyStates[Key::N];
    inputData.keyOState = Input::keyStates[Key::O];
    inputData.keyPState = Input::keyStates[Key::P];
    inputData.keyQState = Input::keyStates[Key::Q];
    inputData.keyRState = Input::keyStates[Key::R];
    inputData.keySState = Input::keyStates[Key::S];
    inputData.keyTState = Input::keyStates[Key::T];
    inputData.keyUState = Input::keyStates[Key::U];
    inputData.keyVState = Input::keyStates[Key::V];
    inputData.keyWState = Input::keyStates[Key::W];
    inputData.keyXState = Input::keyStates[Key::X];
    inputData.keyYState = Input::keyStates[Key::Y];
    inputData.keyZState = Input::keyStates[Key::Z];

    inputData.keyF1State = Input::keyStates[Key::F1];
    inputData.keyF2State = Input::keyStates[Key::F2];
    inputData.keyF3State = Input::keyStates[Key::F3];
    inputData.keyF4State = Input::keyStates[Key::F4];
    inputData.keyF5State = Input::keyStates[Key::F5];
    inputData.keyF6State = Input::keyStates[Key::F6];
    inputData.keyF7State = Input::keyStates[Key::F7];
    inputData.keyF8State = Input::keyStates[Key::F8];
    inputData.keyF9State = Input::keyStates[Key::F9];
    inputData.keyF10State = Input::keyStates[Key::F10];
    inputData.keyF11State = Input::keyStates[Key::F11];
    inputData.keyF12State = Input::keyStates[Key::F12];

    inputData.keyTabState = Input::keyStates[Key::TAB];
    inputData.keyEscapeState = Input::keyStates[Key::ESCAPE];
    inputData.keyLeftAltState = Input::keyStates[Key::LEFT_ALT];
    inputData.keyRightAltState = Input::keyStates[Key::RIGHT_ALT];
    inputData.keyLeftControlState = Input::keyStates[Key::LEFT_CONTROL];
    inputData.keyRightControlState = Input::keyStates[Key::RIGHT_CONTROL];
    inputData.keyLeftShiftState = Input::keyStates[Key::LEFT_SHIFT];
    inputData.keyRightShiftState = Input::keyStates[Key::RIGHT_SHIFT];
    inputData.keySpaceState = Input::keyStates[Key::SPACE];
    inputData.keyBackspaceState = Input::keyStates[Key::BACKSPACE];
    inputData.keySlashState = Input::keyStates[Key::SLASH];
    inputData.keyBackslashState = Input::keyStates[Key::BACKSLASH];
    inputData.keyCapsLockState = Input::keyStates[Key::CAPS_LOCK];
    inputData.keyEnterState = Input::keyStates[Key::ENTER];
    inputData.keyApostropheState = Input::keyStates[Key::APOSTROPHE];
    inputData.keyCommaState = Input::keyStates[Key::COMMA];
    inputData.keyDeleteState = Input::keyStates[Key::DELETE];
    inputData.keyPageUpState = Input::keyStates[Key::PAGE_UP];
    inputData.keyPageDownState = Input::keyStates[Key::PAGE_DOWN];
    inputData.keyInsertState = Input::keyStates[Key::INSERT];
    inputData.keyHomeState = Input::keyStates[Key::HOME];
    inputData.keyEndState = Input::keyStates[Key::END];
    inputData.keyPrintScreenState = Input::keyStates[Key::PRINT_SCREEN];
    inputData.keyScrollLockState = Input::keyStates[Key::SCROLL_LOCK];
    inputData.keyPauseState = Input::keyStates[Key::PAUSE];
    inputData.keyEqualState = Input::keyStates[Key::EQUAL];
    inputData.keyGraveAccentState = Input::keyStates[Key::GRAVE_ACCENT];
    inputData.keySemicolonState = Input::keyStates[Key::SEMICOLON];
    inputData.keyMinusState = Input::keyStates[Key::MINUS];
    inputData.keyLeftBracketState = Input::keyStates[Key::LEFT_BRACKET];
    inputData.keyRightBracketState = Input::keyStates[Key::RIGHT_BRACKET];
    inputData.keyPeriodState = Input::keyStates[Key::PERIOD];
    inputData.keyWorld1State = Input::keyStates[Key::WORLD_1];
    inputData.keyWorld2State = Input::keyStates[Key::WORLD_2];

    // mouse button states
    inputData.mouseButtonLeftState = Input::mouseButtonStates[MouseButton::MOUSE_BUTTON_LEFT];
    inputData.mouseButtonMiddleState = Input::mouseButtonStates[MouseButton::MOUSE_BUTTON_MIDDLE];
    inputData.mouseButtonRightState = Input::mouseButtonStates[MouseButton::MOUSE_BUTTON_RIGHT];
    inputData.mouseButton4State = Input::mouseButtonStates[MouseButton::MOUSE_BUTTON_4];
    inputData.mouseButton5State = Input::mouseButtonStates[MouseButton::MOUSE_BUTTON_5];
    inputData.mouseButton6State = Input::mouseButtonStates[MouseButton::MOUSE_BUTTON_6];
    inputData.mouseButton7State = Input::mouseButtonStates[MouseButton::MOUSE_BUTTON_7];
    inputData.mouseButton8State = Input::mouseButtonStates[MouseButton::MOUSE_BUTTON_8];

    return &inputData;
}
