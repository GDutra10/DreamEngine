using DreamEngine.ECS;
using DreamEngine.ECS.Components;

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

    protected Entity CreateEntity(string tag = "default", string name = "") 
        => Game.Scene.CreateEntity(tag, name);

    protected T? GetComponent<T>() where T : Component
    {
        return Entity.GetComponent<T>();
    }
}
