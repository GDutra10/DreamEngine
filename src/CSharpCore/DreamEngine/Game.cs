namespace DreamEngine;
public static class Game
{
    public static float DeltaTime { get; internal set; }
    public static Scene Scene { get; internal set; } = new();
}
