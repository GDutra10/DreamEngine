using System;
using System.Runtime.InteropServices;

namespace DreamEngine.Scripting;

[StructLayout(LayoutKind.Sequential)]
internal struct ScriptInfo
{
    public IntPtr AssemblyName { get; set; }
    public IntPtr Name { get; set; }
}