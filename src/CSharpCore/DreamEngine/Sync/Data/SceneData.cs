using System.Runtime.InteropServices;

namespace DreamEngine.Sync.Data;

[StructLayout(LayoutKind.Sequential)]
internal struct SceneData
{
    public uint mainCameraEntityId;
    public byte showCursor;
    public byte mustRecreateEntities;
    public float globalLightColorR;
    public float globalLightColorG;
    public float globalLightColorB;
    public float globalLightIntensity;
}
