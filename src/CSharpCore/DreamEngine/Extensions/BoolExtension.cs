namespace DreamEngine.Extensions;

internal static class BoolExtension
{
    public static byte ToByte(this bool value) => (byte)(value ? 1 : 0);
}
