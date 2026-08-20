namespace DreamEngine.ECS.Components;

public class AudioListenerComponent : Component
{
    public bool Enabled { get; set; }

    internal AudioListenerComponent(uint entityId) : base(entityId)
    {
    }
}
