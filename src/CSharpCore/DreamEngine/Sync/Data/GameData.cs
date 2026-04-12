using System.Runtime.InteropServices;

namespace DreamEngine.Sync.Data;

[StructLayout(LayoutKind.Sequential)]
internal struct GameData
{
    public float deltaTime;
    public InputData inputData;
    public SceneData sceneData;
}
