namespace DreamEngine.ECS.Components;

public class AudioEmitterComponent : Component
{
    public bool Enabled { get; set; } = true;
    public float Volume { get; set; } = 1.0f;
    public float Pitch { get; set; } = 1.0f;
    public bool Spatial { get; set; } = false;
    public float MinDistance { get; set; } = 1.0f;
    public float MaxDistance { get; set; } = 100.0f;

    internal AudioEmitterComponent(uint entityId) : base(entityId)
    {
    }
}
