using DreamEngine.ECS.Components;

namespace DreamEngine.ECS;
public class Entity
{
    public Transform Transform { get; private set; } = new();
    private readonly List<Component> _components = [];
    internal uint Id { get; set; }

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
}
