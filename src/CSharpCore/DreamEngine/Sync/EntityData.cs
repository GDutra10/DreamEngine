using System.Runtime.InteropServices;

namespace DreamEngine.Sync;

[StructLayout(LayoutKind.Sequential)]
internal struct EntityData
{
    public float transformPositionX;
    public float transformPositionY;
    public float transformPositionZ;
    public float transformRotationX;
    public float transformRotationY;
    public float transformRotationZ;
    public float transformScaleX;
    public float transformScaleY;
    public float transformScaleZ;
}