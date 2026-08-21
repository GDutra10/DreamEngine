#ifndef CORE_INPUTS_BUTTON_STATE_H_
#define CORE_INPUTS_BUTTON_STATE_H_

#include "../Sync/InputData.h"

namespace DreamEngine::Core::Inputs
{
    using namespace DreamEngine::Core::Sync;
struct ButtonState
{
    bool current = false;
    bool previous = false;

    ButtonStateData ToButtonStateData() { return {current, previous}; }
};
}  // namespace DreamEngine::Core
#endif