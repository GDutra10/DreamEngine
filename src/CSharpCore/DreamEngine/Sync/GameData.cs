using System.Runtime.InteropServices;

namespace DreamEngine.Sync;

[StructLayout(LayoutKind.Sequential)]
internal struct GameData
{
    public float deltaTime;
    public InputData inputData;
}
