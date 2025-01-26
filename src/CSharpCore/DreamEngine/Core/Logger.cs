using System.Runtime.InteropServices;

namespace DreamEngine.Core;

public static class Logger
{
    private static class NativeMethods
    {
        [DllImport("Core.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Log(string message);

        [DllImport("Core.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void LogError(string message);

        [DllImport("Core.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void LogInfo(string message);
    }

    // Temporally solution

    private static void Log(string level, string message)
    {
        var now = DateTime.Now.ToString("dd/MM/yyyyy HH:mm:ss:ffff");
        
        Console.WriteLine($"{now} {level}[SCRIPT] {message}");
    }

    public static void Log(string message)
    {
        Log("", message);
    }

    public static void LogTrace(string message)
    {
        Log("[TRACE]", message);
    }

    public static void LogDebug(string message)
    {
        Log("[DEBUG]", message);
    }

    public static void LogInfo(string message)
    {
        Log("[INFO]", message);
    }

    public static void LogWarning(string message)
    {
        Log("[WARN]", message);
    }

    public static void LogError(string message)
    {
        Log("[ERROR]", message);
    }

    
}