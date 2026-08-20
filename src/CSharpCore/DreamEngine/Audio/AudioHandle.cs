namespace DreamEngine.Audio;

public class AudioHandle
{
    public uint Id { get; init; }
    public uint Generation { get; init; }

    internal AudioHandle(uint id, uint generation)
    {
        Id = id;
        Generation = generation;
    }
}
