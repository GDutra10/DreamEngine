using DreamEngine.Audio;
using static DreamEngine.Core.AudioSystem;

namespace DreamEngine.Extensions;

internal static class AudioExtensions
{
    internal static CoreAudioPlayOptions ToCoreAudioPlayOptions(this AudioPlayOptions audioPlayOptions)
    {
        return new CoreAudioPlayOptions
        {
            pich = audioPlayOptions.Pitch,
            volume = audioPlayOptions.Volume,
            bus = (uint)audioPlayOptions.Bus,
            loop = audioPlayOptions.Loop.ToByte(),
            spatial = audioPlayOptions.Spatial.ToByte(),
        };
    }

    internal static CoreAudioHandle ToCoreAudioHandle(this AudioHandle handle)
    {
        return new CoreAudioHandle
        {
            id = handle.Id,
            generation = handle.Generation
        };
    }
}
