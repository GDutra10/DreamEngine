using DreamEngine.Scripting;
using DreamEngine.Sync.Data;
using Component = DreamEngine.ECS.Components.Component;

namespace DreamEngine.ECS;
public class Entity
{
    public Transform Transform { get; private set; } = new();
    public Script? Script { get; internal set; }
    
    private readonly List<Component> _components = [];
    private EntityData _entityData;

    internal uint Id { get; set; }
    
    internal Entity() { }

    public T? GetComponent<T>() where T : Component
    {
        foreach (var component in _components)
        {
            if (component is T typedComponent)
            {
                return typedComponent;
            }
        }

        return null;
    }

    internal void AddComponent<T>(T component) where T : Component
    {
        _components.Add(component);
    }

    internal void RemoveComponent<T>(T component) where T : Component
    {
        _components.Remove(component);
    }

    internal void SetEntityData(ref EntityData data)
    {
        _entityData = data;
    }

    internal ref EntityData GetEntityData()
    {
        return ref _entityData;
    }
}
