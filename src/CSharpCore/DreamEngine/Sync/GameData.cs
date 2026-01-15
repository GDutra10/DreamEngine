using System.Runtime.InteropServices;

namespace DreamEngine.Sync;

[StructLayout(LayoutKind.Sequential)]
internal struct GameData
{
    public float deltaTime;
    public byte mustRecreateEntities;
    public InputData inputData;
}
