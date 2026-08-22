using System.Runtime.InteropServices;
using DreamEngine.ECS;

namespace DreamEngine.Core;
internal static class SceneManager
{
    internal static class NativeMethods
    {

        [DllImport(
            CoreConstants.DllName, 
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_SceneManagerCreateEntity")]
        public static extern uint CreateEntity(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string tag,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string name);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_SceneManagerDestroyEntity")]
        public static extern bool DestroyEntity(
            [MarshalAs(UnmanagedType.LPUTF8Str)] uint entityId);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_SceneManagerSetMainCamera")]
        public static extern bool SetMainCamera(uint entityId);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_SceneManagerSetShowCursor")]
        public static extern bool SetShowCursor(bool showCursor);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_SceneManagerSetGlobalLight")]
        public static extern bool SetGlobalLight(float r, float g, float b, float intensity);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_SceneManagerChangeScene")]
        public static extern bool ChangeScene(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string sceneName);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_PrefabInstantiate")]
        public static extern int PrefabInstantiate(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string resourceId);
    }

    internal static Entity CreateEntity(string tag, string name = "")
    {
        var id = NativeMethods.CreateEntity(tag, name);

        return new Entity
        {
            Id = id
        };
    }

    internal static bool DestroyEntity(uint entityId) => NativeMethods.DestroyEntity(entityId);

    internal static bool PrefabInstantiate(string resourceId)
    {
        var id = NativeMethods.PrefabInstantiate(resourceId);

        return id >= 0;
    }

    internal static bool SetMainCamera(Entity entity) => NativeMethods.SetMainCamera(entity.Id);

    internal static bool SetShowCursor(bool showCursor) => NativeMethods.SetShowCursor(showCursor);

    internal static bool SetGlobalLight(float r, float g, float b, float intensity) =>
        NativeMethods.SetGlobalLight(r, g, b, intensity);

    internal static bool SetCamera(uint entityId) => NativeMethods.SetMainCamera(entityId);

    internal static bool ChangeScene(string sceneName) => NativeMethods.ChangeScene(sceneName);
}
