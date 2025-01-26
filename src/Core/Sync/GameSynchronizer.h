#ifndef CORE_SYNC_GAME_SYNCHRONIZER_H_
#define CORE_SYNC_GAME_SYNCHRONIZER_H_
#include "GameData.h"

namespace DreamEngine::Core::Sync
{
class GameSynchronizer
{
   public:
    static GameData* Synchronize();
};
}  // namespace DreamEngine::Core::Sync
#endif