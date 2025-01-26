#ifndef CORE_SYNC_INPUT_SYNCHRONIZER_H_
#define CORE_SYNC_INPUT_SYNCHRONIZER_H_
#include "InputData.h"

namespace DreamEngine::Core::Sync
{
class InputSynchronizer
{
   public:
    static InputData* Synchronize();
};
}  // namespace DreamEngine::Core::Sync
#endif