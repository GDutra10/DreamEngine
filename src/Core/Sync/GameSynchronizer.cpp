#include "GameSynchronizer.h"

#include "Application.h"
#include "InputSynchronizer.h"

using namespace DreamEngine::Core::Sync;

GameData* GameSynchronizer::Synchronize(const bool isFocused)
{
    GameData* gameData = Application::Instance().GetGameData();
    gameData->deltaTime = Application::Instance().GetDeltaTime();
    gameData->sceneData = *Application::Instance().GetGame()->GetActiveScene()->GetSceneData();

    if (isFocused)
        gameData->inputData = *InputSynchronizer::Synchronize();

    return gameData;
}
