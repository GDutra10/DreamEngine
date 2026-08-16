using System.Numerics;

namespace DreamEngine.ECS.Components;

public class ColliderComponent : Component
{
    public bool Enabled;
    public bool IsTrigger;
    public Vector3 Center = Vector3.Zero;
    public Vector3 Size = Vector3.Zero;

    public ColliderComponent(uint entityId) : base(entityId)
    {
    }
}
