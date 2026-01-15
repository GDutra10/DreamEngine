using DreamEngine.ECS;

namespace DreamEngine.Scripting;

public class Script
{
    public Entity Entity => _entity!;

    private Entity? _entity;

    private bool HasBeenInitialized => _entity is not null;

    internal void Initialize(Entity entity)
    {
        if (HasBeenInitialized)
            return;

        _entity = entity;

        this.Start();
    }

    public virtual void Start()
    {
    }

    public virtual void Update()
    {
        Console.WriteLine("Hello from .NET!");
    }
}
