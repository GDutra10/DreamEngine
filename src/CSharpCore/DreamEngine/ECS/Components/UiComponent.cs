namespace DreamEngine.ECS.Components;

public class UiComponent : Component
{
    internal UiComponent(uint entityId) : base(entityId)
    {
    }

    public void Set(string prop, string value) 
    {
        Core.UiManager.NativeMethods.Set(EntityId, prop, value);
    }
    
    public void Set(string prop, int value)
    {
        Core.UiManager.NativeMethods.Set(EntityId, prop, value);
    }

    public void Set(string prop, float value)
    {
        Core.UiManager.NativeMethods.Set(EntityId, prop, value);
    }

    public bool BindOnClickEvent(string prop, Delegate eventHandler) => Core.UiManager.BindOnClickEvent(EntityId, prop, eventHandler);
}
