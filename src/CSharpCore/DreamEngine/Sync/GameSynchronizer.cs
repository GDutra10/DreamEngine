using DreamEngine.Sync.Data;

namespace DreamEngine.Sync;
internal static class GameSynchronizer
{
    public static unsafe void Synchronize(ref GameData gameData, EntityData* entities, int entityCount)
    {
        Game.DeltaTime = gameData.deltaTime;
        InputSynchronizer.Synchronize(ref gameData.inputData);
        SceneSynchronizer.SynchronizeFromTo(ref gameData.sceneData, Game.Scene, entities, entityCount);
    }
}
