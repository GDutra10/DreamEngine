using System.Runtime.InteropServices;
using DreamEngine.Audio;
using DreamEngine.ECS;
using DreamEngine.Extensions;

namespace DreamEngine.Core;

internal static class AudioSystem
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct CoreAudioPlayOptions
    {
        public float volume;
        public float pich;
        public byte loop;
        public byte spatial;
        public uint bus;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct CoreAudioHandle
    {
        public uint id;
        public uint generation;
    }

    internal static class NativeMethods
    {
        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_AudioSystemPlay")]
        public static extern CoreAudioHandle AudioSystemPlay(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string resourceId,
            CoreAudioPlayOptions audioPlayOptions);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_AudioSystemPlayByEntity")]
        public static extern CoreAudioHandle AudioSystemPlayByEntity(
            uint entityId,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string resourceId,
            CoreAudioPlayOptions audioPlayOptions);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_AudioSystemStop")]
        public static extern void AudioSystemStop(CoreAudioHandle handle);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_AudioSystemPause")]
        public static extern void AudioSystemPause(CoreAudioHandle handle);

        [DllImport(
            CoreConstants.DllName,
            CallingConvention = CallingConvention.Cdecl,
            EntryPoint = "Core_AudioSystemResume")]
        public static extern void AudioSystemResume(CoreAudioHandle handle);
    }

    internal static AudioHandle Play(string resourceId, AudioPlayOptions audioPlayOptions)
    {
        CoreAudioHandle nativeHandle = NativeMethods.AudioSystemPlay(resourceId, audioPlayOptions.ToCoreAudioPlayOptions());

        return new AudioHandle(nativeHandle.id, nativeHandle.generation);
    }

    internal static AudioHandle Play(uint entityId, string resourceId, AudioPlayOptions audioPlayOptions)
    {
        CoreAudioHandle nativeHandle = NativeMethods.AudioSystemPlayByEntity(entityId, resourceId, audioPlayOptions.ToCoreAudioPlayOptions());

        return new AudioHandle(nativeHandle.id, nativeHandle.generation);
    }

    internal static void Stop(AudioHandle handle)
    {
        NativeMethods.AudioSystemStop(handle.ToCoreAudioHandle());
    }

    internal static void Pause(AudioHandle handle)
    {
        NativeMethods.AudioSystemPause(handle.ToCoreAudioHandle());
    }

    internal static void Resume(AudioHandle handle)
    {
        NativeMethods.AudioSystemResume(handle.ToCoreAudioHandle());
    }
}
