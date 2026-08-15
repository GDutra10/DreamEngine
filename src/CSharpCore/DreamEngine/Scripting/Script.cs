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
    }

    // events
    public virtual void OnTriggerEnter(Entity other) { }
    public virtual void OnTriggerStay(Entity other) { }
    public virtual void OnTriggerExit(Entity other) { }

    public virtual void OnCollisionEnter(Entity other) { }
    public virtual void OnCollisionStay(Entity other) { }
    public virtual void OnCollisionExit(Entity other) { }

    protected Entity CreateEntity(string tag = "default", string name = "") 
        => Game.Scene.CreateEntity(tag, name);

    protected T? GetComponent<T>() where T : Component => Entity.GetComponent<T>();

    protected T AddComponent<T>() where T : Component => Entity.AddComponent<T>();

    protected void RemoveComponent<T>() where T : Component => Entity.RemoveComponent<T>();
}
