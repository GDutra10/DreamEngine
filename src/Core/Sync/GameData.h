#include "InputData.h"
#ifndef CORE_SYNC_GAME_DATA_H_
#define CORE_SYNC_GAME_DATA_H_

namespace DreamEngine::Core::Sync
{
struct GameData
{
    float deltaTime;
    InputData inputData;
};
}  // namespace DreamEngine::Core::Sync
#endif