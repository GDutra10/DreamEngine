 using System.Runtime.InteropServices;

namespace DreamEngine.Sync.Data;

[StructLayout(LayoutKind.Sequential)]
internal struct ButtonStateData
{
    public byte current;
    public byte previous;
}