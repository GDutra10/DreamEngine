#include "GameSynchronizer.h"

#include "Application.h"
#include "InputSynchronizer.h"

using namespace DreamEngine::Core::Sync;

GameData* GameSynchronizer::Synchronize()
{
    GameData* gameData = Application::Instance().GetGameData();
    gameData->deltaTime = Application::Instance().GetDeltaTime();

    gameData->inputData = *InputSynchronizer::Synchronize();

    return gameData;
}
