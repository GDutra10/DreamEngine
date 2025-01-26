using DreamEngine.ECS;

namespace DreamEngine.Scripting;

public class Script
{
    public Entity Entity { get; private set; } = new();
    
    private bool _isInitialized = false;

    public void Initialize()
    {
        if (_isInitialized)
            return;

        this.Start();
        _isInitialized = true;
    }

    public virtual void Start()
    {
    }

    public virtual void Update()
    {
        Console.WriteLine("Hello from .NET!");
    }
}
