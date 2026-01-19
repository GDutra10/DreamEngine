
namespace DreamEngine.ECS.Components;
public class CameraComponent : Component
{
    public float Fov { get; set; } = 45.0f;
    public float Near { get; set; } = 0.1f;
    public float Far { get; set; } = 100.0f;

    internal CameraComponent(uint entityId) : base(entityId) {}
}
