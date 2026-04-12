using DreamEngine.Math;

namespace DreamEngine.ECS;
public class Transform
{
    protected readonly Entity _owner;

    internal Transform(Entity owner)
    {
        _owner = owner;
        Position = new Vector3(_owner);
        Rotation = new Vector3(_owner);
        Scale = new Vector3(_owner);
    }

    public Vector3 Position { get; internal set; }

    public Vector3 Rotation { get; internal set; }

    public Vector3 Scale { get; internal set; }
}
