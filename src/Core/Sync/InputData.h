#ifndef CORE_SYNC_INPUT_DATA_H_
#define CORE_SYNC_INPUT_DATA_H_

#include "../Core/Inputs/Input.h"

#include <stdint.h>

namespace DreamEngine::Core::Sync
{
using namespace DreamEngine::Core::Inputs;

struct ButtonStateData
{
    uint8_t current;
    uint8_t previous;
};

struct InputData
{
    float mousePositionX;
    float mousePositionY;
    ButtonStateData keyKpNum0State;
    ButtonStateData keyKpNum1State;
    ButtonStateData keyKpNum2State;
    ButtonStateData keyKpNum3State;
    ButtonStateData keyKpNum4State;
    ButtonStateData keyKpNum5State;
    ButtonStateData keyKpNum6State;
    ButtonStateData keyKpNum7State;
    ButtonStateData keyKpNum8State;
    ButtonStateData keyKpNum9State;
    ButtonStateData keyNum0State;
    ButtonStateData keyNum1State;
    ButtonStateData keyNum2State;
    ButtonStateData keyNum3State;
    ButtonStateData keyNum4State;
    ButtonStateData keyNum5State;
    ButtonStateData keyNum6State;
    ButtonStateData keyNum7State;
    ButtonStateData keyNum8State;
    ButtonStateData keyNum9State;
    ButtonStateData keyAState;
    ButtonStateData keyBState;
    ButtonStateData keyCState;
    ButtonStateData keyDState;
    ButtonStateData keyEState;
    ButtonStateData keyFState;
    ButtonStateData keyGState;
    ButtonStateData keyHState;
    ButtonStateData keyIState;
    ButtonStateData keyJState;
    ButtonStateData keyKState;
    ButtonStateData keyLState;
    ButtonStateData keyMState;
    ButtonStateData keyNState;
    ButtonStateData keyOState;
    ButtonStateData keyPState;
    ButtonStateData keyQState;
    ButtonStateData keyRState;
    ButtonStateData keySState;
    ButtonStateData keyTState;
    ButtonStateData keyUState;
    ButtonStateData keyVState;
    ButtonStateData keyWState;
    ButtonStateData keyXState;
    ButtonStateData keyYState;
    ButtonStateData keyZState;
    ButtonStateData keyF1State;
    ButtonStateData keyF2State;
    ButtonStateData keyF3State;
    ButtonStateData keyF4State;
    ButtonStateData keyF5State;
    ButtonStateData keyF6State;
    ButtonStateData keyF7State;
    ButtonStateData keyF8State;
    ButtonStateData keyF9State;
    ButtonStateData keyF10State;
    ButtonStateData keyF11State;
    ButtonStateData keyF12State;
    ButtonStateData keyTabState;
    ButtonStateData keyEscapeState;
    ButtonStateData keyLeftAltState;
    ButtonStateData keyRightAltState;
    ButtonStateData keyLeftControlState;
    ButtonStateData keyRightControlState;
    ButtonStateData keyLeftShiftState;
    ButtonStateData keyRightShiftState;
    ButtonStateData keySpaceState;
    ButtonStateData keyBackspaceState;
    ButtonStateData keySlashState;
    ButtonStateData keyBackslashState;
    ButtonStateData keyCapsLockState;
    ButtonStateData keyEnterState;
    ButtonStateData keyApostropheState;
    ButtonStateData keyCommaState;
    ButtonStateData keyDeleteState;
    ButtonStateData keyPageUpState;
    ButtonStateData keyPageDownState;
    ButtonStateData keyInsertState;
    ButtonStateData keyHomeState;
    ButtonStateData keyEndState;
    ButtonStateData keyPrintScreenState;
    ButtonStateData keyScrollLockState;
    ButtonStateData keyPauseState;
    ButtonStateData keyUpState;
    ButtonStateData keyDownState;
    ButtonStateData keyLeftState;
    ButtonStateData keyRightState;
    ButtonStateData keyEqualState;
    ButtonStateData keyGraveAccentState;
    ButtonStateData keySemicolonState;
    ButtonStateData keyMinusState;
    ButtonStateData keyLeftBracketState;
    ButtonStateData keyRightBracketState;
    ButtonStateData keyPeriodState;
    ButtonStateData keyWorld1State;
    ButtonStateData keyWorld2State;
    ButtonStateData mouseButtonLeftState;
    ButtonStateData mouseButtonMiddleState;
    ButtonStateData mouseButtonRightState;
    ButtonStateData mouseButton4State;
    ButtonStateData mouseButton5State;
    ButtonStateData mouseButton6State;
    ButtonStateData mouseButton7State;
    ButtonStateData mouseButton8State;
};
}  // namespace DreamEngine::Core::Sync
#endif