namespace DreamEngine;
public static class Game
{
    /// <summary>
    /// The "difference" in time (delta) between the last frame and the current one
    /// </summary>
    public static float DeltaTime { get; internal set; }

    /// <summary>
    /// Current scene running in the game
    /// </summary>
    public static Scene Scene { get; internal set; } = new();
}
