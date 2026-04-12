using DreamEngine.Commands;
using DreamEngine.Scripting;
using DreamEngine.Sync.Data;
using System.Globalization;
using System.Reflection;
using Component = DreamEngine.ECS.Components.Component;

namespace DreamEngine.ECS;
public class Entity
{
    public Transform Transform { get; init; }
    public Script? Script { get; internal set; }
    public Entity? Parent
    {
        get => InternalParent;
        set
        {
            if (MustProcessCommandsNextUpdate)
                CommandQueue.Add(new ParentCommand(this, value));
            else
                InternalParent = value;
        }
    }

    public IReadOnlyList<Entity> Children => [.. Game.Scene.Entities.Values.Where(x => x.Parent is not null && x.Parent.Id == Id)];

    internal Entity? InternalParent;

    internal readonly HashSet<Component> _components = [];
    internal EntityData _entityData;
    internal bool MustProcessCommandsNextUpdate { get; set; }

    internal uint Id { get; set; }

    internal Entity() 
    {
        Transform = new Transform(this);
    }

    public T? GetComponent<T>() where T : Component
    {
        // TODO: improve this
        foreach (var component in _components)
        {
            if (component is T typedComponent)
            {
                return typedComponent;
            }
        }

        return null;
    }

    public T AddComponent<T>() where T : Component
    {
        var component = GetComponent<T>();

        if (component is not null)
            return component;

        component = (T)Activator.CreateInstance(
                type: typeof(T),
                bindingAttr: BindingFlags.Instance | BindingFlags.NonPublic,
                binder: null,
                args: [Id],
                culture: CultureInfo.CurrentCulture)!;

        AddComponent(component);

        return component;
    }

    public void RemoveComponent<T>() where T : Component
    {
        var component = GetComponent<T>();

        if (component is not null)
            RemoveComponent(component);
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
