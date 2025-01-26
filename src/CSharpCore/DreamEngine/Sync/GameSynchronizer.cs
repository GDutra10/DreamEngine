namespace DreamEngine.Sync;
internal static class GameSynchronizer
{
    public static void Synchronize(ref GameData gameData)
    {
        Game.DeltaTime = gameData.deltaTime;
        
        // TODO: scene data

        InputSynchronizer.Synchronize(ref gameData.inputData);
    }
}
