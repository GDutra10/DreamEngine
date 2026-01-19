
#ifndef CORE_SYNC_GAME_DATA_H_
#define CORE_SYNC_GAME_DATA_H_

#include <cstdint>

#include "InputData.h"
#include "SceneData.h"

namespace DreamEngine::Core::Sync
{
struct GameData
{
    float deltaTime;
    InputData inputData;
    SceneData sceneData;
};
}  // namespace DreamEngine::Core::Sync
#endif