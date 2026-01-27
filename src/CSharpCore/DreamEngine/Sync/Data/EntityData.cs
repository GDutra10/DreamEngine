using System.Runtime.InteropServices;
using System.Text;

namespace DreamEngine.Sync.Data;

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct EntityData
{
    public uint id;
    public float transformPositionX;
    public float transformPositionY;
    public float transformPositionZ;
    public float transformRotationX;
    public float transformRotationY;
    public float transformRotationZ;
    public float transformScaleX;
    public float transformScaleY;
    public float transformScaleZ;
    public float cameraFov;
    public float cameraNear;
    public float cameraFar;
    public float directionalLightSpecularX;
    public float directionalLightSpecularY;
    public float directionalLightSpecularZ;
    public float directionalLightColorRed;
    public float directionalLightColorGreen;
    public float directionalLightColorBlue;
    public float directionalLightColorAlpha;
    public float directionalLightInfluence;

    public byte uiComponentHas;
    public byte cameraComponentHas;
    public byte directionalLightComponentHas;
    public fixed byte scriptAssemblyName[256];
    public fixed byte scriptClassName[256];

    public string GetAssemblyName()
    {
        fixed (byte* p = scriptAssemblyName)
            return ReadUtf8Z(p, 256);
    }

    public string GetClassName()
    {
        fixed (byte* p = scriptClassName)
            return ReadUtf8Z(p, 256);
    }

    private static string ReadUtf8Z(byte* ptr, int maxLen)
    {
        var len = 0;
        
        while (len < maxLen && ptr[len] != 0) 
            len++;

        return Encoding.UTF8.GetString(ptr, len);
    }
}