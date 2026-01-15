
#ifndef CORE_SYNC_GAME_DATA_H_
#define CORE_SYNC_GAME_DATA_H_

#include <cstdint>

#include "InputData.h"

namespace DreamEngine::Core::Sync
{
struct GameData
{
    float deltaTime;
    uint8_t mustRecreateEntities;
    InputData inputData;
};
}  // namespace DreamEngine::Core::Sync
#endif