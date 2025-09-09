using System.Runtime.InteropServices;

namespace DreamEngine.Core;

public static class Logger
{
    private static class NativeMethods
    {
        private const string CoreDll = "Core"; // or "Core.dll" on Windows


        [DllImport(CoreDll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_LogTrace")]
        public static extern void LogTrace([MarshalAs(UnmanagedType.LPUTF8Str)] string message);

        [DllImport(CoreDll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_LogDebug")]
        public static extern void LogDebug([MarshalAs(UnmanagedType.LPUTF8Str)] string message);

        [DllImport(CoreDll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_LogInfo")]
        public static extern void LogInfo([MarshalAs(UnmanagedType.LPUTF8Str)] string message);

        [DllImport(CoreDll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_LogWarning")]
        public static extern void LogWarning([MarshalAs(UnmanagedType.LPUTF8Str)] string message);

        [DllImport(CoreDll, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_LogError")]
        public static extern void LogError([MarshalAs(UnmanagedType.LPUTF8Str)] string message);
    }

    private const string Template = "[SCRIPT] {0}";

    private static string GetMessage(string message) => string.Format(Template, message);

    public static void LogTrace(string message)
    {
        NativeMethods.LogTrace(GetMessage(message));
    }

    public static void LogDebug(string message)
    {
        NativeMethods.LogDebug(GetMessage(message));
    }

    public static void LogInfo(string message)
    {
        NativeMethods.LogInfo(GetMessage(message));
    }

    public static void LogWarning(string message)
    {
        NativeMethods.LogWarning(GetMessage(message));
    }

    public static void LogError(string message)
    {
        NativeMethods.LogError(GetMessage(message));
    }
}