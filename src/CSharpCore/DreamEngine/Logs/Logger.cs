using System;
using System.Globalization;

namespace DreamEngine.Managers;

public static class Logger 
{
    public static void LogTrace(string message)
    {
        Log("TRACE", message);
    }

    public static void LogInfo(string message)
    {
        Log("INFO", message);
    }

    public static void LogDebug(string message)
    {
        Log("DEBUG", message);
    }

    public static void LogWarning(string message)
    {
        Log("WARNING", message);
    }

    public static void LogError(string message)
    {
        Log("ERROR", message);
    }

    private static void Log(string level, string message)
    {
        var nowStr = DateTime.Now.ToString("dd/MM/yyyy hh:mm:ss:fff", CultureInfo.InvariantCulture);
        Console.WriteLine($"{nowStr} [SCRIPT][{level}] {message}");
    }
}