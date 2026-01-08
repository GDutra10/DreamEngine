using System.Runtime.InteropServices;

namespace DreamEngine.Core;

internal static class UiManager
{
    private static int _nextEventId = 1;
    static Dictionary<int, Delegate> _eventHandlers = [];

    internal static class NativeMethods
    {
        [DllImport(CoreConstants.DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_UiManagerSetString")]
        public static extern bool Set(
            uint entityId,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string prop,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string value);

        [DllImport(CoreConstants.DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_UiManagerSetInt")]
        public static extern bool Set(
            uint entityId,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string prop,
            int value);

        [DllImport(CoreConstants.DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_UiManagerSetFloat")]
        public static extern bool Set(
            uint entityId,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string prop,
            float value);

        [DllImport(CoreConstants.DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Core_UiManagerBindOnClickCallback")]
        public static extern bool BindOnClickEvent(
            uint entityId,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string e,
            int eventId);
    }

    public static bool BindOnClickEvent(uint entityId, string e, Delegate handler)
    {
        var eventId = _nextEventId++;

        if (!NativeMethods.BindOnClickEvent(entityId, e, eventId))
        {
            _nextEventId--;

            return false;
        }

        _eventHandlers[eventId] = handler;

        return true;
    }

    public static void HandleEvent(int eventId)
    {
        if (_eventHandlers.TryGetValue(eventId, out var eventHandler))
            eventHandler.DynamicInvoke();
    }
}
