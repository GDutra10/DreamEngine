namespace DreamEngine.Audio;

public class AudioPlayOptions
{
    public float Volume { get; set; } = 1.0f;
    public float Pitch { get; set; } = 1.0f;

    public bool Loop { get; set; }
    public bool Spatial { get; set; }

    public AudioBus Bus { get; set; } = AudioBus.Master;
}
