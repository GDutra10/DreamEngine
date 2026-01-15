#include "GameSynchronizer.h"

#include "Application.h"
#include "InputSynchronizer.h"

using namespace DreamEngine::Core::Sync;

GameData* GameSynchronizer::Synchronize(const bool isFocused, const bool mustRecreateEntities)
{
    GameData* gameData = Application::Instance().GetGameData();
    gameData->deltaTime = Application::Instance().GetDeltaTime();
    gameData->mustRecreateEntities = mustRecreateEntities ? 1 : 0;

    if (isFocused)
        gameData->inputData = *InputSynchronizer::Synchronize();

    return gameData;
}
