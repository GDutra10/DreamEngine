namespace DreamEngine.ECS.Components;

public abstract class Component
{
    internal uint EntityId { get; init; }

    internal Component(uint entityId)
    {
        EntityId = entityId;
    }
}
